#include <fstream>
#include <gtest/gtest.h>
#include <rules/player.hh>

#include "../game_state.hh"

TEST(CarteTest, txt_test)
{
    std::ifstream is("colibri.json");
    rules::Players players;
    GameState st(players, is);
}