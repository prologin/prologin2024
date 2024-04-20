#include <fstream>
#include <gtest/gtest.h>
#include <rules/player.hh>

#include "../game_state.hh"

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
    ASSERT_EQ(carte.get_case(4, 0), VILLAGE);
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

TEST(CarteTest, json_test_get_gain)
{
    std::ifstream is("colibri.json");
    rules::Players players;
    GameState st(players, is);
    Carte carte = st.carte;
    
    // Valide
    ASSERT_EQ(carte.get_gain(0, 0), 3);
    ASSERT_EQ(carte.get_gain(0, 1), 0);
    ASSERT_EQ(carte.get_gain(0, 2), 10);
    ASSERT_EQ(carte.get_gain(2, 0), 4);
    ASSERT_EQ(carte.get_gain(2, 1), -4);
    ASSERT_EQ(carte.get_gain(2, 2), 2);
    
    // Invalide
    ASSERT_EQ(carte.get_gain(0, -1), 0);
    ASSERT_EQ(carte.get_gain(-1, -1), 0);
    ASSERT_EQ(carte.get_gain(-1, 0), 0);
    ASSERT_EQ(carte.get_gain(0, 3), 0);
    ASSERT_EQ(carte.get_gain(5, 0), 0);
}

TEST(CarteTest, json_test_case_valide)
{
    std::ifstream is("colibri.json");
    rules::Players players;
    GameState st(players, is);
    Carte carte = st.carte;
    
    // Valide
    for (int x = 0; x < 5; x++)
    {
        for (int y = 0; y < 4; y++)
        {
        ASSERT_EQ(carte.case_valide(x, y), true);
        }
    }
    
    // Invalide
    ASSERT_EQ(carte.case_valide(-1, 0), false);
    ASSERT_EQ(carte.case_valide(-1, -1), false);
    ASSERT_EQ(carte.case_valide(0, -1), false);
    ASSERT_EQ(carte.case_valide(5, 4), false);
    ASSERT_EQ(carte.case_valide(5, 0), false);
    ASSERT_EQ(carte.case_valide(0, 4), false);
}

TEST(CarteTest, json_test_emplacement_valide)
{
    std::ifstream is("colibri.json");
    rules::Players players;
    GameState st(players, is);
    Carte carte = st.carte;
    
    // Valide
    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y < 3; y++)
        {
        ASSERT_EQ(carte.emplacement_valide(x, y), true);
        }
    }
    
    // Invalide
    ASSERT_EQ(carte.emplacement_valide(-1, 0), false);
    ASSERT_EQ(carte.emplacement_valide(-1, -1), false);
    ASSERT_EQ(carte.emplacement_valide(0, -1), false);
    ASSERT_EQ(carte.emplacement_valide(4, 3), false);
    ASSERT_EQ(carte.emplacement_valide(4, 0), false);
    ASSERT_EQ(carte.emplacement_valide(0, 3), false);
}

TEST(CarteTest, json_test_ile_presente)
{
    std::ifstream is("colibri.json");
    rules::Players players;
    GameState st(players, is);
    Carte carte = st.carte;
    
    // Valide
    ASSERT_EQ(carte.ile_presente(2, 0), true);
    ASSERT_EQ(carte.ile_presente(3, 0), true);
    ASSERT_EQ(carte.ile_presente(0, 2), true);
    ASSERT_EQ(carte.ile_presente(1, 2), true);
    ASSERT_EQ(carte.ile_presente(2, 2), true);
    
    // Ivalide (pas ile)
    ASSERT_EQ(carte.ile_presente(0, 0), false);
    ASSERT_EQ(carte.ile_presente(1, 0), false);
    ASSERT_EQ(carte.ile_presente(2, 1), false);
    ASSERT_EQ(carte.ile_presente(3, 1), false);
    ASSERT_EQ(carte.ile_presente(0, 1), false);
    ASSERT_EQ(carte.ile_presente(1, 1), false);
    ASSERT_EQ(carte.ile_presente(2, 1), false);
    ASSERT_EQ(carte.ile_presente(3, 2), false);
    
    // Invalide (hors carte)
    ASSERT_EQ(carte.ile_presente(-1, 0), false);
    ASSERT_EQ(carte.ile_presente(1, -1), false);
    ASSERT_EQ(carte.ile_presente(4, 1), false);
    ASSERT_EQ(carte.ile_presente(3, 3), false);
}
