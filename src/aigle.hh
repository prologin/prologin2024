#pragma once

#include "constant.hh"

struct Aigle
{
    Aigle() = default;
    Aigle(int identifiant, position pos, effet_aigle effet, int puissance, int tour_eclosion);
    int identifiant;
    position pos;
    effet_aigle effet;
    int puissance;
    int tour_eclosion; // 0 si déjà éclot
};