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