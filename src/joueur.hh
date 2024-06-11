// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2024 Association Prologin <association@prologin.org>

#pragma once

#include <vector>

#include "aigle.hh"
#include "carte.hh"

struct Joueur {
  Joueur() = default;
  Joueur(int score, int points_action, std::vector<Aigle> aigles,
         std::vector<position> villages);
  int score;
  int score_tour;
  int points_action;
  std::vector<Aigle> aigles;
  std::vector<position> villages;

  [[nodiscard]] std::vector<std::vector<bool>> territoire(
      const Carte& carte) const;
  [[nodiscard]] Aigle* trouve_aigle(int aigle_id);
  [[nodiscard]] const Aigle* trouve_aigle(int aigle_id) const;
};
