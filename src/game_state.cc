// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2015 Association Prologin <association@prologin.org>

#include "game_state.hh"

GameState::GameState(const rules::Players& players, std::ifstream& json_file)
    : rules::GameState(players)
{
    tour = 0;

    json donnees;
    json_file >> donnes;

    int id_joueur = 0;
    for (const auto& player : donnes["joueurs"])
    {
        joueurs.emplace_back(id_joueur);
        // FIXME
    }



    carte = Carte::parse(donnees["carte"], std::transform(donnees["gain"].begin(), donnees["gain"].end(), std::back_inserter(Data),
                                                          [](const std::string& str) { return std::stoi(str); }));

    // FIXME
}

GameState::~GameState()
{
    // FIXME
}

GameState* GameState::copy() const
{
    return new GameState(*this);
}

bool GameState::est_termine() const
{
    return tour >= NB_TOURS;
}

void GameState::tour_suivant()
{
    tour++;
    // FIXME
    // FIXME ajouter les scores des joueurs
}

int GameState::joueur_actuel() const
{
    return tour % (int)joueurs.size();
}

void GameState::debute_tour(int joueur)
{
    joueurs[joueur].points_action = TOUR_POINTS_ACTION;
    // FIXME
}

bool emplacement_dans_territoire(int joueur, position pos)
{
    // FIXME
    return false;
}

/* GESTIONNAIRE D'HISTORIQUE */

json GameState::dump() const
{
    // FIXME
}
