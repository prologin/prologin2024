#include "test-helpers.hh"

TEST_F(ApiTestColibri, ActionActiverAiglePasAigle)
{
    const auto& st = api->game_state();
    ASSERT_EQ(api->activer_aigle(0), AIGLE_INVALIDE);
    ASSERT_EQ(st.aigles_sauvages[0].pos.colonne, 2);
    ASSERT_EQ(st.aigles_sauvages[0].pos.ligne, 1);
}

//EFFET_METEORE

TEST_F(ApiTestColibri, ActionActiverAigleRazDeMareeCorrect0)
{
    auto& st = api->game_state();
    Aigle a(10, {1, 1}, EFFET_METEORE, 0, 0);
    type_case caze = st.carte.get_case(1, 1);
    ASSERT_EQ(caze, NORD_OUEST);
    st.joueurs[0].aigles.push_back(a);
    ASSERT_EQ(api->activer_aigle(10), OK);
    caze = st.carte.get_case(0, 0);
    EXPECT_EQ(caze, SUD_EST);
    caze = st.carte.get_case(1, 0);
    EXPECT_EQ(caze, SUD_OUEST);
    caze = st.carte.get_case(2, 0);
    EXPECT_EQ(caze, SUD_OUEST);
    caze = st.carte.get_case(3, 0);
    EXPECT_EQ(caze, NORD_OUEST);
    caze = st.carte.get_case(0, 1);
    EXPECT_EQ(caze, SUD_EST);
    caze = st.carte.get_case(1, 1);
    EXPECT_EQ(caze, SUD_EST);
    caze = st.carte.get_case(2, 1);
    EXPECT_EQ(caze, NORD_OUEST);
    caze = st.carte.get_case(3, 1);
    EXPECT_EQ(caze, SUD_EST);
    caze = st.carte.get_case(0, 2);
    EXPECT_EQ(caze, SUD_OUEST);
    caze = st.carte.get_case(1, 2);
    EXPECT_EQ(caze, SUD_OUEST);
    caze = st.carte.get_case(2, 2);
    EXPECT_EQ(caze, SUD_EST);
    caze = st.carte.get_case(3, 2);
    EXPECT_EQ(caze, VILLAGE);
    caze = st.carte.get_case(0, 3);
    EXPECT_EQ(caze, VILLAGE);
    caze = st.carte.get_case(1, 3);
    EXPECT_EQ(caze, SUD_EST);
    caze = st.carte.get_case(2, 3);
    EXPECT_EQ(caze, SUD_OUEST);
    caze = st.carte.get_case(3, 3);
    EXPECT_EQ(caze, SUD_EST);
}

TEST_F(ApiTestColibri, ActionActiverAigleRazDeMareeCorrect1)
{
    auto& st = api->game_state();
    Aigle a(10, {1, 1}, EFFET_METEORE, 1, 0);
    type_case caze = st.carte.get_case(1, 1);
    ASSERT_EQ(caze, NORD_OUEST);
    st.joueurs[0].aigles.push_back(a);
    ASSERT_EQ(api->activer_aigle(10), OK);
    caze = st.carte.get_case(1, 1);
    EXPECT_EQ(caze, SUD_EST);
    caze = st.carte.get_case(0, 0);
    EXPECT_EQ(caze, NORD_OUEST);
    caze = st.carte.get_case(3, 0);
    EXPECT_EQ(caze, SUD_EST);
    caze = st.carte.get_case(4, 0);
    EXPECT_EQ(caze, VILLAGE);
    caze = st.carte.get_case(3, 1);
    EXPECT_EQ(caze, NORD_OUEST);
    caze = st.carte.get_case(4, 1);
    EXPECT_EQ(caze, SUD_OUEST);
    caze = st.carte.get_case(0, 3);
    EXPECT_EQ(caze, VILLAGE);
    caze = st.carte.get_case(3, 3);
    EXPECT_EQ(caze, NORD_OUEST);
    caze = st.carte.get_case(4, 3);
    EXPECT_EQ(caze, SUD_OUEST);
}

