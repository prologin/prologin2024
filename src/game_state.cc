// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2015 Association Prologin <association@prologin.org>

#include "game_state.hh"

namespace
{
    position vec_to_pos(const std::vector<int>& vec)
    {
        return { vec[0], vec[1] };
    }
} // namespace


GameState::GameState(const rules::Players& players)
        : rules::GameState(players)
{
    // should not come here
}

GameState::GameState(const rules::Players& players, std::ifstream& json_file)
    : rules::GameState(players)
{
    tour = 0;

    json donnees;
    json_file >> donnees;

    int id_joueur = 0;
    for (const auto& player : donnees["joueurs"])
    {
        std::vector<Aigle> aigles;
        std::vector<position> villages;
        villages.push_back(vec_to_pos(player));
        joueurs.emplace_back(id_joueur++, -1, aigles, villages);
    }

    std::vector<std::vector<int>> gains;
    for (const auto& ligne : donnees["gains"])
    {
        std::vector<int> ligne_vec;
        for (int elt : ligne)
            ligne_vec.push_back(elt);
        gains.push_back(ligne_vec);
    }

    std::vector<std::string> carte_texte;
    for (const std::string ligne : donnees["carte"])
        carte_texte.push_back(ligne);

    carte = Carte(carte_texte, gains);

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
    std::tie(largeur, hauteur) = carte.get_dimension();

    Joueur& j_actuel = joueurs[joueur_actuel()];
    std::vector<std::vector<bool>> territoire = j_actuel.territoire(carte);
    for (int y = 0; y < hauteur - 1; y++)
    {
        for (int x = 0; x < largeur - 1; x++)
        {
            if (territoire[y][x])
                j_actuel.score += carte.get_gain(x, y);
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
