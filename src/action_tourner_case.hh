// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2024 Association Prologin <association@prologin.org>

#pragma once

#include <rules/action.hh>

#include "actions.hh"
#include "constant.hh"
#include "game_state.hh"

class ActionTournerCase : public rules::Action<GameState>
{
public:
    ActionTournerCase(position pos, int player_id)
        : pos_(pos)
        , player_id_(player_id)
    {
    }
    ActionTournerCase() = default; // for register_action()

    [[nodiscard]] int check(const GameState& st) const override;
    void apply_on(GameState* st) const override;

    void handle_buffer(utils::Buffer& buf) override
    {
        buf.handle(pos_);
        buf.handle(player_id_);
    }

    [[nodiscard]] uint32_t player_id() const override { return player_id_; };
    [[nodiscard]] uint32_t id() const override { return ID_ACTION_TOURNER_CASE; }

private:
    position pos_;
    int player_id_;
};

enum type_case rotation_case(enum type_case actuel);
bool case_bloquee(const GameState& st, int x, int y);
