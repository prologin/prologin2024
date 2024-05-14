#include "aigle.hh"

Aigle::Aigle(int identifiant, position pos, effet_aigle effet, int puissance, int tour_eclosion)
    : identifiant{identifiant},
    pos{pos},
    effet{effet},
    puissance{puissance},
    tour_eclosion{tour_eclosion}
{}

bool Aigle::case_dans_rayon(int x, int y) const
{
    int x_min = pos.colonne - puissance;
    int x_max = pos.colonne + puissance + 1;
    int y_min = pos.ligne - puissance;
    int y_max = pos.ligne + puissance + 1;
    return x >= x_min && x <= x_max && y >= y_min && y <= y_max;
}
