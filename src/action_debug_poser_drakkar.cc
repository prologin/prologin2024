// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "actions.hh"

int ActionDebugPoserDrakkar::check(const GameState& st) const
{
    if (!st.carte.case_valide(pos_.ligne, pos_.colonne))
        return POSITION_INVALIDE;

    if (!(drakkar_ == PAS_DE_DRAKKAR || drakkar_ == DRAKKAR_BLEU ||
        drakkar_ == DRAKKAR_JAUNE || drakkar_ == DRAKKAR_ROUGE))
        return CASE_INVALIDE;

    return OK;
}

void ActionDebugPoserDrakkar::apply_on(GameState* st) const
{
    st->pose_drakkar(pos_, drakkar_);
}
