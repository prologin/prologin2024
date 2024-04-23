#include "aigle.hh"

Aigle::Aigle(int identifiant, position pos, effet_aigle effet, int puissance, int tour_eclosion)
    : identifiant{identifiant},
    pos{pos},
    effet{effet},
    puissance{puissance},
    tour_eclosion{tour_eclosion}
{}
