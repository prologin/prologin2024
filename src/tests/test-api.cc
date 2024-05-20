#include "test-helpers.hh"

TEST_F(ApiTestColibri, TestApiMoiAdversaire)
{
    ASSERT_EQ(api->moi(), 0);
    ASSERT_EQ(api->adversaire(), 1);
}

TEST_F(ApiTestColibri, TestApiDimensionsCarte)
{
    ASSERT_EQ(api->dimensions_carte().largeur, 5);
    ASSERT_EQ(api->dimensions_carte().hauteur, 4);
}

TEST_F(ApiTestColibri, TestApiInfoCaseValide)
{
    ASSERT_EQ(api->info_case({0, 0}).contenu, SUD_EST);
    ASSERT_EQ(api->info_case({0, 0}).points, 3);
    ASSERT_EQ(api->info_case({0, 0}).pos_case.ligne, 0);
    ASSERT_EQ(api->info_case({0, 0}).pos_case.colonne, 0);

    ASSERT_EQ(api->info_case({1, 0}).contenu, SUD_OUEST);
    ASSERT_EQ(api->info_case({1, 0}).points, 2);
    ASSERT_EQ(api->info_case({1, 0}).pos_case.ligne, 0);
    ASSERT_EQ(api->info_case({1, 0}).pos_case.colonne, 1);

    ASSERT_EQ(api->info_case({4, 0}).contenu, VILLAGE);
    ASSERT_EQ(api->info_case({4, 0}).points, 0);
    ASSERT_EQ(api->info_case({4, 0}).pos_case.ligne, 0);
    ASSERT_EQ(api->info_case({4, 0}).pos_case.colonne, 4);

    ASSERT_EQ(api->info_case({4, 3}).contenu, SUD_OUEST);
    ASSERT_EQ(api->info_case({4, 3}).points, 0);
    ASSERT_EQ(api->info_case({4, 3}).pos_case.ligne, 3);
    ASSERT_EQ(api->info_case({4, 3}).pos_case.colonne, 4);
}

TEST_F(ApiTestColibri, TestApiInfoCaseInvalide)
{
    ASSERT_EQ(api->info_case({-1, 1}).contenu, CASE_INVALIDE);
    ASSERT_EQ(api->info_case({-1, -1}).contenu, CASE_INVALIDE);
    ASSERT_EQ(api->info_case({1, 4}).contenu, CASE_INVALIDE);
    ASSERT_EQ(api->info_case({5, 1}).contenu, CASE_INVALIDE);
}


TEST_F(ApiTestColibri, TestApiInfoAigles)
{
    ASSERT_EQ(api->info_aigles().size(), 1);

    ASSERT_EQ(api->info_aigles()[0].identifiant, 0);
    EXPECT_EQ(api->info_aigles()[0].joueur, -1);
    EXPECT_EQ(api->info_aigles()[0].pos.ligne, 1);
    EXPECT_EQ(api->info_aigles()[0].pos.colonne, 2);
    EXPECT_EQ(api->info_aigles()[0].effet, EFFET_GEL);
    EXPECT_EQ(api->info_aigles()[0].puissance, 4);
    EXPECT_EQ(api->info_aigles()[0].tour_eclosion, 42);
}

TEST_F(ApiTestColibri, TestApiInfoAiglesJoueurs)
{
    auto& st = api->game_state();
    Aigle a(1, {1, 0}, EFFET_VIE, 3, 0);
    ASSERT_EQ(st.joueurs[0].points_action, 2);
    st.joueurs[0].aigles.push_back(a);

    ASSERT_EQ(api->info_aigles().size(), 2);

    ASSERT_EQ(api->info_aigles()[0].identifiant, 0);
    EXPECT_EQ(api->info_aigles()[0].joueur, -1);
    EXPECT_EQ(api->info_aigles()[0].pos.ligne, 1);
    EXPECT_EQ(api->info_aigles()[0].pos.colonne, 2);
    EXPECT_EQ(api->info_aigles()[0].effet, EFFET_GEL);
    EXPECT_EQ(api->info_aigles()[0].puissance, 4);
    EXPECT_EQ(api->info_aigles()[0].tour_eclosion, 42);

    ASSERT_EQ(api->info_aigles()[1].identifiant, 1);
    EXPECT_EQ(api->info_aigles()[1].joueur, 0);
    EXPECT_EQ(api->info_aigles()[1].pos.ligne, 0);
    EXPECT_EQ(api->info_aigles()[1].pos.colonne, 1);
    EXPECT_EQ(api->info_aigles()[1].effet, EFFET_VIE);
    EXPECT_EQ(api->info_aigles()[1].puissance, 3);
    EXPECT_EQ(api->info_aigles()[1].tour_eclosion, 0);
}

