#include <fstream>
#include <gtest/gtest.h>
#include <rules/player.hh>

#include "../game_state.hh"

#include <sstream>
#include <filesystem>

TEST(CarteTest, txt_test)
{
    std::ifstream is("colibri.json");
std::cout << std::filesystem::current_path() << std::endl;
    rules::Players players;
    GameState st(players, is);
}