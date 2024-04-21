#include "test-helpers.hh"

TEST_F(ApiTestColibri, ActionActiverAiglePasAigle)
{
    const auto& st = api->game_state();
    ASSERT_EQ(api->activer_aigle(0), AIGLE_INVALIDE);
    ASSERT_EQ(st.aigles_sauvages[0].pos.colonne, 2);
    ASSERT_EQ(st.aigles_sauvages[0].pos.ligne, 1);
}