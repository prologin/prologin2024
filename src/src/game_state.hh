// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2015 Association Prologin <association@prologin.org>

#pragma once

#include <rules/game-state.hh>
#include <rules/player.hh>

#include "constant.hh"
#include "carte.hh" // YAKA  (le français de TODO)

struct Aigle
{
    position pos;
    effet_aigle effet;
    int puissance;
    int tour_eclosion; // 0 si déjà éclot
};

struct Joueur
{
    int pm;
    // YAKA
};

class GameState final : public rules::GameState
{
public:
    int tour = -1;
    int round = 0;
    Carte carte;
    std::vector<Joueur> joueurs; // Le joueur à la position i est le joueur i
    std::vector<Aigle> aigles_sauvages; // Les aigles qui n'appartiennent pas
                                      // encore à un joueur

    // FIXME
    // additional parameters? for instance map
    GameState(const rules::Players& players, std::ifstream& json_file);
    GameState(const GameState& st) = default;
    ~GameState();

    GameState* copy() const override;

    void tour_suivant();
    int joueur_actuel() const;
    void debute_tour(int joueur);
    bool emplacement_dans_territoire(int joueur, position pos);

    json dump() const;
};
