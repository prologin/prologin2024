#include "test-helpers.hh"

TEST_F(ApiTestColibri, ActionTournerCaseAllier) {
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

TEST_F(ApiTestColibri, ActionTournerCaseBloqueeAllier) {
  auto& st = api->game_state();
  type_case caze = st.carte.get_case(0, 0);
  EXPECT_EQ(caze, SUD_EST);
  Aigle a(10, {0, 0}, EFFET_GEL, 0, 0);
  st.joueurs[0].aigles.push_back(a);
  position pos = {0, 0};
  EXPECT_EQ(api->tourner_case(pos), CASE_BLOQUEE);
  pos = {1, 0};
  EXPECT_EQ(api->tourner_case(pos), CASE_BLOQUEE);
  pos = {1, 1};
  EXPECT_EQ(api->tourner_case(pos), CASE_BLOQUEE);
  pos = {0, 0};
  EXPECT_EQ(api->tourner_case(pos), CASE_BLOQUEE);
  pos = {1, 2};
  EXPECT_EQ(api->tourner_case(pos), OK);
  pos = {1, 2};
  EXPECT_EQ(api->tourner_case(pos), OK);
}

TEST_F(ApiTestColibri, ActionTournerCaseBloqueeEnnemi) {
  auto& st = api->game_state();
  type_case caze = st.carte.get_case(0, 0);
  EXPECT_EQ(caze, SUD_EST);
  Aigle a(10, {0, 0}, EFFET_GEL, 0, 0);
  st.joueurs[1].aigles.push_back(a);
  position pos = {0, 0};
  EXPECT_EQ(api->tourner_case(pos), CASE_BLOQUEE);
  pos = {1, 0};
  EXPECT_EQ(api->tourner_case(pos), CASE_BLOQUEE);
  pos = {1, 1};
  EXPECT_EQ(api->tourner_case(pos), CASE_BLOQUEE);
  pos = {0, 0};
  EXPECT_EQ(api->tourner_case(pos), CASE_BLOQUEE);
  pos = {1, 2};
  EXPECT_EQ(api->tourner_case(pos), OK);
  pos = {1, 2};
  EXPECT_EQ(api->tourner_case(pos), OK);
}

TEST_F(ApiTestColibri, ActionTournerCaseBloqueeAigleSauvage) {
  auto& st = api->game_state();
  type_case caze = st.carte.get_case(0, 0);
  EXPECT_EQ(caze, SUD_EST);
  Aigle a(10, {0, 0}, EFFET_FEU, 0, 0);
  st.aigles_sauvages.push_back(a);
  position pos = {0, 0};
  EXPECT_EQ(api->tourner_case(pos), OK);
  pos = {1, 0};
  EXPECT_EQ(api->tourner_case(pos), OK);
}

TEST_F(ApiTestColibri, ActionTournerCaseEnnemi) {
  const auto& st = api->game_state();
  position pos = {3, 0};
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

TEST_F(ApiTestColibri, ActionTournerCaseVillage) {
  const auto& st = api->game_state();
  position pos = {0, 3};
  type_case caze = st.carte.get_case(0, 3);
  EXPECT_EQ(caze, VILLAGE);
  EXPECT_EQ(api->tourner_case(pos), ROTATION_VILLAGE);
  caze = st.carte.get_case(0, 3);
  EXPECT_EQ(caze, VILLAGE);
  EXPECT_EQ(api->tourner_case(pos), ROTATION_VILLAGE);
  caze = st.carte.get_case(0, 3);
  EXPECT_EQ(caze, VILLAGE);
  position pos2 = {3, 0};
  EXPECT_EQ(api->tourner_case(pos2), OK);
  caze = st.carte.get_case(3, 0);
  EXPECT_EQ(caze, SUD_OUEST);
  EXPECT_EQ(api->tourner_case(pos2), PLUS_DE_PA);
  caze = st.carte.get_case(3, 0);
  EXPECT_EQ(caze, SUD_OUEST);
}
