// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "actions.hh"
#include "action_tourner_case.hh"

int ActionActiverAigle::check(const GameState& st) const
{
    if (!st.init)
        return HORS_TOUR;
    std::vector<Joueur> joueurs = st.joueurs;
    Aigle* aigle = joueurs[player_id_].trouve_aigle(id_);
    if (aigle == NULL)
        return AIGLE_INVALIDE;
    Aigle aiglantine = *aigle;
    if (!(aiglantine.effet == EFFET_RAZ_DE_MAREE || aiglantine.effet == EFFET_ACTIONS
          || aiglantine.effet == EFFET_EFFRAYER))
        return AIGLE_INVALIDE;
    if (aiglantine.effet == EFFET_ACTIONS && aiglantine.puissance < 0)
        return AIGLE_INVALIDE;
    return OK;
}

bool envoler_aigle_id(std::vector<Aigle> aigles, int id)
{
    for (std::vector<Aigle>::iterator it = aigles.begin(); it != aigles.end(); it++)
    {
        if (it->identifiant == id)
        {
            aigles.erase(it);
            return true;
        }
    }
    return false;
}

bool envoler_aigle_pos(std::vector<Aigle> aigles, position pos)
{
    bool envole = false;
    for (std::vector<Aigle>::iterator it = aigles.begin(); it != aigles.end(); it++)
    {
        if (it->pos.ligne == pos.ligne && it->pos.colonne == pos.colonne)
        {
            aigles.erase(it);
            envole = true;
        }
    }
    return envole;
}

void tourner_cases(Carte carte, Aigle aigle)
{
    for (int x = aigle.pos.colonne - aigle.puissance; x < aigle.pos.colonne + aigle.puissance; x++)
    {
        for (int y = aigle.pos.ligne - aigle.puissance; y < aigle.pos.ligne + aigle.puissance; y++)
        {
            if (carte.case_valide(x, y))
            {
                carte.set_case(x, y, rotation_case(carte.get_case(x, y)));
                carte.set_case(x, y, rotation_case(carte.get_case(x, y)));
            }
        }
    }
}

void ActionActiverAigle::apply_on(GameState* st) const
{
    std::vector<Joueur> joueurs = st->joueurs;
    Aigle* aigle = joueurs[player_id_].trouve_aigle(id_);
    if (aigle == NULL)
        return;
    Aigle aiglantine = *aigle;
    switch (aiglantine.effet)
    {
        case EFFET_RAZ_DE_MAREE:
        {
            tourner_cases(st->carte, aiglantine);
            break;
        }
        case EFFET_ACTIONS:
        {
            joueurs[player_id_].points_action += aigle->puissance;
            break;
        }
        case EFFET_EFFRAYER:
        {
            envoler_aigle_pos(joueurs[0].aigles, aiglantine.pos); //TODO YAKA pas sur
            envoler_aigle_pos(joueurs[1].aigles, aiglantine.pos);
            envoler_aigle_pos(st->aigles_sauvages, aiglantine.pos);
            break;
        }
        default:
            return;
    }
    envoler_aigle_id(joueurs[player_id_].aigles, id_);
}
