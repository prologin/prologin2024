// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2015 Association Prologin <association@prologin.org>

#include "game_state.hh"
#include "api.hh"
#include "constant.hh"
#include "rules.hh"

GameState::GameState(const rules::Players& players)
    : rules::GameState(players)
{
    std::cout << "CQLE" << std::endl;
    // should not come here
}
void GameState::capture(int largeur, int hauteur, int j_actuel_id,
                        const std::vector<std::vector<bool>>& territoire)
{
    Joueur& j_adverse = joueurs[(j_actuel_id + 1) % 2];
    Joueur& j_actuel = joueurs[j_actuel_id];
    const std::vector<std::vector<bool>>& territoire_adverse =
        j_adverse.territoire(carte);
    int tour_actuel = tour;
    auto const verifie_territoire =
        [&territoire, &territoire_adverse, tour_actuel](const Aigle& aigle)
    {
        return territoire[aigle.pos.ligne][aigle.pos.colonne] &&
               !territoire_adverse[aigle.pos.ligne][aigle.pos.colonne] &&
               aigle.tour_eclosion <= tour_actuel;
    };
    auto const verifie_deux_territoires =
        [largeur, hauteur, &territoire,
         &territoire_adverse](const auto& village)
    {
        bool dans_mon_territoire = false;
        for (int dx = 0; dx >= -1; dx--)
        {
            for (int dy = 0; dy >= -1; dy--)
            {
                int rx = village.colonne + dx;
                int ry = village.ligne + dy;
                if (0 <= rx && rx < largeur - 1 && 0 <= ry && ry < hauteur - 1)
                {
                    if (territoire_adverse[ry][rx])
                        return false;
                    else if (territoire[ry][rx])
                        dans_mon_territoire = true;
                }
            }
        }
        return dans_mon_territoire;
    };

    if (!aigles_sauvages.empty())
    {
        std::copy_if(aigles_sauvages.begin(), aigles_sauvages.end(),
                     std::back_inserter(j_actuel.aigles), verifie_territoire);
        aigles_sauvages.erase(std::remove_if(aigles_sauvages.begin(),
                                             aigles_sauvages.end(),
                                             verifie_territoire),
                              aigles_sauvages.end());
    }

    if (villages_libres.size() > 0)
    {
        std::copy_if(villages_libres.begin(), villages_libres.end(),
                     std::back_inserter(j_actuel.villages),
                     verifie_deux_territoires);
        villages_libres.erase(std::remove_if(villages_libres.begin(),
                                             villages_libres.end(),
                                             verifie_deux_territoires),
                              villages_libres.end());
    }
}

GameState::GameState(const rules::Players& players, std::ifstream& json_file)
    : rules::GameState(players)
{
    json donnees;
    json_file >> donnees;

    int x1 = donnees["joueur1"]["x"];
    int y1 = donnees["joueur1"]["y"];
    std::vector<Aigle> aigles1;
    std::vector<position> villages1;
    villages1.push_back({x1, y1});
    joueurs.emplace_back(0, TOUR_POINTS_ACTION, aigles1, villages1);

    int x2 = donnees["joueur2"]["x"];
    int y2 = donnees["joueur2"]["y"];
    std::vector<Aigle> aigles2;
    std::vector<position> villages2;
    villages2.push_back({x2, y2});
    joueurs.emplace_back(0, TOUR_POINTS_ACTION, aigles2, villages2);

    std::vector<std::vector<int>> gains;
    for (const auto& ligne : donnees["gains"])
    {
        std::vector<int> ligne_vec;
        for (int elt : ligne)
            ligne_vec.push_back(elt);
        gains.push_back(ligne_vec);
    }

    int id = 0;
    for (const auto& aigle : donnees["aigles"])
    {
        effet_aigle effet;
        std::string effet_string = aigle["effet"];
        if (effet_string == "METEORE")
            effet = EFFET_METEORE;
        else if (effet_string == "VIE")
            effet = EFFET_VIE;
        else if (effet_string == "MORT")
            effet = EFFET_MORT;
        else if (effet_string == "FEU")
            effet = EFFET_FEU;
        else if (effet_string == "GEL")
            effet = EFFET_GEL;
        else
            effet = EFFET_GEL;
        Aigle a(id, {aigle["pos"]["x"], aigle["pos"]["y"]}, effet,
                aigle["puissance"], aigle["tour_eclosion"]);
        aigles_sauvages.push_back(a);
        id++;
    }

    std::vector<std::string> carte_texte;
    for (const auto& ligne : donnees["carte"])
        carte_texte.push_back(ligne);

    carte = Carte(carte_texte, gains);
    const auto [largeur, hauteur] = carte.get_dimension();

    for (int y = 0; y < hauteur; y++)
    {
        for (int x = 0; x < largeur; x++)
        {
            if (carte.get_case(x, y) != VILLAGE)
                continue;
            if (x == villages1[0].colonne && y == villages1[0].ligne)
                continue;
            if (x == villages2[0].colonne && y == villages2[0].ligne)
                continue;
            villages_libres.push_back({x, y});
        }
    }

    historiques.emplace_back();
    historiques.emplace_back();
    for (int i = 0; i <= 1; i++)
    {
        std::vector<std::vector<bool>> territoire =
            joueurs[i].territoire(carte);
        capture(largeur, hauteur, i, territoire);
    }

    // FIXME
}

