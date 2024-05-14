#pragma once

#include "constant.hh"

struct Aigle
{
    Aigle() = default;
    Aigle(int identifiant, position pos, effet_aigle effet, int puissance, int tour_eclosion);
    // bool case_dans_rayon(int x, int y);

    int identifiant;
    position pos;
    effet_aigle effet;
    int puissance;
    int tour_eclosion;
};
