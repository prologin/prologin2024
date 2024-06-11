// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2024 Association Prologin <association@prologin.org>

#pragma once

#include <rules/action.hh>

#include "actions.hh"
#include "constant.hh"
#include "game_state.hh"

class ActionDebugPoserDrakkar : public rules::Action<GameState> {
 public:
  ActionDebugPoserDrakkar(position pos, drakkar_debug drakkar, int player_id)
      : pos_(pos), drakkar_(drakkar), player_id_(player_id) {}
  ActionDebugPoserDrakkar() = default;  // for register_action()

  [[nodiscard]] int check(const GameState& st) const override;
  void apply_on(GameState* st) const override;

  void handle_buffer(utils::Buffer& buf) override {
    buf.handle(pos_);
    buf.handle(drakkar_);
    buf.handle(player_id_);
  }

  [[nodiscard]] uint32_t player_id() const override { return player_id_; };
  [[nodiscard]] uint32_t id() const override {
    return ID_ACTION_DEBUG_POSER_DRAKKAR;
  }

 private:
  position pos_;
  drakkar_debug drakkar_;
  int player_id_;
};
