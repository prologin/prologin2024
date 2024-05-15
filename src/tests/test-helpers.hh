#pragma once

#include <fstream>
#include <gtest/gtest.h>
#include <rules/player.hh>

#include "../api.hh"
#include "../game_state.hh"
#include "../rules.hh"

namespace
{
    template <typename ...Ts>
    rules::Players make_players(Ts &&...ids)
    {
        rules::Players players;
        (
         players.add(std::make_shared<rules::Player>(ids, rules::PLAYER)),
         ...
         );
        return players;
    }

} // namespace

class ApiTestColibri : public ::testing::Test
{
public:
protected:
    constexpr static int PLAYER_ID_1 = 0;
    constexpr static int PLAYER_ID_2 = 1;

     void SetUp() override
    {
        utils::Logger::get().level() = utils::Logger::DEBUG_LEVEL;
        const auto players = make_players(PLAYER_ID_1, PLAYER_ID_2);
        std::ifstream is("colibri.json");
        auto st = std::make_unique<GameState>(players, is);
        st->init = true;
        st->debute_tour(PLAYER_ID_1);
        api = std::make_unique<Api>(std::move(st), players[0]);
    }

    std::unique_ptr<Api> api;
};

class ApiTestCoq : public ::testing::Test
{
public:
protected:
    constexpr static int PLAYER_ID_1 = 0;
    constexpr static int PLAYER_ID_2 = 1;

     void SetUp() override
    {
        utils::Logger::get().level() = utils::Logger::DEBUG_LEVEL;
        const auto players = make_players(PLAYER_ID_1, PLAYER_ID_2);
        std::ifstream is("coq.json");
        auto st = std::make_unique<GameState>(players, is);
        st->init = true;
        st->debute_tour(PLAYER_ID_1);
        api = std::make_unique<Api>(std::move(st), players[0]);
    }

    std::unique_ptr<Api> api;
};