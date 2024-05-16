// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#pragma once

#include <memory>

#include <rules/actions.hh>
#include <rules/client-messenger.hh>
#include <rules/options.hh>
#include <rules/player.hh>
#include <rules/rules.hh>
#include <rules/server-messenger.hh>
#include <utils/dll.hh>
#include <utils/sandbox.hh>

#include "api.hh"

using f_champion_partie_init = void (*)();
using f_champion_jouer_tour = void (*)();
using f_champion_partie_fin = void (*)();

class Rules : public rules::TurnBasedRules
{
public:
    explicit Rules(const rules::Options opt);
    virtual ~Rules() {}

    rules::Actions* get_actions() override;
    void apply_action(const rules::IAction& action) override;
    bool is_finished() override;

    virtual void at_player_start(rules::ClientMessenger_sptr) override;
    virtual void at_spectator_start(rules::ClientMessenger_sptr) override;
    virtual void at_player_end(rules::ClientMessenger_sptr) override;
    virtual void at_spectator_end(rules::ClientMessenger_sptr) override;

    virtual void player_turn() override;
    virtual void spectator_turn() override;

    virtual void start_of_player_turn(uint32_t) override;
    virtual void end_of_player_turn(uint32_t) override;

    virtual void start_of_round() override;
    virtual void end_of_round() override;

    virtual void dump_state(std::ostream& os);

protected:
    // FIXME: Override turnbased methods here

private:
    void register_actions();

    std::unique_ptr<utils::DLL> champion_dll_;
    std::unique_ptr<Api> api_;
    utils::Sandbox sandbox_;

    f_champion_partie_init champion_partie_init_;
    f_champion_jouer_tour champion_jouer_tour_;
    f_champion_partie_fin champion_partie_fin_;
};