GameState::~GameState()
{
    // FIXME
}

GameState* GameState::copy() const
{
    return new GameState(*this);
}

bool GameState::est_termine() const
{
    return tour > NB_TOURS;
}

int calcul_multiplicatif(const std::vector<Aigle>& aigles, int x, int y)
{
    int multiplicatif = 1;
    for (const Aigle& aiglantine : aigles)
    {
        if (aiglantine.effet != EFFET_FEU)
            continue;
        if (aiglantine.pos.colonne != x || aiglantine.pos.ligne != y)
            continue;
        multiplicatif *= aiglantine.puissance;
    }
    return multiplicatif;
}

int GameState::calcul_score(int x, int y)
{
    int multiplicatif = 1;
    multiplicatif *= calcul_multiplicatif(joueurs[0].aigles, x, y);
    multiplicatif *= calcul_multiplicatif(joueurs[1].aigles, x, y);
    return carte.get_gain(x, y) * multiplicatif;
}

void GameState::tour_suivant()
{
    auto [largeur, hauteur] = carte.get_dimension();

    Joueur& j_actuel = joueurs[joueur_actuel()];
    j_actuel.score_tour = 0;

    const std::vector<std::vector<bool>>& territoire =
        j_actuel.territoire(carte);
    for (int y = 0; y < hauteur - 1; y++)
    {
        for (int x = 0; x < largeur - 1; x++)
        {
            if (territoire[y][x])
            {
                if (tour >= NB_TOURS - 2)
                    j_actuel.score_tour +=
                        calcul_score(x, y) * MULTIPLICATEUR_DERNIER_TOUR;
                else
                    j_actuel.score_tour += calcul_score(x, y);
            }
        }
    }
    capture(largeur, hauteur, joueur_actuel(), territoire);

    j_actuel.score += j_actuel.score_tour;
}

int GameState::joueur_actuel() const
{
    int tour = this->tour < 0 ? 0 : this->tour;
    return tour % (int)joueurs.size();
}

void GameState::debute_tour(int joueur)
{
    tour++;
    historiques[joueur_actuel()].clear();
    joueurs[joueur].points_action = TOUR_POINTS_ACTION;
}

/* GESTIONNAIRE D'HISTORIQUE */

void GameState::ajoute_historique(action_hist action)
{
    historiques[joueur_actuel()].push_back({false, action, PAS_DE_DRAKKAR});
}

void GameState::pose_drakkar(position pos, drakkar_debug couleur)
{
    action_hist vide;
    vide.fin = pos;
    historiques[joueur_actuel()].push_back({true, vide, couleur});
}

bool GameState::annuler()
{
    if (historiques[joueur_actuel()].empty())
        return false;
    historiques[joueur_actuel()].pop_back();
    return true;
}

