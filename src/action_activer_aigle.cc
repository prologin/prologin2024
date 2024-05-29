// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include <algorithm>

#include "action_tourner_case.hh"

int ActionActiverAigle::check(const GameState& st) const
{
    if (!st.init)
        return HORS_TOUR;
    const auto& joueur = st.joueurs[player_id_];
    const auto *const aiglantine = joueur.trouve_aigle(id_);
    if (aiglantine == nullptr)
        return AIGLE_INVALIDE;

    if (aiglantine->effet != EFFET_METEORE &&
        aiglantine->effet != EFFET_VIE &&
        aiglantine->effet != EFFET_MORT)
        return AIGLE_INVALIDE;
    return OK;
}

bool envoler_aigle_id(std::vector<Aigle>& aigles, int id)
{
    const auto aigle_correct = [id](const Aigle &aigle) {
        return aigle.identifiant == id;
    };
    const auto it = std::find_if(aigles.cbegin(), aigles.cend(), aigle_correct);
    if (it == aigles.cend())
        return false;
    aigles.erase(it);
    return true;
}

void envoler_aigle_pos(std::vector<Aigle>& aigles, position pos, int tour_actuel, int puiss)
{
    const auto aigle_correct = [pos, tour_actuel, puiss](const Aigle &aigle) {
        return aigle.pos.colonne + puiss <= pos.colonne && aigle.pos.colonne - puiss >= pos.colonne &&
            aigle.pos.ligne + puiss <= pos.ligne && aigle.pos.ligne - puiss >= pos.ligne &&
            aigle.tour_eclosion <= tour_actuel;
    };
    const auto erase_iterator = std::remove_if(aigles.begin(),
            aigles.end(), aigle_correct);
    aigles.erase(erase_iterator, aigles.end());
}

void tourner_cases(GameState& st, Carte& carte, Aigle& aigle)
{
    for (int x = aigle.pos.colonne - aigle.puissance; x <= aigle.pos.colonne + aigle.puissance + 1; x++)
    {
        for (int y = aigle.pos.ligne - aigle.puissance; y <= aigle.pos.ligne + aigle.puissance + 1; y++)
        {
            if (carte.case_valide(x, y))
            {
                for (int i = 0; i < 2; i++)
                {
                    if (!case_bloquee(st, x, y))
                        carte.set_case(x, y, rotation_case(carte.get_case(x, y)));
                }
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
        case EFFET_METEORE:
        {
            tourner_cases(*st, st->carte, *aiglantine);
            break;
        }
        case EFFET_VIE:
        {
            joueur.points_action += aiglantine->puissance;
            break;
        }
        case EFFET_MORT:
        {
            envoler_aigle_pos(st->joueurs[0].aigles, aiglantine->pos, st->tour, aiglantine->puissance);
            envoler_aigle_pos(st->joueurs[1].aigles, aiglantine->pos, st->tour, aiglantine->puissance);
            envoler_aigle_pos(st->aigles_sauvages, aiglantine->pos, st->tour, aiglantine->puissance);
            break;
        }
        default:
            return;
    }
    envoler_aigle_id(joueur.aigles, id_);
    st->ajoute_historique({ACTION_ACTIVER_AIGLE, aiglantine->pos, aiglantine->pos, id_});
}
