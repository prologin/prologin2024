// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2015 Association Prologin <association@prologin.org>

#include "game_state.hh"

namespace
{
    position vec_to_pos(const std::vector<int>& vec)
    {
        return { vec[0], vec[1] };
    }
} // namespace


GameState::GameState(const rules::Players& players)
        : rules::GameState(players)
{
    // should not come here
}

GameState::GameState(const rules::Players& players, std::ifstream& json_file)
    : rules::GameState(players)
{
    tour = 0;

    json donnees;
    json_file >> donnees;

    int id_joueur = 0;

    int x1 = donnees["joueur1"]["x"];
    int y1 = donnees["joueur1"]["y"];
    std::vector<Aigle> aigles1;
    std::vector<position> villages1;
    villages1.push_back(vec_to_pos({x1, y1}));
    joueurs.emplace_back(id_joueur++, -1, aigles1, villages1);

    int x2 = donnees["joueur2"]["x"];
    int y2 = donnees["joueur2"]["y"];
    std::vector<Aigle> aigles2;
    std::vector<position> villages2;
    villages2.push_back(vec_to_pos({x2, y2}));
    joueurs.emplace_back(id_joueur++, -1, aigles2, villages2);

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
        if (!effet_string.compare("METEORE"))
            effet = EFFET_RAZ_DE_MAREE;
        else if (!effet_string.compare("VIE"))
            effet = EFFET_ACTIONS;
        else if (!effet_string.compare("FEU"))
            effet = EFFET_EFFRAYER;
        else if (!effet_string.compare("ABONDANCE"))
            effet = EFFET_MULTIPLICATIF;
        else if (!effet_string.compare("GEL"))
            effet = EFFET_BLOQUEUR;
        else
            effet = EFFET_BLOQUEUR;
        Aigle a(id, {aigle["pos"]["x"], aigle["pos"]["y"]}, effet, aigle["puissance"], aigle["tour_eclosion"]);
        aigles_sauvages.push_back(a);
    }

    std::vector<std::string> carte_texte;
    for (const std::string ligne : donnees["carte"])
        carte_texte.push_back(ligne);

    carte = Carte(carte_texte, gains);

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
    return tour >= NB_TOURS;
}


