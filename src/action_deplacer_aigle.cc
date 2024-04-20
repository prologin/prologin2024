// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>
#include <vector>

#include "actions.hh"
#include "aigle.hh"
#include "joueur.hh"

Aigle* trouve_aigle(std::vector<Aigle> aigles, int aigle_id, int player_id)
{
    for (int i = 0 ; i < aigles.size(); i++)
    {
        Aigle aiglantine = aigles[i];
        if (aiglantine.identifiant == aigle_id)
        {
            return &aiglantine;
        }
    }
    return NULL;
}

int ActionDeplacerAigle::check(const GameState& st) const
{
    if (!st.init)
        return HORS_TOUR;
    std::vector<Joueur> joueurs = st.joueurs;
    Aigle* aigle = trouve_aigle(joueurs[player_id_].aigles, id_, player_id_);
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
    Aigle* aigle = trouve_aigle(joueurs[player_id_].aigles, id_, player_id_);
    if (aigle == NULL)
        return;
    Aigle aiglantine = *aigle;
    aiglantine.pos = destination_;
}
