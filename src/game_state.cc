// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2015 Association Prologin <association@prologin.org>

#include "game_state.hh"

#include <iostream>

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
    int x1 = donnees["joueur1"]["x"];
    int y1 = donnees["joueur1"]["y"];
    std::vector<Aigle> aigles1;
    std::vector<position> villages1;
    villages1.push_back(vec_to_pos({x1, y1}));
    joueurs.emplace_back(id_joueur++, -1, aigles1, villages1);
    int x2 = donnees["joueur2"]["x"];
    int y2 = donnees["joueur2"]["y"];
    std::vector<Aigle> aigles2;
    std::vector<position> villages2;
    villages2.push_back(vec_to_pos({x2, y2}));
    joueurs.emplace_back(id_joueur++, -1, aigles2, villages2);

    std::vector<std::vector<int>> gains;
    for (const auto& ligne : donnees["gains"])
    {
        std::vector<int> ligne_vec;
        for (int elt : ligne)
            ligne_vec.push_back(elt);
        gains.push_back(ligne_vec);
    }

    int id = 0;
    for (const auto& aigle : donnees["aigles"])
    {
        effet_aigle effet;
        std::string effet_string = aigle["effet"];
        std::cout << aigle << std::endl;
        if (effet_string.compare("METEORE"))
            effet = EFFET_RAZ_DE_MAREE;
        else if (effet_string.compare("VIE"))
            effet = EFFET_ACTIONS;
        else if (effet_string.compare("FEU"))
            effet = EFFET_EFFRAYER;
        else if (effet_string.compare("ABONDANCE"))
            effet = EFFET_MULTIPLICATIF;
        else if (effet_string.compare("GEL"))
            effet = EFFET_BLOQUEUR;
        else
            effet = EFFET_BLOQUEUR;
        std::cout << "AAAAAAAAAAAAAAAA" << std::endl;
        Aigle a(id, {aigle["pos"]["x"], aigle["pos"]["y"]}, effet, aigle["puissance"], aigle["tour_eclosion"]);
        aigles_sauvages.push_back(a);
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

void GameState::ajoute_historique(action_hist action)
{
    historiques[joueur_actuel()].push_back({ false, action, PAS_DE_DRAKKAR });
}

void GameState::pose_drakkar(position pos, drakkar_debug couleur)
{
    action_hist vide;
    vide.fin = pos;
    historiques[joueur_actuel()].push_back({ true, vide, couleur });
}

bool GameState::annuler()
{
    if (historiques[joueur_actuel()].empty())
        return false;
    historiques[joueur_actuel()].pop_back();
    return true;
}

/* DUMPER FOR GAME STATE */

json GameState::dump() const
{
    // FIXME
    return NULL;
}
