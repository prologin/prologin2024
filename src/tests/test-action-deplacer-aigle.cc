#include "test-helpers.hh"

#include "../game_state.hh"

TEST_F(ApiTestColibri, ActionDeplacerAiglePasAigle)
{
    const auto& st = api->game_state();
    ASSERT_EQ(api->deplacer_aigle(0, position{0, 0}), AIGLE_INVALIDE);
    ASSERT_EQ(st.aigles_sauvages[0].pos.colonne, 2);
    ASSERT_EQ(st.aigles_sauvages[0].pos.ligne, 1);
}

// TODO YAKA
