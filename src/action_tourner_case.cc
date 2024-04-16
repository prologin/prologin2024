// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "actions.hh"

int ActionTournerCase::check(const GameState& st) const
{
    Carte carte = st.carte;
    if (!carte.case_valide(pos_.colonne, pos_.ligne))
        return POSITION_INVALIDE;
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
    int x = pos_.colonne;
    int y = pos_.lignt;
    enum type_case actuel = st.carte.get_case(x, y);
    st.carte.set_case(x, y, rotation_case(actuel));
}