/* DUMPER FOR GAME STATE */

json dump_position(const position& pos)
{
    json jpos;
    jpos["x"] = pos.colonne;
    jpos["y"] = pos.ligne;
    return jpos;
}

json dump_carte(const Carte& carte)
{
    json jcarte = json::array();

    const auto [largeur, hauteur] = carte.get_dimension();
    for (int y = 0; y < hauteur; y++)
    {
        std::string ligne;
        for (int x = 0; x < largeur; x++)
        {
            type_case type = carte.get_case(x, y);
            switch (type)
            {
            case VILLAGE:
                ligne.push_back('X');
                break;
            case NORD_EST:
                ligne.push_back('1');
                break;
            case NORD_OUEST:
                ligne.push_back('2');
                break;
            case SUD_OUEST:
                ligne.push_back('3');
                break;
            case SUD_EST:
                ligne.push_back('4');
                break;
            default:
                break;
            }
        }
        jcarte.push_back(ligne);
    }

    return jcarte;
}

json dump_gains(const Carte& carte)
{
    json jgains = json::array();

    const auto [largeur, hauteur] = carte.get_dimension();
    for (int y = 0; y < hauteur - 1; y++)
    {
        json jligne = json::array();
        for (int x = 0; x < largeur - 1; x++)
            jligne.push_back(carte.get_gain(x, y));

        jgains.push_back(jligne);
    }

    return jgains;
}

[[nodiscard]] static int drakkar_to_int(drakkar_debug drakkar)
{
    // Here we cannot use the actual enum values since the front has different
    // assumptions
    switch (drakkar)
    {
    case DRAKKAR_ROUGE:
        return 1;
    case DRAKKAR_JAUNE:
        return 2;
    case DRAKKAR_BLEU:
        return 3;
    default:
        return 0;
    }
}

json dump_debug(const Carte& carte,
                const std::vector<std::vector<ActionInterne>>& historiques)
{
    const auto [largeur, hauteur] = carte.get_dimension();

    std::vector<std::vector<int>> carte_debug(hauteur,
                                              std::vector<int>(largeur));

    for (int joueur = 0; joueur < 2; joueur++)
    {
        for (const ActionInterne& action_interne : historiques[joueur])
        {
            if (action_interne.est_drakkar)
            {
                position pos = action_interne.action.fin;
                carte_debug[pos.ligne][pos.colonne] =
                    drakkar_to_int(action_interne.couleur);
            }
        }
    }

    json jdebug = json::array();
    for (int y = 0; y < hauteur; y++)
    {
        json jligne = json::array();
        for (int x = 0; x < largeur; x++)
            jligne.push_back(carte_debug[y][x]);

        jdebug.push_back(jligne);
    }

    return jdebug;
}

json dump_aigle(const Aigle& aigle)
{
    json jaigle;
    jaigle["id"] = aigle.identifiant;
    jaigle["pos"] = dump_position(aigle.pos);
    switch (aigle.effet)
    {
    case EFFET_METEORE:
        jaigle["effet"] = "METEORE";
        break;
    case EFFET_VIE:
        jaigle["effet"] = "VIE";
        break;
    case EFFET_MORT:
        jaigle["effet"] = "MORT";
        break;
    case EFFET_FEU:
        jaigle["effet"] = "FEU";
        break;
    case EFFET_GEL:
        jaigle["effet"] = "GEL";
        break;
    }
    jaigle["puissance"] = aigle.puissance;
    jaigle["tour_eclosion"] = aigle.tour_eclosion;
    return jaigle;
}

json dump_joueur(const Joueur& joueur)
{
    json jjoueur;

    json jvillages = json::array();
    for (const position& pos : joueur.villages)
        jvillages.push_back(dump_position(pos));
    jjoueur["villages"] = jvillages;

    json jaigles = json::array();
    for (const Aigle& aigle : joueur.aigles)
        jaigles.push_back(dump_aigle(aigle));
    jjoueur["aigles"] = jaigles;

    jjoueur["score"] = joueur.score_tour;
    jjoueur["score_total"] = joueur.score;

    return jjoueur;
}

