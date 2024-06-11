// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>
#include <vector>

#include "actions.hh"
#include "aigle.hh"
#include "joueur.hh"

int ActionDeplacerAigle::check(const GameState& st) const {
  if (!st.init) return HORS_TOUR;

  const auto& joueur = st.joueurs[player_id_];
  const auto* const aigle = joueur.trouve_aigle(id_);
  if (aigle == nullptr) return AIGLE_INVALIDE;

  // Aigle aiglantine = *aigle; #JesuisAiglantine
  if (!st.carte.emplacement_valide(destination_.colonne, destination_.ligne))
    return DESTINATION_INVALIDE;

  return OK;
}

void ActionDeplacerAigle::apply_on(GameState* st) const {
  auto* const aiglantine = st->joueurs[player_id_].trouve_aigle(id_);
  st->ajoute_historique(
      {ACTION_DEPLACER_AIGLE, aiglantine->pos, destination_, id_});
  aiglantine->pos = destination_;
}
