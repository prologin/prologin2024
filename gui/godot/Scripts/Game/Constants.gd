# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (c) 2024 Association Prologin <association@prologin.org>

# Ce fichier a été généré automatiquement. Modifiez-le à vos propre risques.

class_name Constants

extends Node

# Largeur maximale de la carte
const LARGEUR_MAX = 100

# Hauteur maximale de la carte
const HAUTEUR_MAX = 100

# Largeur minimale de la carte
const LARGEUR_MIN = 10

# Hauteur minimale de la carte
const HAUTEUR_MIN = 10

# Nombre de tours à jouer avant la fin de la partie
const NB_TOURS = 400

# Points maximum qu'apporte une ile
const POINTS_MAX = 100

# Points minimum qu'apporte une ile
const POINTS_MIN = -100

# Points d'action au début d'un tour
const TOUR_POINTS_ACTION = 2

# Coût de rotation d'une case en lien avec une île ennemi
const COUT_ROTATION_ENNEMI = 2

# Coût de rotation d'une case qui n'est pas en lien avec une île ennemi
const COUT_ROTATION_STANDARD = 1

# k est une constante (relou)
const LA_CONSTANTE_K = -41


# Erreurs possibles après avoir effectué une action
enum Erreur {
  OK,
  HORS_TOUR,
  POSITION_INVALIDE,
  PLUS_DE_PA,
  AIGLE_INVALIDE,
}

# Contenu topographique d'une case
enum TypeCase {
  VILLAGE,
  VILLAGE_J1,
  VILLAGE_J2,
  NORD_OUEST,
  NORD_EST,
  SUD_OUEST,
  SUD_EST,
}

# Type de drakkar de debug
enum DrakkarDebug {
  PAS_DE_DRAKKAR,
  BLEU,
  JAUNE,
  ROUGE,
}

# Types d'actions
enum TypeAction {
  TOURNER_CASE,
  ACTIVER_AIGLE,
  DEPLACER_AIGLE,
}

# Effet de l'aigle
enum EffetAigle {
  RAZ_DE_MAREE,
  ACTIONS,
  EFFRAYER,
  MULTIPLICATIF,
  BLOQUEUR,
}
