// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "api.hh"

#include <memory>
#include <utility>

// global used in interface.cc
Api* api;

Api::Api(std::unique_ptr<GameState> game_state,
         std::shared_ptr<rules::Player> player)
    : rules::Api<GameState, erreur>(std::move(game_state), player)
{
    api = this;
}

dimension Api::dimensions_carte()
{
   int largeur, hauteur;
   std::tie(largeur, hauteur) = game_state_->carte.get_dimension();
   return { hauteur, largeur };
}

etat_case Api::info_case(position pos)
{
    // TODO: gestion d'erreur
    int x = pos.colonne;
    int y = pos.ligne;
    type_case info = game_state_->carte.get_case(x, y);
    int gains = game_state_->carte.get_gain(x, y);
    return { info, gains, pos };
}

std::vector<aigle> Api::info_aigles()
{
    // TODO
    abort();
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
    // TODO
    abort();
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
