// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2024 Association Prologin <association@prologin.org>

#pragma once

#include <rules/action.hh>

#include "actions.hh"
#include "constant.hh"
#include "game_state.hh"

class ActionDeplacerAigle : public rules::Action<GameState>
{
public:
    ActionDeplacerAigle(int id, position destination, int player_id)
        : id_(id)
        , destination_(destination)
        , player_id_(player_id)
    {
    }
    ActionDeplacerAigle() = default; // for register_action()

    [[nodiscard]] int check(const GameState& st) const override;
    void apply_on(GameState* st) const override;

    void handle_buffer(utils::Buffer& buf) override
    {
        buf.handle(id_);
        buf.handle(destination_);
        buf.handle(player_id_);
    }

    [[nodiscard]] uint32_t player_id() const override { return player_id_; };
    [[nodiscard]] uint32_t id() const override { return ID_ACTION_DEPLACER_AIGLE; }

private:
    int id_;
    position destination_;
    int player_id_;
};

