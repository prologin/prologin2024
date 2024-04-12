#include <fstream>
#include <gtest/gtest.h>
#include <rules/player.hh>

#include "../carte.hh"
#include "../game_state.hh"

TEST(CarteTest, txt_test)
{
    std::ifstream is("exemple.json");
    rules::Players players;
    GameState st(players, is);
    Carte carte = st.carte;
}