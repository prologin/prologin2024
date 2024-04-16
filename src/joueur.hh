#pragma once

#include <vector>

#include "aigle.hh"

struct Joueur
{
    int score;
    int points_action;
    std::vector<Aigle> aigles;
    std::vector<position> villages;

    std::vector<std::vector<bool>> territoire(const Carte& carte) const;
};
