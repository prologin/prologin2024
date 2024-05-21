#include "test-helpers.hh"

TEST_F(ApiTestColibri, TestGameStateCalculScoreSansAigle)
{
    auto& st = api->game_state();
    int score = st.calcul_score(0, 0);
    ASSERT_EQ(score, 3);
    score = st.calcul_score(1, 0);
    ASSERT_EQ(score, 2);
}

TEST_F(ApiTestColibri, TestGameStateCalculScoreBonAigle)
{
    auto& st = api->game_state();
    Aigle a(1, {0, 0}, EFFET_FEU, 3, 0);
    st.joueurs[0].aigles.push_back(a);
    int score = st.calcul_score(0, 0);
    ASSERT_EQ(score, 9);

    Aigle b(2, {1, 0}, EFFET_FEU, -3, 0);
    st.joueurs[1].aigles.push_back(b);
    score = st.calcul_score(1, 0);
    ASSERT_EQ(score, -6);

}

TEST_F(ApiTestColibri, TestGameStateCalculScoreBonAigleMauvaisEndroit)
{
    auto& st = api->game_state();
    Aigle a(1, {1, 0}, EFFET_FEU, 3, 0);
    st.joueurs[0].aigles.push_back(a);
    int score =st.calcul_score(0, 0);
    ASSERT_EQ(score, 3);
}

TEST_F(ApiTestColibri, TestGameStateCalculScoreMauvaisAigle)
{
    auto& st = api->game_state();
    Aigle a(1, {0, 0}, EFFET_VIE, 3, 0);
    st.joueurs[0].aigles.push_back(a);
    int score =st.calcul_score(0, 0);
    ASSERT_EQ(score, 3);
}

// colibri
// ..OO
// ....
// OOO.

TEST_F(ApiTestColibri, TestGameStateTerritoireDebut)
{
    auto& st = api->game_state();
    EXPECT_EQ(st.joueurs[0].territoire(st.carte)[0][0], false);
    EXPECT_EQ(st.joueurs[0].territoire(st.carte)[0][2], false);
    EXPECT_EQ(st.joueurs[0].territoire(st.carte)[0][3], false);
    EXPECT_EQ(st.joueurs[0].territoire(st.carte)[2][0], true);
    EXPECT_EQ(st.joueurs[0].territoire(st.carte)[2][1], true);
    EXPECT_EQ(st.joueurs[0].territoire(st.carte)[2][2], true);
    EXPECT_EQ(st.joueurs[0].territoire(st.carte)[2][3], false);

    EXPECT_EQ(st.joueurs[1].territoire(st.carte)[0][0], false);
    EXPECT_EQ(st.joueurs[1].territoire(st.carte)[0][1], false);
    EXPECT_EQ(st.joueurs[1].territoire(st.carte)[0][2], true);
    EXPECT_EQ(st.joueurs[1].territoire(st.carte)[0][3], true);
    EXPECT_EQ(st.joueurs[1].territoire(st.carte)[2][1], false);
    EXPECT_EQ(st.joueurs[1].territoire(st.carte)[2][2], false);
    EXPECT_EQ(st.joueurs[1].territoire(st.carte)[2][3], false);
}

TEST_F(ApiTestColibri, TestGameStateCaptureVillageDebut)
{
    auto& st = api->game_state();
    ASSERT_EQ(st.joueurs[0].villages.size(), 2);
    EXPECT_EQ(st.joueurs[0].villages[0].colonne, 0);
    EXPECT_EQ(st.joueurs[0].villages[0].ligne, 3);
    EXPECT_EQ(st.joueurs[0].villages[1].colonne, 3);
    EXPECT_EQ(st.joueurs[0].villages[1].ligne, 2);
}

TEST_F(ApiTestColibri, TestGameStateCaptureAigleDebut)
{
    auto& st = api->game_state();
    for (int x = 0; x < st.carte.get_dimension().first; x++)
    {
        for (int y = 0; y < st.carte.get_dimension().second; y++)
        {
            st.carte.set_case(x, y, NORD_OUEST);
        }
    }
    // colibri
    // 22222
    // 22222
    // 22212
    // 24444
    // ....
    // ..O.
    // OOO.
    st.carte.set_case(1, 3, SUD_EST);
    st.carte.set_case(2, 3, SUD_EST);
    st.carte.set_case(3, 3, SUD_EST);
    st.carte.set_case(4, 3, SUD_EST);
    st.carte.set_case(3, 2, NORD_EST);
    st.tour_suivant();
    ASSERT_EQ(st.joueurs[0].aigles.size(), 1);
    ASSERT_EQ(st.joueurs[0].aigles[0].identifiant, 0);
    ASSERT_EQ(st.joueurs[0].aigles[0].effet, EFFET_GEL);
    ASSERT_EQ(st.joueurs[1].aigles.size(), 0);

}