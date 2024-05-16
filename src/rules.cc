// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "rules.hh"

#include <memory>
#include <utility>

#include "actions.hh"

Rules::Rules(const rules::Options opt)
    : TurnBasedRules(opt)
    , sandbox_(opt.time)
{
    if (!opt.champion_lib.empty())
    {
        champion_dll_ = std::make_unique<utils::DLL>(opt.champion_lib);

        champion_partie_init_ =
            champion_dll_->get<f_champion_partie_init>("partie_init");
        champion_jouer_tour_ =
            champion_dll_->get<f_champion_jouer_tour>("jouer_tour");
        champion_partie_fin_ =
            champion_dll_->get<f_champion_partie_fin>("partie_fin");
    }

    std::ifstream is(opt.map_file);
    auto game_state = std::make_unique<GameState>(opt.players, is);
    api_ = std::make_unique<Api>(std::move(game_state), opt.player);
    register_actions();
}

void Rules::register_actions()
{
    api_->actions()->register_action(ID_ACTION_TOURNER_CASE,
                                     [] { return std::make_unique<ActionTournerCase>(); });
    api_->actions()->register_action(ID_ACTION_ACTIVER_AIGLE,
                                     [] { return std::make_unique<ActionActiverAigle>(); });
    api_->actions()->register_action(ID_ACTION_DEPLACER_AIGLE,
                                     [] { return std::make_unique<ActionDeplacerAigle>(); });
    api_->actions()->register_action(ID_ACTION_DEBUG_POSER_DRAKKAR,
                                     [] { return std::make_unique<ActionDebugPoserDrakkar>(); });
}

rules::Actions* Rules::get_actions()
{
    return api_->actions();
}

void Rules::apply_action(const rules::IAction& action)
{
    // When receiving an action, the API should have already checked that it
    // is valid. We recheck that for the current gamestate here to avoid weird
    // client/server desynchronizations and make sure the gamestate is always
    // consistent across the clients and the server.

    int err = api_->game_state_check(action);
    if (err)
        FATAL("Synchronization error: received action %d from player %d, but "
              "check() on current gamestate returned %d.",
              action.id(), action.player_id(), err);
    api_->game_state_apply(action);
}

bool Rules::is_finished()
{
    // FIXME
    // return true;
    return api_->game_state().est_termine();
}

void Rules::at_player_start(rules::ClientMessenger_sptr)
{
    try
    {
        sandbox_.execute(champion_partie_init_);
    }
    catch (utils::SandboxTimeout&)
    {
        FATAL("player_start: timeout");
    }
}

void Rules::at_spectator_start(rules::ClientMessenger_sptr)
{
    champion_partie_init_();
}

void Rules::at_player_end(rules::ClientMessenger_sptr)
{
    try
    {
        sandbox_.execute(champion_partie_fin_);
    }
    catch (utils::SandboxTimeout&)
    {
        FATAL("player_end: timeout");
    }
}

void Rules::at_spectator_end(rules::ClientMessenger_sptr)
{
    champion_partie_fin_();
}

void Rules::player_turn()
{
    try
    {
        sandbox_.execute(champion_jouer_tour_);
    }
    catch (utils::SandboxTimeout&)
    {
        FATAL("player_turn: timeout");
    }
}

void Rules::spectator_turn()
{
    champion_jouer_tour_();
}

void Rules::start_of_player_turn(unsigned int player_key)
{
    api_->game_state().debute_tour(player_key);
    api_->game_state().init = true;
}

void Rules::end_of_player_turn(unsigned int player_key)
{
    api_->game_state().init = false;
    //api_->game_state().sync_score();
    api_->game_state().tour_suivant();
    api_->clear_old_game_states();
}

void Rules::start_of_round()
{}
void Rules::end_of_round()
{}