int calcul_multiplicatif(const std::vector<Aigle>& aigles, int x, int y)
{
    int multiplicatif = 1;
    for (const Aigle& aiglantine : aigles)
    {
        if (aiglantine.effet != EFFET_MULTIPLICATIF)
            continue;
        if (aiglantine.case_dans_rayon(x, y))
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
    return carte.get_gain(y, x) * multiplicatif;
}

void GameState::tour_suivant()
{
    int largeur, hauteur;
    std::tie(largeur, hauteur) = carte.get_dimension();

    Joueur& j_actuel = joueurs[joueur_actuel()];
    std::vector<std::vector<bool>> territoire = j_actuel.territoire(carte);
    for (int y = 0; y < hauteur - 1; y++)
    {
        for (int x = 0; x < largeur - 1; x++)
        {
            if (territoire[y][x])
                j_actuel.score += calcul_score(x, y);
        }
    }

    tour++;
}

int GameState::joueur_actuel() const
{
    return tour % (int)joueurs.size();
}

void GameState::debute_tour(int joueur)
{
    joueurs[joueur].points_action = TOUR_POINTS_ACTION;
    // FIXME
}

/* GESTIONNAIRE D'HISTORIQUE */

void GameState::ajoute_historique(action_hist action)
{
    historiques[joueur_actuel()].push_back({ false, action, PAS_DE_DRAKKAR });
}

void GameState::pose_drakkar(position pos, drakkar_debug couleur)
{
    action_hist vide;
    vide.fin = pos;
    historiques[joueur_actuel()].push_back({ true, vide, couleur });
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

    int largeur, hauteur;
    std::tie(largeur, hauteur) = carte.get_dimension();
    for (int y = 0; y < hauteur; y++)
    {
        std::string ligne = "";
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

    int largeur, hauteur;
    std::tie(largeur, hauteur) = carte.get_dimension();
    for (int y = 0; y < hauteur - 1; y++)
    {
        json jligne = json::array();
        for (int x = 0; x < largeur - 1; x++)
            jligne.push_back(carte.get_gain(x, y));

        jgains.push_back(jligne);
    }

    return jgains;
}

json dump_debug(const Carte& carte,
                const std::vector<std::vector<ActionInterne>>& historiques)
{
    int largeur, hauteur;
    std::tie(largeur, hauteur) = carte.get_dimension();

    std::vector<std::vector<int>> carte_debug(hauteur, std::vector<int>(largeur));

    for (int joueur = 0; joueur < 2; joueur++)
    {
        for (const ActionInterne& action_interne : historiques[joueur])
        {
            if (action_interne.est_drakkar)
            {
                position pos = action_interne.action.fin;
                switch (action_interne.couleur)
                {
                case PAS_DE_DRAKKAR:
                    carte_debug[pos.ligne][pos.colonne] = 0;
                    break;
                case DRAKKAR_ROUGE:
                    carte_debug[pos.ligne][pos.colonne] = 1;
                    break;
                case DRAKKAR_JAUNE:
                    carte_debug[pos.ligne][pos.colonne] = 2;
                    break;
                case DRAKKAR_BLEU:
                    carte_debug[pos.ligne][pos.colonne] = 3;
                    break;
                default:
                    break;
                }
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
    case EFFET_RAZ_DE_MAREE:
        jaigle["effet"] = "METEORE";
        break;
    case EFFET_ACTIONS:
        jaigle["effet"] = "VIE";
        break;
    case EFFET_EFFRAYER:
        jaigle["effet"] = "MORT";
        break;
    case EFFET_MULTIPLICATIF:
        jaigle["effet"] = "FEU";
        break;
    case EFFET_BLOQUEUR:
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
    for (const position& pos: joueur.villages)
        jvillages.push_back(dump_position(pos));
    jjoueur["villages"] = jvillages;

    json jaigles = json::array();
    for (const Aigle& aigle: joueur.aigles)
        jaigles.push_back(dump_aigle(aigle));
    jjoueur["aigles"] = jaigles;

    // TODO: score vs score total
    jjoueur["score"] = joueur.score;
    jjoueur["score_total"] = joueur.score;

    return jjoueur;
}

json dump_action(const ActionInterne& action_interne)
{
    // assert(!action_interne.est_drakkar)
    json jaction;
    action_hist action = action_interne.action;
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
    jaction["position"] = dump_position(action.fin);
    return jaction;
}

json GameState::dump() const
{
    json jetat;
    {
        json jjeu;
        jjeu["carte"] = dump_carte(carte);
        jjeu["gains"] = dump_gains(carte);
        jjeu["debug"] = dump_debug(carte, historiques);
        jjeu["joueur 1"] = dump_joueur(joueurs[0]);
        jjeu["joueur 2"] = dump_joueur(joueurs[1]);
        json jaigles = json::array();
        for (const Aigle& aigle : aigles_sauvages)
            jaigles.push_back(dump_aigle(aigle));
        jjeu["aigles"] = jaigles;
        jetat["jeu"] = jjeu;
    }

    json jtour;
    jtour["joueur_actuel"] = joueur_actuel();
    jtour["id_tour"] = tour;
    jtour["fin"] = est_termine();
    jetat["tour"] = jtour;

    // TODO: dump que le dernier tour?
    json jactions = json::array();
    for (int joueur = 0; joueur <= 1; joueur++)
    {
        for (const ActionInterne& action_interne: historiques[joueur])
        {
            if (action_interne.est_drakkar)
                continue;
            jactions.push_back(dump_action(action_interne));
        }
    }
    jetat["actions"] = jactions;

    return jetat;
}
