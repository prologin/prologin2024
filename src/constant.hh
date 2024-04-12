// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#pragma once


/// Largeur maximale de la carte
#define LARGEUR_MAX 100

/// Hauteur maximale de la carte
#define HAUTEUR_MAX 100

/// Largeur minimale de la carte
#define LARGEUR_MIN 10

/// Hauteur minimale de la carte
#define HAUTEUR_MIN 10

/// Nombre de tours à jouer avant la fin de la partie
#define NB_TOURS 400

/// Points maximum qu'apporte une ile
#define POINTS_MAX 100

/// Points minimum qu'apporte une ile
#define POINTS_MIN -100

/// Points d'action au début d'un tour
#define TOUR_POINTS_ACTION 2

/// Coût de rotation d'une case en lien avec une île ennemi
#define COUT_ROTATION_ENNEMI 2

/// Coût de rotation d'une case qui n'est pas en lien avec une île ennemi
#define COUT_ROTATION_STANDARD 1

/// k est une constante (relou)
#define LA_CONSTANTE_K -41


/// Erreurs possibles après avoir effectué une action
typedef enum erreur
{
    OK, ///< L'action a été effectuée avec succès
    HORS_TOUR, ///< Vous ne pouvez pas faire d'action en dehors de votre tour
    POSITION_INVALIDE, ///< La position fournie est invalide
    PLUS_DE_PA, ///< Vous n'avez plus de points d'action
    AIGLE_INVALIDE, ///< L'identifiant de l'aigle est invalide
} erreur;

/// Contenu topographique d'une case
typedef enum type_case
{
    VILLAGE_NEUTRE, ///< Village n'appartenant à aucun joueur
    VILLAGE_ACQUIS, ///< Village appartenant à un joueur
    NORD_OUEST, ///< Case dont le coin manquant est au nord ouest
    NORD_EST, ///< Case dont le coin manquant est au nord est
    SUD_OUEST, ///< Case dont le coin manquant est au sud ouest
    SUD_EST, ///< Case dont le coin manquant est au sud est
    CASE_INVALIDE, ///< Case invalide
} type_case;

/// Type de drakkar de debug
typedef enum drakkar_debug
{
    PAS_DE_DRAKKAR, ///< Aucun drakkar, enlève le drakkar présent
    DRAKKAR_BLEU, ///< Drakkar bleue
    DRAKKAR_JAUNE, ///< Drakkar jaune
    DRAKKAR_ROUGE, ///< Drakkar rouge
} drakkar_debug;

/// Types d'actions
typedef enum type_action
{
    ACTION_TOURNER_CASE, ///< Tourne une case, action ``tourner_case``
    ACTION_ACTIVER_AIGLE, ///< Active l'effet d'un aigle, action ``activer_aigle``
    ACTION_DEPLACER_AIGLE, ///< Déplace un aigle appartenant à l'utilisateur, action ``deplacer_aigle``
} type_action;

/// Effet de l'aigle
typedef enum effet_aigle
{
    EFFET_RAZ_DE_MAREE, ///< Déclenche un raz de marée
    EFFET_ACTIONS, ///< Donne des points actions
    EFFET_EFFRAYER, ///< Effraye les aigles d'une case
    EFFET_MULTIPLICATIF, ///< Multiplie les point d'une île
    EFFET_BLOQUEUR, ///< Bloque les mouvements de cases
} effet_aigle;


/// Position dans la carte, donnée par deux coordonnées
typedef struct position
{
    int colonne; ///< Abscisse
    int ligne; ///< Ordonnée
} position;

/// Dimensions de la carte
typedef struct dimension
{
    int hauteur; ///< Hauteur de la carte
    int largeur; ///< Largeur de la carte
} dimension;

/// Aigle
typedef struct aigle
{
    int identifiant; ///< Identifiant de l'aigle
    int joueur; ///< Identifiant du joueur auquel appartient l'aigle, -1 si n'appartient à aucun des joueurs
    position pos; ///< Position de l'aigle
    effet_aigle effet; ///< Effet de l'aigle
    int puissance; ///< Valeur de la puissance de l'aigle
    int tour_eclosion; ///< Tour d'éclosion de l'oeuf
    bool existant; ///< Si l'aigle existe toujours
} aigle;

/// Description complète d'une case
typedef struct etat_case
{
    type_case contenu; ///< Contenu topographique de la case
    int points; ///< Points dans le coin sud-est de la case
    position pos_case; ///< Position de la case
} etat_case;

/// Action représentée dans l'historique
typedef struct action_hist
{
    type_action action_type; ///< Type de l'action
    position debut; ///< Position de début de déplacement ou position de la case tournée
    position fin; ///< Position de fin
    int identifiant_aigle; ///< Identifiant de l'aigle utilisé
} action_hist;
