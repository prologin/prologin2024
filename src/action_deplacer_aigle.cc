// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>
#include <vector>

#include "actions.hh"
#include "aigle.hh"
#include "joueur.hh"

int ActionDeplacerAigle::check(const GameState& st) const
{
    if (!st.init)
        return HORS_TOUR;
    std::vector<Joueur> joueurs = st.joueurs;
    Aigle* aigle = joueurs[player_id_].trouve_aigle(id_);
    if (aigle == NULL)
        return AIGLE_INVALIDE;
    Aigle aiglantine = *aigle;
    if (!st.carte.emplacement_valide(destination_.colonne, destination_.ligne))
        return DESTINATION_INVALIDE;

    return OK;
}

void ActionDeplacerAigle::apply_on(GameState* st) const
{
    std::vector<Joueur> joueurs = st->joueurs;
    Aigle* aigle = joueurs[player_id_].trouve_aigle(id_);
    if (aigle == NULL)
        return;
    Aigle aiglantine = *aigle;
    aiglantine.pos = destination_;
}
