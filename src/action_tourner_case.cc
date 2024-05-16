// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "actions.hh"

bool case_bloquee(const std::vector<Aigle>& aigles, int x, int y)
{
    for (const Aigle& aigle : aigles)
    {
        if (aigle.effet != EFFET_GEL)
            continue;
        if (aigle.case_dans_rayon(x, y))
            return true;
    }
    return false;
}

bool case_bloquee(const GameState& st, int x, int y)
{
    return (case_bloquee(st.joueurs[0].aigles, x, y) ||
            case_bloquee(st.joueurs[1].aigles, x, y));
}

int ActionTournerCase::check(const GameState& st) const
{
    if (!st.init)
        return HORS_TOUR;

    Carte carte = st.carte;
    int x = pos_.colonne;
    int y = pos_.ligne;
    if (!carte.case_valide(x, y))
        return POSITION_INVALIDE;
    if (case_bloquee(st, x, y))
        return CASE_BLOQUEE;

    Joueur joueur = st.joueurs[player_id_];
    Joueur adversaire = st.joueurs[player_id_ ^ 1];
    std::vector<std::vector<bool>> territoire_adverse = adversaire.territoire(st.carte);

    int cout = 1;
    for (int emplacement_y = y - 1; emplacement_y <= y; emplacement_y++)
    {
        for (int emplacement_x = x - 1; emplacement_x <= x; emplacement_x++)
        {
            if (!carte.emplacement_valide(emplacement_x, emplacement_y))
                continue;
            if (territoire_adverse[emplacement_y][emplacement_x])
                cout = 2;
        }
    }

    if (joueur.points_action < cout)
        return PLUS_DE_PA;

    return OK;
}

enum type_case rotation_case(enum type_case actuel)
{
    switch (actuel)
    {
    case NORD_OUEST:
        return SUD_OUEST;
    case SUD_OUEST:
        return SUD_EST;
    case SUD_EST:
        return NORD_EST;
    case NORD_EST:
        return NORD_OUEST;
    default:
        return actuel;
    }
}

void ActionTournerCase::apply_on(GameState* st) const
{
    Carte carte = st->carte;
    int x = pos_.colonne;
    int y = pos_.ligne;

    Joueur& joueur = st->joueurs[player_id_];
    Joueur& adversaire = st->joueurs[player_id_ ^ 1];
    std::vector<std::vector<bool>> territoire_adverse = adversaire.territoire(st->carte);
    int cout = 1;
    for (int emplacement_y = y - 1; emplacement_y <= y; emplacement_y++)
    {
        for (int emplacement_x = x - 1; emplacement_x <= x; emplacement_x++)
        {
            if (!carte.emplacement_valide(emplacement_x, emplacement_y))
                continue;
            if (territoire_adverse[emplacement_y][emplacement_x])
                cout = 2;
        }
    }
    joueur.points_action -= cout;

    enum type_case actuel = st->carte.get_case(x, y);
    st->carte.set_case(x, y, rotation_case(actuel));
    st->ajoute_historique({ACTION_TOURNER_CASE, pos_, pos_, -1});
}
