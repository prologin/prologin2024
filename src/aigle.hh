// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2024 Association Prologin <association@prologin.org>

#pragma once

#include "constant.hh"

struct Aigle
{
    Aigle() = default;
    Aigle(int identifiant, position pos, effet_aigle effet, int puissance, int tour_eclosion);
    [[nodiscard]] bool case_dans_rayon(int x, int y) const;

    int identifiant;
    position pos;
    effet_aigle effet;
    int puissance;
    int tour_eclosion;
};
