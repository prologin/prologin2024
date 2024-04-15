// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "api.hh"

#include <memory>
#include <utility>

// global used in interface.cc
Api* api;

Api::Api(std::unique_ptr<GameState> game_state,
         std::shared_ptr<rules::Player> player)
    : rules::Api<GameState, erreur>(std::move(game_state), player)
{
    api = this;
}

dimension Api::dimensions_carte()
{
    // TODO
    abort();
}
etat_case Api::info_case(position pos)
{
    // TODO
    abort();
}
std::vector<aigle> Api::info_aigles()
{
    // TODO
    abort();
}
int Api::points_action(int joueur)
{
    // TODO
    abort();
}
int Api::score(int joueur)
{
    // TODO
    abort();
}
std::vector<action_hist> Api::historique()
{
    // TODO
    abort();
}
int Api::moi()
{
    // TODO
    abort();
}
int Api::adversaire()
{
    // TODO
    abort();
}
bool Api::annuler()
{
    // TODO
    abort();
}
int Api::tour_actuel()
{
    // TODO
    abort();
}
void Api::afficher_erreur(erreur v)
{
    // TODO
    abort();
}
void Api::afficher_type_case(type_case v)
{
    // TODO
    abort();
}
void Api::afficher_drakkar_debug(drakkar_debug v)
{
    // TODO
    abort();
}
void Api::afficher_type_action(type_action v)
{
    // TODO
    abort();
}
void Api::afficher_effet_aigle(effet_aigle v)
{
    // TODO
    abort();
}
void Api::afficher_position(position v)
{
    // TODO
    abort();
}
void Api::afficher_dimension(dimension v)
{
    // TODO
    abort();
}
void Api::afficher_aigle(aigle v)
{
    // TODO
    abort();
}
void Api::afficher_etat_case(etat_case v)
{
    // TODO
    abort();
}
void Api::afficher_action_hist(action_hist v)
{
    // TODO
    abort();
}