TEST_F(ApiTestColibri, ActionActiverAigleRazDeMareeCorrectVillage)
{
    auto& st = api->game_state();
    Aigle a(10, {0, 3}, EFFET_METEORE, 0, 0);
    type_case caze = st.carte.get_case(0, 3);
    ASSERT_EQ(caze, VILLAGE);
    st.joueurs[0].aigles.push_back(a);
    ASSERT_EQ(api->activer_aigle(10), OK);
    caze = st.carte.get_case(0, 3);
    EXPECT_EQ(caze, VILLAGE);
}

TEST_F(ApiTestColibri, ActionActiverAigleRazDeMareeCorrectBord)
{
    auto& st = api->game_state();
    Aigle a(10, {0, 0}, EFFET_METEORE, 1, 0);
    type_case caze = st.carte.get_case(0, 0);
    ASSERT_EQ(caze, SUD_EST);
    st.joueurs[0].aigles.push_back(a);
    ASSERT_EQ(api->activer_aigle(10), OK);
    caze = st.carte.get_case(0, 0);
    EXPECT_EQ(caze, NORD_OUEST);
    caze = st.carte.get_case(1, 0);
    EXPECT_EQ(caze, NORD_EST);
    caze = st.carte.get_case(0, 1);
    EXPECT_EQ(caze, NORD_OUEST);
    caze = st.carte.get_case(1, 1);
    EXPECT_EQ(caze, SUD_EST);
    caze = st.carte.get_case(2, 1);
    EXPECT_EQ(caze, NORD_OUEST);
    caze = st.carte.get_case(3, 1);
    EXPECT_EQ(caze, SUD_EST);
}

TEST_F(ApiTestColibri, ActionActiverAigleRazDeMareeAigleBloquant)
{
    auto& st = api->game_state();
    Aigle a(10, {0, 0}, EFFET_METEORE, 1, 0);
    Aigle b(10, {0, 0}, EFFET_GEL, 0, 0);
    type_case caze = st.carte.get_case(0, 0);
    ASSERT_EQ(caze, SUD_EST);
    st.joueurs[0].aigles.push_back(a);
    st.joueurs[0].aigles.push_back(b);
    ASSERT_EQ(api->activer_aigle(10), OK);
    caze = st.carte.get_case(0, 0);
    EXPECT_EQ(caze, SUD_EST);
    caze = st.carte.get_case(1, 0);
    EXPECT_EQ(caze, SUD_OUEST);
    caze = st.carte.get_case(0, 1);
    EXPECT_EQ(caze, SUD_EST);
    caze = st.carte.get_case(1, 1);
    EXPECT_EQ(caze, NORD_OUEST);
    caze = st.carte.get_case(2, 1);
    EXPECT_EQ(caze, NORD_OUEST);
    caze = st.carte.get_case(1, 2);
    EXPECT_EQ(caze, SUD_OUEST);
    caze = st.carte.get_case(3, 1);
    EXPECT_EQ(caze, SUD_EST);
}

//EFFET_VIE

TEST_F(ApiTestColibri, ActionActiverAigleActionPositif1)
{
    auto& st = api->game_state();
    Aigle a(10, {0, 0}, EFFET_VIE, 1, 0);
    ASSERT_EQ(st.joueurs[0].points_action, 2);
    st.joueurs[0].aigles.push_back(a);
    ASSERT_EQ(api->activer_aigle(10), OK);
    ASSERT_EQ(st.joueurs[0].points_action, 3);
}

TEST_F(ApiTestColibri, ActionActiverAigleActionPositif3)
{
    auto& st = api->game_state();
    Aigle a(10, {0, 0}, EFFET_VIE, 3, 0);
    ASSERT_EQ(st.joueurs[0].points_action, 2);
    st.joueurs[0].aigles.push_back(a);
    ASSERT_EQ(api->activer_aigle(10), OK);
    ASSERT_EQ(st.joueurs[0].points_action, 5);
}

