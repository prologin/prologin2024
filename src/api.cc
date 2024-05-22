// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "api.hh"

#include <memory>
#include <utility>

// global used in interface.cc
Api* api;

Api::Api(std::unique_ptr<GameState> game_state,
         std::shared_ptr<rules::Player> player)
    : rules::Api<GameState, erreur>(std::move(game_state), std::move(player))
{
    api = this;
}

dimension Api::dimensions_carte()
{
   auto [largeur, hauteur] = game_state_->carte.get_dimension();
   return { largeur, hauteur };
}

etat_case Api::info_case(position pos)
{
    int x = pos.colonne;
    int y = pos.ligne;
    if (!game_state_->carte.case_valide(x, y))
        return {CASE_INVALIDE, 0, pos};

    type_case info = game_state_->carte.get_case(x, y);
    int gains = game_state_->carte.get_gain(x, y);
    return { info, gains, pos };
}

std::vector<aigle> Api::info_aigles()
{
    std::vector<aigle> aigles;
    for (Aigle& aigle_sauvage : game_state_->aigles_sauvages)
    {
        const aigle aigle_converti = {
            aigle_sauvage.identifiant,
            -1,
            aigle_sauvage.pos,
            aigle_sauvage.effet,
            aigle_sauvage.puissance,
            aigle_sauvage.tour_eclosion
        };
        aigles.push_back(aigle_converti);
    }
    for (int joueur = 0; joueur <= 1; joueur++)
    {
        for (Aigle& aigle_joueur : game_state_->joueurs[joueur].aigles)
        {
            aigle aigle_converti = {
                aigle_joueur.identifiant,
                joueur,
                aigle_joueur.pos,
                aigle_joueur.effet,
                aigle_joueur.puissance,
                aigle_joueur.tour_eclosion
            };
            aigles.push_back(aigle_converti);
        }
    }

    return aigles;
}

int Api::points_action(int joueur)
{
    if (joueur < 0 || joueur >= 2)
        return -1;
    return game_state_->joueurs[joueur].points_action;
}

int Api::score(int joueur)
{
    if (joueur < 0 || joueur >= 2)
        return -1;
    return game_state_->joueurs[joueur].score;
}

std::vector<action_hist> Api::historique()
{
    std::vector<action_hist> actions_valides;
    std::vector<ActionInterne> actions_et_debug =
        game_state_->historiques[game_state_->joueur_actuel()];
        // TODO: Pourquoi `1 - game_state_->joueur_actuel()` ? (#jeux/2023)

    for (const ActionInterne &action : actions_et_debug) {
        if(!action.est_drakkar) {
            actions_valides.push_back(action.action);
        }
    }

    return actions_valides;
}
int Api::moi()
{
    return player_->id;
}

int Api::adversaire()
{
    return 1 - player_->id;
}

bool Api::annuler()
{
    // TODO: Verifier j'ai juste copié de l'année dernière
    return game_state_->annuler() && cancel();
}

int Api::tour_actuel()
{
    return game_state_->tour;
}

std::vector<position> Api::liste_villages(int joueur)
{
    if (joueur == -1)
        return game_state_->villages_libres;
    if (joueur > 1 || joueur < -1)
        return std::vector<position>();
    return game_state_->joueurs[joueur].villages;
}

std::vector<position> Api::recuperer_territoire(int joueur)
{
    if (joueur < 0 || joueur > 1)
        return std::vector<position>();

    const Carte& carte = game_state_->carte;
    std::vector<std::vector<bool>> territoire = game_state_->joueurs[joueur].territoire(carte);

    int largeur, hauteur;
    std::tie(largeur, hauteur) = game_state_->carte.get_dimension();

    std::vector<position> resultat;
    for (int y = 0; y < hauteur - 1; y++)
    {
        for (int x = 0; x < largeur - 1; x++)
        {
            if (territoire[y][x])
                resultat.push_back({x, y});
        }
    }

    return resultat;
}

bool Api::case_dans_rayon(int id, position pos)
{
    for (const Aigle& aiglantine : game_state_->aigles_sauvages)
    {
        if (aiglantine.identifiant == id)
        {
            return aiglantine.case_dans_rayon(pos.colonne, pos.ligne);
        }
    }
    for (int i = 0; i < 2; i++)
    {
        for (const Aigle& aiglantine : game_state_->joueurs[i].aigles)
        {
            if (aiglantine.identifiant == id)
            {
                return aiglantine.case_dans_rayon(pos.colonne, pos.ligne);
            }
        }
    }
    return false;
}

// TODO: aucune idée de pourquoi tout cela est là
std::ostream& operator<<(std::ostream& os, erreur v);
void Api::afficher_erreur(erreur v)
{
    std::cerr << v << std::endl;
}

std::ostream& operator<<(std::ostream& os, type_case v);
void Api::afficher_type_case(type_case v)
{
    std::cerr << v << std::endl;
}

std::ostream& operator<<(std::ostream& os, drakkar_debug v);
void Api::afficher_drakkar_debug(drakkar_debug v)
{
    std::cerr << v << std::endl;
}

std::ostream& operator<<(std::ostream& os, type_action v);
void Api::afficher_type_action(type_action v)
{
    std::cerr << v << std::endl;
}

std::ostream& operator<<(std::ostream& os, effet_aigle v);
void Api::afficher_effet_aigle(effet_aigle v)
{
    std::cerr << v << std::endl;
}

std::ostream& operator<<(std::ostream& os, position v);
void Api::afficher_position(position v)
{
    std::cerr << v << std::endl;
}

std::ostream& operator<<(std::ostream& os, dimension v);
void Api::afficher_dimension(dimension v)
{
    std::cerr << v << std::endl;
}

std::ostream& operator<<(std::ostream& os, aigle v);
void Api::afficher_aigle(aigle v)
{
    std::cerr << v << std::endl;
}

std::ostream& operator<<(std::ostream& os, etat_case v);
void Api::afficher_etat_case(etat_case v)
{
    std::cerr << v << std::endl;
}

std::ostream& operator<<(std::ostream& os, action_hist v);
void Api::afficher_action_hist(action_hist v)
{
    std::cerr << v << std::endl;
}
