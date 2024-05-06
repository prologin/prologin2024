#include "test-helpers.hh"

TEST_F(ApiTestColibri, ActionActiverCaseAllier)
{
    const auto& st = api->game_state();
    position pos = {0, 2};
    type_case caze = st.carte.get_case(0, 2);
    EXPECT_EQ(caze, SUD_OUEST);
    EXPECT_EQ(api->tourner_case(pos), OK);
    caze = st.carte.get_case(0, 2);
    EXPECT_EQ(caze, SUD_EST);
    EXPECT_EQ(api->tourner_case(pos), OK);
    caze = st.carte.get_case(0, 2);
    EXPECT_EQ(caze, NORD_EST);
    EXPECT_EQ(api->tourner_case(pos), PLUS_DE_PA);
    caze = st.carte.get_case(0, 2);
    EXPECT_EQ(caze, NORD_EST);
    EXPECT_EQ(api->tourner_case(pos), PLUS_DE_PA);
    caze = st.carte.get_case(0, 2);
    EXPECT_EQ(caze, NORD_EST);
    EXPECT_EQ(api->tourner_case(pos), PLUS_DE_PA);
    caze = st.carte.get_case(0, 2);
    EXPECT_EQ(caze, NORD_EST);
}


TEST_F(ApiTestColibri, ActionActiverCaseEnnemi)
{
    const auto& st = api->game_state();
    position pos = {0, 2};
    type_case caze = st.carte.get_case(3, 0);
    EXPECT_EQ(caze, NORD_OUEST);
    EXPECT_EQ(api->tourner_case(pos), OK);
    caze = st.carte.get_case(3, 0);
    EXPECT_EQ(caze, SUD_OUEST);
    EXPECT_EQ(api->tourner_case(pos), PLUS_DE_PA);
    caze = st.carte.get_case(3, 0);
    EXPECT_EQ(caze, SUD_OUEST);
    EXPECT_EQ(api->tourner_case(pos), PLUS_DE_PA);
    caze = st.carte.get_case(3, 0);
    EXPECT_EQ(caze, SUD_OUEST);
    EXPECT_EQ(api->tourner_case(pos), PLUS_DE_PA);
    caze = st.carte.get_case(3, 0);
    EXPECT_EQ(caze, SUD_OUEST);
}

TEST_F(ApiTestColibri, ActionActiverCaseVillage)
{
    const auto& st = api->game_state();
    position pos = {0, 2};
    type_case caze = st.carte.get_case(0, 3);
    EXPECT_EQ(caze, VILLAGE);
    EXPECT_EQ(api->tourner_case(pos), OK);
    caze = st.carte.get_case(0, 3);
    EXPECT_EQ(caze, VILLAGE);
    EXPECT_EQ(api->tourner_case(pos), OK);
    caze = st.carte.get_case(0, 3);
    EXPECT_EQ(caze, VILLAGE);
    EXPECT_EQ(api->tourner_case(pos), PLUS_DE_PA);
    caze = st.carte.get_case(0, 3);
    EXPECT_EQ(caze, VILLAGE);
}