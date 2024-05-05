// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include <algorithm>

#include "actions.hh"
#include "action_tourner_case.hh"

int ActionActiverAigle::check(const GameState& st) const
{
    if (!st.init)
        return HORS_TOUR;
    auto& joueur = st.joueurs[player_id_];
    const auto aiglantine = joueur.trouve_aigle(id_);
    if (aiglantine == joueur.aigles.end())
        return AIGLE_INVALIDE;

    if (!(aiglantine->effet == EFFET_RAZ_DE_MAREE ||
        aiglantine->effet == EFFET_ACTIONS ||
        aiglantine->effet == EFFET_EFFRAYER))
        return AIGLE_INVALIDE;
    return OK;
}

bool envoler_aigle_id(std::vector<Aigle> aigles, int id)
{
    auto aigle_correct = [id](Aigle aigle) {
        return aigle.identifiant == id;
    };
    auto it = std::find_if(aigles.begin(), aigles.end(), aigle_correct);
    if (it == aigles.end())
        return false;
    aigles.erase(it);
    return true;
}

void envoler_aigle_pos(std::vector<Aigle> aigles, position pos, int tour_actuel)
{
    auto aigle_correct = [pos, tour_actuel](Aigle aigle) {
        return aigle.pos.colonne != pos.colonne ||
            aigle.pos.ligne != pos.ligne ||
            aigle.tour_eclosion < tour_actuel;
    };
    std::vector<Aigle> aigles_survivants;
    std::copy_if(aigles.begin(), aigles.end(),
                 std::back_inserter(aigles_survivants), aigle_correct);
    aigles = aigles_survivants;
}

void tourner_cases(Carte& carte, Aigle& aigle)
{
    for (int x = aigle.pos.colonne - aigle.puissance; x <= aigle.pos.colonne + aigle.puissance; x++)
    {
        for (int y = aigle.pos.ligne - aigle.puissance; y <= aigle.pos.ligne + aigle.puissance; y++)
        {
            if (carte.case_valide(x, y))
            {
                for (int i = 0; i < 2; i++)
                    carte.set_case(x, y, rotation_case(carte.get_case(x, y)));
            }
        }
    }
}

void ActionActiverAigle::apply_on(GameState* st) const
{
    auto& joueur = st->joueurs[player_id_];
    auto aiglantine = joueur.trouve_aigle(id_);

    switch (aiglantine->effet)
    {
        case EFFET_RAZ_DE_MAREE:
        {
            tourner_cases(st->carte, *aiglantine);
            break;
        }
        case EFFET_ACTIONS:
        {
            joueur.points_action += aiglantine->puissance;
            break;
        }
        case EFFET_EFFRAYER:
        {
            envoler_aigle_pos(st->joueurs[0].aigles, aiglantine->pos, st->tour);
            envoler_aigle_pos(st->joueurs[1].aigles, aiglantine->pos, st->tour);
            envoler_aigle_pos(st->aigles_sauvages, aiglantine->pos, st->tour);
            break;
        }
        default:
            return;
    }
    envoler_aigle_id(joueur.aigles, id_);
}
