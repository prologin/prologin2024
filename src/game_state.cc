// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2015 Association Prologin <association@prologin.org>

#include "game_state.hh"

GameState::GameState(const rules::Players& players, std::ifstream& json_file)
    : rules::GameState(players)
{
    tour = 0;

    json donnees;
    json_file >> donnees;

    int id_joueur = 0;
    for (const auto& player : donnees["joueurs"])
    {
        joueurs.emplace_back(id_joueur);
        // FIXME
    }

    std::vector<std::vector<int>> gains;
    for (const auto& ligne : donnees["gains"])
    {
        std::vector<int> ligne_vec;
        for (const int& elt : ligne)
            ligne_vec.push_back(elt);
        gains.push_back(ligne_vec);
    }


    carte = Carte(donnees["carte"], gains);

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
    int largeur, hauteur;
    tie(largeur, hauteur) = carte.get_dimension();

    Joueur& joueur_actuel = joueurs[joueur_actuel()];
    std::vector<std::vector<bool>> territoire = joueur_actuel.territoire();
    for (int y = 0; y < hauteur - 1; y++)
    {
        for (int x = 0; x < largeur - 1; x++)
        {
            if (territoire[y][x])
                joueur_actuel.score += carte.get_gain(x, y);
        }
    }

    tour++;
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