json dump_action(const ActionInterne& action_interne)
{
    // assert(!action_interne.est_drakkar)
    json jaction;
    action_hist action = action_interne.action;
    if (action_interne.est_drakkar)
    {
        jaction["type"] = "action_debug_poser_drakkar";
        jaction["id"] = drakkar_to_int(action_interne.couleur);
    }
    else
    {
        switch (action.action_type)
        {
        case ACTION_TOURNER_CASE:
            jaction["type"] = "action_tourner_case";
            break;
        case ACTION_ACTIVER_AIGLE:
            jaction["type"] = "action_activer_aigle";
            jaction["id"] = action.identifiant_aigle;
            break;
        case ACTION_DEPLACER_AIGLE:
            jaction["type"] = "action_deplacer_aigle";
            jaction["id"] = action.identifiant_aigle;
            break;
        }
    }
    jaction["position"] = dump_position(action.fin);
    return jaction;
}

json dump_territoire(const GameState& st)
{
    json territoire = json::array();

    auto territoire_j1 = st.joueurs[0].territoire(st.carte);
    auto territoire_j2 = st.joueurs[1].territoire(st.carte);

    int largeur, hauteur;
    std::tie(largeur, hauteur) = st.carte.get_dimension();

    std::vector<std::vector<int>> vec_terr(hauteur, std::vector<int>(largeur));
    // TODO : verif
    for (int y = 0; y < hauteur - 1; y++)
    {
        for (int x = 0; x < largeur - 1; x++)
        {
            if (territoire_j2[y][x] != 0 && territoire_j1[y][x] != 0)
                vec_terr[y][x] = 3;
            else
                vec_terr[y][x] =
                    (territoire_j2[y][x] != 0) ? 2 : territoire_j1[y][x];
        }
    }

    territoire = vec_terr;
    return territoire;
}

json GameState::dump() const
{
    json jetat;
    {
        json jjeu;
        int largeur, hauteur;
        std::tie(largeur, hauteur) = carte.get_dimension();
        jjeu["largeur"] = largeur;
        jjeu["hauteur"] = hauteur;
        jjeu["carte"] = dump_carte(carte);
        jjeu["gains"] = dump_gains(carte);
        jjeu["debug"] = dump_debug(carte, historiques);

        jjeu["territoire"] = dump_territoire(*this);

        jjeu["joueur1"] = dump_joueur(joueurs[0]);
        jjeu["joueur2"] = dump_joueur(joueurs[1]);
        json jaigles = json::array();
        for (const Aigle& aigle : aigles_sauvages)
        {
            jaigles.push_back(dump_aigle(aigle));
        }
        jjeu["aigles"] = jaigles;
        json jvillages_libres = json::array();
        for (const position& pos : villages_libres)
            jvillages_libres.push_back(dump_position(pos));
        jjeu["villages_libres"] = jvillages_libres;
        jetat["jeu"] = jjeu;
    }

    json jtour;
    jtour["joueur_actuel"] = joueur_actuel();
    jtour["id_tour"] = tour < 0 ? 0 : tour;
    jtour["fin"] = est_termine();
    jetat["tour"] = jtour;

    // TODO: dump que le dernier tour?
    json jactions = json::array();
    int joueur = joueur_actuel();
    for (const ActionInterne& action_interne : historiques[joueur])
    {
        jactions.push_back(dump_action(action_interne));
    }
    jetat["actions"] = jactions;

    return jetat;
}

extern Api* api;

void Rules::dump_state(std::ostream& ss)
{
    ss << api_->game_state().dump() << "\n";
}

extern "C" const char* dump_state_json()
{
    static std::string s;
    std::ostringstream ss;
    ss << api->game_state().dump() << "\n";
    s = ss.str();
    return s.c_str();
}

void GameState::sync_score()
{
    int score = joueurs[joueur_actuel()].score;
    players_[joueur_actuel()]->score = score;
    score = joueurs[joueur_actuel() ^ 1].score;
    players_[joueur_actuel() ^ 1]->score = score;
}
