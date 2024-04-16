#pragma once

#include <vector>

struct Joueur
{
    int score;
    int points_action;
    std::vector<Aigle> aigles;
    std::vector<position> villages;

    int territoire(const Carte& carte);
};
