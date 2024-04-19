#include <fstream>
#include <gtest/gtest.h>
#include <rules/player.hh>

#include "../game_state.hh"

#include <sstream>
#include <filesystem>

TEST(CarteTest, json_test_dimensions)
{
    std::ifstream is("colibri.json");
    rules::Players players;
    GameState st(players, is);
    Carte carte = st.carte;

    ASSERT_EQ(carte.get_dimension().first, 5);
    ASSERT_EQ(carte.get_dimension().second, 4);
}

TEST(CarteTest, json_test_cases)
{
    std::ifstream is("colibri.json");
    rules::Players players;
    GameState st(players, is);
    Carte carte = st.carte;

    // Tous les types de cases
    ASSERT_EQ(carte.get_case(0, 0), SUD_EST);
    ASSERT_EQ(carte.get_case(1, 0), SUD_OUEST);
    ASSERT_EQ(carte.get_case(3, 0), NORD_OUEST);
    ASSERT_EQ(carte.get_case(4, 0), VILLAGE_NEUTRE); //TODO modifier apres le changement dans le yaml
    ASSERT_EQ(carte.get_case(1, 2), NORD_EST);

    // Case bord
    ASSERT_EQ(carte.get_case(1, 3), SUD_EST);

    // Case coin
    ASSERT_EQ(carte.get_case(4, 3), SUD_OUEST);

    // Case invalide
    ASSERT_EQ(carte.get_case(0, -1), CASE_INVALIDE);
    ASSERT_EQ(carte.get_case(-42, -1), CASE_INVALIDE);
    ASSERT_EQ(carte.get_case(-1, 0), CASE_INVALIDE);
    ASSERT_EQ(carte.get_case(0, 4), CASE_INVALIDE);
    ASSERT_EQ(carte.get_case(5, 0), CASE_INVALIDE);
    ASSERT_EQ(carte.get_case(5, 4), CASE_INVALIDE);
}

TEST(CarteTest, json_test_set_case)
{
    std::ifstream is("colibri.json");
    rules::Players players;
    GameState st(players, is);
    Carte carte = st.carte;

    // Valide
    carte.set_case(0, 0, NORD_OUEST);
    ASSERT_EQ(carte.get_case(0, 0), NORD_OUEST);
    carte.set_case(1, 1, NORD_OUEST);
    ASSERT_EQ(carte.get_case(1, 1), NORD_OUEST);
    carte.set_case(2, 2, SUD_EST);
    ASSERT_EQ(carte.get_case(2, 2), SUD_EST);
    for (int x = 0; x < 5; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            carte.set_case(x, y, NORD_EST);
        }
    }
    for (int x = 0; x < 5; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            ASSERT_EQ(carte.get_case(x, y), NORD_EST);
        }
    }

    // Invalide
    carte.set_case(-1, 0, NORD_OUEST);
    carte.set_case(-1, -2, NORD_OUEST);
    carte.set_case(0, -2, NORD_OUEST);
    carte.set_case(0, 4, NORD_OUEST);
    carte.set_case(5, 4, NORD_OUEST);
    carte.set_case(5, 4, NORD_OUEST);
    for (int x = 0; x < 5; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            carte.set_case(x, y, NORD_EST);
        }
    }
    for (int x = 0; x < 5; x++)
    {
        for (int y = 0; y < 4; y++)
        {
        ASSERT_EQ(carte.get_case(x, y), NORD_EST);
        }
    }
}