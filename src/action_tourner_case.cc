// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "actions.hh"

int ActionTournerCase::check(const GameState& st) const
{
    if (!st.init)
        return HORS_TOUR;

    Carte& carte = st.carte;
    int x, y;
    std::tie(x, y) = pos_
    if (!carte.case_valide(x, y))
        return POSITION_INVALIDE;

    Joueur& joueur = st.joueurs[player_id_];
    Joueur& adversaire = st.joueurs[player_id_ ^ 1];
    std::vector<std::vector<bool>> territoire_adverse = adversaire.territoire();
    int cout = territoire_adverse[y][x] ? 2 : 1;
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
    int x = pos_.colonne;
    int y = pos_.lignt;

    Joueur& joueur = st.joueurs[player_id_];
    Joueur& adversaire = st.joueurs[player_id_ ^ 1];
    std::vector<std::vector<bool>> territoire_adverse = adversaire.territoire();
    int cout = territoire_adverse[y][x] ? 2 : 1;
    joueur.points_action -= cout;

    enum type_case actuel = st.carte.get_case(x, y);
    st.carte.set_case(x, y, rotation_case(actuel));
}
