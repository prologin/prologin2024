// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#pragma once

#include <memory>
#include <string>
#include <vector>

#include <rules/api.hh>
#include <rules/player.hh>

#include "actions.hh"
#include "constant.hh"
#include "game_state.hh"

/**
 * The methods of this class are exported through 'interface.cc'
 * to be called by the clients
 */
class Api final : public rules::Api<GameState, erreur>
{
public:
    Api(std::unique_ptr<GameState> game_state,
        std::shared_ptr<rules::Player> player);
    ~Api() {}

    /// Rotation d'un quart de tour d'une case dans le sens trigonométrique
    /// (anti-horaire)
    ApiActionFunc<ActionTournerCase> tourner_case{this};

    /// Activer l'effet d'un aigle
    ApiActionFunc<ActionActiverAigle> activer_aigle{this};

    /// Déplace un aigle
    ApiActionFunc<ActionDeplacerAigle> deplacer_aigle{this};

    /// Renvoie les dimensions hauteur largeur de la carte
    dimension dimensions_carte();

    /// Renvoie les informations concernant une case
    etat_case info_case(position pos);

    /// Renvoie la liste d'aigles
    std::vector<aigle> info_aigles();

    /// Renvoie la liste des villages. Identifiant -1 pour les villages libres.
    std::vector<position> liste_villages(int joueur);

    /// Renvoie le nombre de points d'action restant Renvoie -1 si le joueur
    /// est invalide.
    int points_action(int joueur);

    /// Renvoie le score d'un joueur. Renvoie -1 si le joueur est invalide.
    int score(int joueur);

    /// Pose un drakkar de debug sur la case indiquée
    ApiActionFunc<ActionDebugPoserDrakkar> debug_poser_drakkar{this};

    /// Renvoie la liste des actions effectuées par l'adversaire durant son
    /// tour, dans l'ordre chronologique. Les actions de débug n'apparaissent
    /// pas dans cette liste.
    std::vector<action_hist> historique();

    /// Renvoie un tableau en 2 dimensions des emplacements indiquant avec vrai
    /// les emplacement du territoire et faux sinon.
    std::vector<position> recuperer_territoire(int joueur);

    /// Renvoie vrai si la case est dans le rayon de l'aigle. Si l'aigle est
    /// invalide, renvoie faux.
    bool case_dans_rayon(int id, position pos);

    /// Renvoie votre numéro de joueur.
    int moi();

    /// Renvoie le numéro du joueur adverse.
    int adversaire();

    /// Annule la dernière action. Renvoie faux quand il n'y a pas d'action à
    /// annuler ce tour-ci.
    bool annuler();

    /// Retourne le numéro du tour actuel.
    int tour_actuel();

    /// Affiche le contenu d'une valeur de type erreur
    void afficher_erreur(erreur v);

    /// Affiche le contenu d'une valeur de type type_case
    void afficher_type_case(type_case v);

    /// Affiche le contenu d'une valeur de type drakkar_debug
    void afficher_drakkar_debug(drakkar_debug v);

    /// Affiche le contenu d'une valeur de type type_action
    void afficher_type_action(type_action v);

    /// Affiche le contenu d'une valeur de type effet_aigle
    void afficher_effet_aigle(effet_aigle v);

    /// Affiche le contenu d'une valeur de type position
    void afficher_position(position v);

    /// Affiche le contenu d'une valeur de type dimension
    void afficher_dimension(dimension v);

    /// Affiche le contenu d'une valeur de type aigle
    void afficher_aigle(aigle v);

    /// Affiche le contenu d'une valeur de type etat_case
    void afficher_etat_case(etat_case v);

    /// Affiche le contenu d'une valeur de type action_hist
    void afficher_action_hist(action_hist v);

};
