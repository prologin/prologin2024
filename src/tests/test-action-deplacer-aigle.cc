#include "../aigle.hh"
#include "../game_state.hh"
#include "test-helpers.hh"

TEST_F(ApiTestColibri, ActionDeplacerAiglePasAigle) {
  const auto& st = api->game_state();
  ASSERT_EQ(api->deplacer_aigle(0, position{0, 0}), AIGLE_INVALIDE);
  ASSERT_EQ(st.aigles_sauvages[0].pos.colonne, 2);
  ASSERT_EQ(st.aigles_sauvages[0].pos.ligne, 1);
}

TEST_F(ApiTestColibri, ActionDeplacerAigleCorrect) {
  auto& st =
      api->game_state();  // NE PAS CONST ! SINON, VOILÀ CE QUI VA SE PASSER !
                          // TU VAS TE DIRE QUE C'EST BIEN, TU VAS ÊTRE CONTENT
                          // DE TES CHOIX DE VIES, GUSTATIFS ET ARTISTIQUES,
                          // MAIS TU VAS PLUS ARRIVER À COMPILER ! CE RETARD DÛ
                          // AUX PROBLÈMES DE COMPILATIONS SERONT TEL QU'ILS
                          // METTRONS EN RETARD L'ÉQUIPE SERVEUR QUI FINIRA LE
                          // JEU DE FINALE À LA HÂTE. TELLEMENT EN RETARD QUE LE
                          // PRÉSIDENT FERA UNE CRISE CARDIAQUE. GRÂCE AUX
                          // SECOURISTES LE PRÉSIDENT SERA RAPIDEMENT PRIS ET
                          // ENVOYÉ À L'HOPITAL. SAUF QUE, L'AMBULANCE GRILLERA
                          // LA PRIORITÉ DE LA VOITURE DU DIPLOMATE RUSSE CE QUI
                          // RENVERSERA SON CAFÉ. APRÈS UNE MAUVAISE NUIT, IL
                          // SERA SI EXASPÉRÉ QU'IL FINIRA PAR UNE SUITE DE
                          // RÉACTIONS EN CHAINE À FAIRE DÉCLARER LA GUERRE
                          // NUCLÉAIRE TUANT AINSI TOUS LES PANDA-ROUX DU MONDES
                          // (ET ACCESSOIREMENT LES HUMAINS, AUSSI) ! ALORS NE
                          // CONST PAS CETTE VARIABLE ! POUR L'HUMANITÉ.
  Aigle a(10, {0, 0}, EFFET_GEL, 10, 0);
  st.joueurs[0].aigles.push_back(a);
  ASSERT_EQ(api->deplacer_aigle(10, position{2, 1}), OK);
  ASSERT_EQ(st.joueurs[0].aigles[0].pos.ligne, 1);
  ASSERT_EQ(st.joueurs[0].aigles[0].pos.colonne, 2);
  ASSERT_EQ(st.joueurs[0].aigles[0].identifiant, 10);
}

TEST_F(ApiTestColibri, ActionDeplacerAigleMauvaisePos1) {
  auto& st = api->game_state();
  Aigle a(10, {0, 0}, EFFET_GEL, 10, 0);
  st.joueurs[0].aigles.push_back(a);
  ASSERT_EQ(api->deplacer_aigle(10, position{-2, 1}), DESTINATION_INVALIDE);
  ASSERT_EQ(st.joueurs[0].aigles[0].pos.ligne, 0);
  ASSERT_EQ(st.joueurs[0].aigles[0].pos.colonne, 0);
  ASSERT_EQ(st.joueurs[0].aigles[0].identifiant, 10);
}

TEST_F(ApiTestColibri, ActionDeplacerAigleMauvaisePos2) {
  auto& st = api->game_state();
  Aigle a(10, {0, 0}, EFFET_GEL, 10, 0);
  st.joueurs[0].aigles.push_back(a);
  ASSERT_EQ(api->deplacer_aigle(10, position{4, 0}), DESTINATION_INVALIDE);
  ASSERT_EQ(st.joueurs[0].aigles[0].pos.ligne, 0);
  ASSERT_EQ(st.joueurs[0].aigles[0].pos.colonne, 0);
  ASSERT_EQ(st.joueurs[0].aigles[0].identifiant, 10);
}

// TODO YAKA
