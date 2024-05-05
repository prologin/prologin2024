#include "test-helpers.hh"

TEST_F(ApiTestColibri, ActionActiverAiglePasAigle)
{
    const auto& st = api->game_state();
    ASSERT_EQ(api->activer_aigle(0), AIGLE_INVALIDE);
    ASSERT_EQ(st.aigles_sauvages[0].pos.colonne, 2);
    ASSERT_EQ(st.aigles_sauvages[0].pos.ligne, 1);
}

//EFFET_RAZ_DE_MAREE

TEST_F(ApiTestColibri, ActionActiverAigleRazDeMareeCorrect0)
{
    auto& st = api->game_state();
    Aigle a(10, {1, 1}, EFFET_RAZ_DE_MAREE, 0, 0);
    type_case caze = st.carte.get_case(1, 1);
    ASSERT_EQ(caze, NORD_OUEST);
    st.joueurs[0].aigles.push_back(a);
    ASSERT_EQ(api->activer_aigle(10), OK);
    caze = st.carte.get_case(1, 1);
    EXPECT_EQ(caze, SUD_EST);
    caze = st.carte.get_case(0, 0);
    EXPECT_EQ(caze, SUD_EST);
    caze = st.carte.get_case(0, 1);
    EXPECT_EQ(caze, SUD_EST);
    caze = st.carte.get_case(0, 2);
    EXPECT_EQ(caze, SUD_OUEST);
    caze = st.carte.get_case(1, 0);
    EXPECT_EQ(caze, SUD_OUEST);
    caze = st.carte.get_case(1, 2);
    EXPECT_EQ(caze, NORD_EST);
    caze = st.carte.get_case(2, 0);
    EXPECT_EQ(caze, SUD_OUEST);
    caze = st.carte.get_case(2, 1);
    EXPECT_EQ(caze, SUD_EST);
    caze = st.carte.get_case(2, 2);
    EXPECT_EQ(caze, NORD_OUEST);
}

TEST_F(ApiTestColibri, ActionActiverAigleRazDeMareeCorrect1)
{
    auto& st = api->game_state();
    Aigle a(10, {1, 1}, EFFET_RAZ_DE_MAREE, 1, 0);
    type_case caze = st.carte.get_case(1, 1);
    ASSERT_EQ(caze, NORD_OUEST);
    st.joueurs[0].aigles.push_back(a);
    ASSERT_EQ(api->activer_aigle(10), OK);
    caze = st.carte.get_case(1, 1);
    EXPECT_EQ(caze, SUD_EST);
    caze = st.carte.get_case(0, 0);
    EXPECT_EQ(caze, NORD_OUEST);
    caze = st.carte.get_case(0, 1);
    EXPECT_EQ(caze, NORD_OUEST);
    caze = st.carte.get_case(0, 2);
    EXPECT_EQ(caze, NORD_EST);
    caze = st.carte.get_case(1, 0);
    EXPECT_EQ(caze, NORD_EST);
    caze = st.carte.get_case(1, 2);
    EXPECT_EQ(caze, SUD_OUEST);
    caze = st.carte.get_case(2, 0);
    EXPECT_EQ(caze, NORD_EST);
    caze = st.carte.get_case(2, 1);
    EXPECT_EQ(caze, NORD_OUEST);
    caze = st.carte.get_case(2, 2);
    EXPECT_EQ(caze, SUD_EST);
}

TEST_F(ApiTestColibri, ActionActiverAigleRazDeMareeCorrectVillage)
{
    auto& st = api->game_state();
    Aigle a(10, {0, 3}, EFFET_RAZ_DE_MAREE, 0, 0);
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
    Aigle a(10, {0, 0}, EFFET_RAZ_DE_MAREE, 1, 0);
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
}

//EFFET_ACTIONS



//EFFET_EFFRAYER

//EFFET_MULTIPLICATIF

//EFFET_BLOQUEUR

//TODO YAKA