TEST_F(ApiTestColibri, ActionActiverAigleActionPositif_2)
{
    auto& st = api->game_state();
    Aigle a(10, {0, 0}, EFFET_VIE, -2, 0);
    ASSERT_EQ(st.joueurs[0].points_action, 2);
    st.joueurs[0].aigles.push_back(a);
    ASSERT_EQ(api->activer_aigle(10), OK);
    ASSERT_EQ(st.joueurs[0].points_action, 0);
}

TEST_F(ApiTestColibri, ActionActiverAigleActionPositif_5)
{
    auto& st = api->game_state();
    Aigle a(10, {0, 0}, EFFET_VIE, -5, 0);
    ASSERT_EQ(st.joueurs[0].points_action, 2);
    st.joueurs[0].aigles.push_back(a);
    ASSERT_EQ(api->activer_aigle(10), OK);
    ASSERT_EQ(st.joueurs[0].points_action, -3);
}

//EFFET_MORT


TEST_F(ApiTestColibri, ActionActiverEffrayerValideBonEndroit)
{
    auto& st = api->game_state();
    Aigle a(10, {2, 1}, EFFET_MORT, -5, 0);
    st.joueurs[0].aigles.push_back(a);
    Aigle b(11, {2, 1}, EFFET_METEORE, -5, 0);
    st.joueurs[0].aigles.push_back(b);
    Aigle c(12, {2, 1}, EFFET_FEU, -5, 0);
    st.joueurs[1].aigles.push_back(c);
    Aigle d(13, {2, 1}, EFFET_GEL, -5, 0);
    st.aigles_sauvages.push_back(d);
    Aigle f(14, {1, 1}, EFFET_GEL, -5, 0);
    st.aigles_sauvages.push_back(f);
    ASSERT_EQ(st.joueurs[0].aigles.size(), 2);
    ASSERT_EQ(st.joueurs[1].aigles.size(), 1);
    ASSERT_EQ(st.aigles_sauvages.size(), 3);
    ASSERT_EQ(st.aigles_sauvages[0].tour_eclosion, 42);
    ASSERT_EQ(api->activer_aigle(10), OK);
    EXPECT_EQ(st.joueurs[0].aigles.size(), 0);
    EXPECT_EQ(st.joueurs[1].aigles.size(), 0);
    ASSERT_EQ(st.aigles_sauvages.size(), 2);
    ASSERT_EQ(st.aigles_sauvages[0].identifiant, 0);
    ASSERT_EQ(st.aigles_sauvages[1].identifiant, 14);
}

TEST_F(ApiTestColibri, ActionActiverEffrayerValideOeuf)
{
    auto& st = api->game_state();
    Aigle a(10, {2, 1}, EFFET_MORT, -5, 0);
    st.joueurs[0].aigles.push_back(a);
    Aigle b(11, {2, 1}, EFFET_METEORE, -5, 10);
    st.aigles_sauvages.push_back(b);
    ASSERT_EQ(st.joueurs[0].aigles.size(), 1);
    ASSERT_EQ(st.joueurs[1].aigles.size(), 0);
    ASSERT_EQ(st.aigles_sauvages.size(), 2);
    ASSERT_EQ(st.aigles_sauvages[0].tour_eclosion, 42);
    ASSERT_EQ(api->activer_aigle(10), OK);
    EXPECT_EQ(st.joueurs[0].aigles.size(), 0);
    EXPECT_EQ(st.joueurs[1].aigles.size(), 0);
    ASSERT_EQ(st.aigles_sauvages.size(), 2);
    ASSERT_EQ(st.aigles_sauvages[0].identifiant, 0);
    ASSERT_EQ(st.aigles_sauvages[1].identifiant, 11);
}

TEST_F(ApiTestColibri, ActionActiverEffetFeu)
{
    auto& st = api->game_state();
    Aigle a(10, {2, 1}, EFFET_FEU, -5, 0);
    st.joueurs[0].aigles.push_back(a);

    ASSERT_EQ(api->activer_aigle(10), AIGLE_INVALIDE);
}