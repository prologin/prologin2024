// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2015 Association Prologin <association@prologin.org>

#pragma once

#include <fstream>
#include <vector>

#include <rules/game-state.hh>
#include <rules/player.hh>

#include "constant.hh"
#include "carte.hh"

#include "aigle.hh"
#include "joueur.hh"
#include "json.hpp" //DONE j'ai pas trouve mieux

using json = nlohmann::json;

struct ActionInterne {
    bool est_drakkar;
    action_hist action;
    drakkar_debug couleur;
};

class GameState final : public rules::GameState
{
public:
    int tour = -1;
    int round = 0;
    bool init;
    Carte carte;
    std::vector<Joueur> joueurs; // Le joueur à la position i - 1 est le joueur i
    std::vector<Aigle> aigles_sauvages; // Les aigles qui n'appartiennent pas
                                      // encore à un joueur
    std::vector<position> villages_libres;
    std::vector<std::vector<ActionInterne>> historiques;

    // FIXME
    // additional parameters? for instance map
    GameState(const rules::Players& players);
    GameState(const rules::Players& players, std::ifstream& json_file);
    GameState(const GameState& st) = default;
    ~GameState() override;

    [[nodiscard]] GameState* copy() const override;


    int calcul_score(int x, int y);

    [[nodiscard]] bool est_termine() const;
    void tour_suivant();
    [[nodiscard]] int joueur_actuel() const;
    void debute_tour(int joueur);
    bool emplacement_dans_territoire(int joueur, position pos);

    void ajoute_historique(action_hist action);
    void pose_drakkar(position pos, drakkar_debug couleur);
    bool annuler();

    [[nodiscard]] json dump() const;
};
