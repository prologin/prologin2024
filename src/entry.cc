// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012 Association Prologin <association@prologin.org>

#include <cstdlib>
#include <rules/client-messenger.hh>
#include <rules/config.hh>
#include <rules/replay-messenger.hh>
#include <rules/server-messenger.hh>
#include <utility>
#include <utils/log.hh>

#include "rules.hh"

// Forward decls
namespace rules {
struct Options;
}

static Rules* rules_;

extern "C" {

void rules_config(rules::Config* cfg) {
  cfg->name = "prologin2024";
  cfg->player_count = 2;
}

void rules_init(const rules::Options& opt) {
  utils::Logger::get().level() = (utils::Logger::DisplayLevel)opt.verbose;
  rules_ = new Rules(opt);
}

void rules_result() { delete rules_; }

void player_loop(rules::ClientMessenger_sptr msgr) {
  rules_->player_loop(std::move(msgr));
}

void replay_loop(rules::ReplayMessenger_sptr msgr) {
  rules_->replay_loop(std::move(msgr));
}

void server_loop(rules::ServerMessenger_sptr msgr) {
  rules_->server_loop(std::move(msgr));
}

void spectator_loop(rules::ClientMessenger_sptr msgr) {
  rules_->spectator_loop(std::move(msgr));
}

}  // extern "C"