TEST_F(ApiTestColibri, TestApiPointsAction)
{
    ASSERT_EQ(api->points_action(-1), -1);
    ASSERT_EQ(api->points_action(0), 2);
    ASSERT_EQ(api->points_action(1), 2);
    ASSERT_EQ(api->points_action(2), -1);
}

TEST_F(ApiTestColibri, TestApiScore)
{
    ASSERT_EQ(api->score(0), 0);
    ASSERT_EQ(api->score(1), 0);
    ASSERT_EQ(api->score(2), -1);

    auto& st = api->game_state();
    st.joueurs[0].score = 42;
    ASSERT_EQ(api->score(0), 42);
}

TEST_F(ApiTestColibri, TestApiHistorique)
{
    ASSERT_EQ(api->points_action(0), 2);
    position pos = {0, 0};
    ASSERT_EQ(api->tourner_case(pos), OK);
    ASSERT_EQ(api->info_case(pos).contenu, NORD_EST);
    ASSERT_EQ(api->historique()[0].action_type, ACTION_TOURNER_CASE);
    ASSERT_EQ(api->historique()[0].debut.ligne, 0);
    ASSERT_EQ(api->historique()[0].debut.colonne, 0);
    ASSERT_EQ(api->historique()[0].fin.ligne, 0);
    ASSERT_EQ(api->historique()[0].fin.colonne, 0);
    ASSERT_EQ(api->historique()[0].identifiant_aigle, -1);
}

TEST_F(ApiTestColibri, TestApiAnnuler)
{
    ASSERT_EQ(api->points_action(0), 2);
    position pos = {0, 0};
    ASSERT_EQ(api->tourner_case(pos), OK);
    ASSERT_EQ(api->info_case(pos).contenu, NORD_EST);
    ASSERT_EQ(api->points_action(0), 1);
    ASSERT_EQ(api->tourner_case(pos), OK);
    ASSERT_EQ(api->info_case(pos).contenu, NORD_OUEST);
    ASSERT_EQ(api->points_action(0), 0);

    EXPECT_EQ(api->annuler(), true);
    ASSERT_EQ(api->info_case(pos).contenu, NORD_EST);
    ASSERT_EQ(api->points_action(0), 1);

    EXPECT_EQ(api->annuler(), true);
    ASSERT_EQ(api->info_case(pos).contenu, SUD_EST);
    ASSERT_EQ(api->points_action(0), 2);

    EXPECT_EQ(api->annuler(), false);
    ASSERT_EQ(api->info_case(pos).contenu, SUD_EST);
    ASSERT_EQ(api->points_action(0), 2);
}

TEST_F(ApiTestColibri, TestApiCaseDansRayon)
{
    auto& st = api->game_state();
    Aigle a(1, {2, 1}, EFFET_GEL, 0, 0);
    st.joueurs[0].aigles.push_back(a);
    position caze = {2, 1};
    ASSERT_EQ(api->case_dans_rayon(1, caze), true);
    caze = {3, 1};
    ASSERT_EQ(api->case_dans_rayon(1, caze), true);
    caze = {3, 2};
    ASSERT_EQ(api->case_dans_rayon(1, caze), true);
    caze = {2, 2};
    ASSERT_EQ(api->case_dans_rayon(1, caze), true);

    caze = {0, 0};
    EXPECT_EQ(api->case_dans_rayon(1, caze), false);
    caze = {2, 0};
    EXPECT_EQ(api->case_dans_rayon(1, caze), false);
    caze = {4, 2};
    EXPECT_EQ(api->case_dans_rayon(1, caze), false);
    caze = {3, 1};
    EXPECT_EQ(api->case_dans_rayon(2, caze), false);
}

TEST_F(ApiTestColibri, TestApiListeVillages)
{
    ASSERT_EQ(api->liste_villages(-1).size(), 1);
    ASSERT_EQ(api->liste_villages(0).size(), 1);
    ASSERT_EQ(api->liste_villages(1).size(), 1);
    ASSERT_EQ(api->liste_villages(2).size(), 0);
    ASSERT_EQ(api->liste_villages(-2).size(), 0);
}

TEST_F(ApiTestColibri, TestApiRecupererTerritoire)
{
    // colibri
    // ..OO
    // ....
    // OOO.
    ASSERT_EQ(api->recuperer_territoire(0).size(), 3);
    position pos1 = {0, 2};
    position pos2 = {1, 2};
    position pos3 = {2, 2};
    bool b1 = false;
    bool b2 = false;
    bool b3 = false;
    for (auto& p : api->recuperer_territoire(0))
    {
        if (p.colonne == pos1.colonne && p.ligne == pos1.ligne)
            b1 = true;
        if (p.colonne == pos2.colonne && p.ligne == pos2.ligne)
            b2 = true;
        if (p.colonne == pos3.colonne && p.ligne == pos3.ligne)
            b3 = true;
    }
    ASSERT_TRUE(b2);
    ASSERT_TRUE(b1);
    ASSERT_TRUE(b3);
}