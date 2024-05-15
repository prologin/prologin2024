// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#pragma once

#include <rules/action.hh>

#include "actions.hh"
#include "game_state.hh"

class ActionActiverAigle : public rules::Action<GameState>
{
public:
    ActionActiverAigle(int id, int player_id)
        : id_(id)
        , player_id_(player_id)
    {
    }
    ActionActiverAigle() = default; // for register_action()

    [[nodiscard]] int check(const GameState& st) const override;
    void apply_on(GameState* st) const override;

    void handle_buffer(utils::Buffer& buf) override
    {
        buf.handle(id_);
        buf.handle(player_id_);
    }

    [[nodiscard]] uint32_t player_id() const override { return player_id_; };
    [[nodiscard]] uint32_t id() const override { return ID_ACTION_ACTIVER_AIGLE; }

private:
    int id_;
    int player_id_;
};

