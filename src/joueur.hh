#pragma once

#include <vector>
#include "carte.hh"

#include "aigle.hh"

struct Joueur
{
    Joueur() = default;
    Joueur(int score, int points_action, std::vector<Aigle> aigles, std::vector<position> villages);
    int score;
    int points_action;
    std::vector<Aigle> aigles;
    std::vector<position> villages;

    std::vector<std::vector<bool>> territoire(const Carte& carte) const;
    std::vector<Aigle>::const_iterator trouve_aigle(int aigle_id) const;
    std::vector<Aigle>::iterator trouve_aigle(int aigle_id);
};
