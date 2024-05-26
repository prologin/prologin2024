# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (c) 2024 Association Prologin <association@prologin.org>

# Ce fichier a été généré automatiquement. Modifiez-le à vos propre risques.

class_name Constants

extends Node

const TILE_SIZE = 96
const HALF_TILE_SIZE = int(TILE_SIZE / 2)

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
	VILLAGE_J1, # TODO : Village custom
	VILLAGE_J2,
	NORD_OUEST,
	SUD_OUEST,
	SUD_EST,
	NORD_EST,
	OEUF_BLANC,
	OEUF_GRIS,
	OEUF_ROUGE,
	OEUF_JAUNE,
	OEUF_BLEU,
	OEUF_VERT,
	AIGLE_BLANC,
	AIGLE_GRIS,
	AIGLE_ROUGE,
	AIGLE_JAUNE,
	AIGLE_BLEU,
	AIGLE_VERT,
	DRAPEAU_ROUGE,
	DRAPEAU_BLEU,
	DEBUG_ROUGE,
	DEBUG_BLEU,
	DEBUG_VERT,
	PONT_1_NORD_OUEST,
	PONT_1_SUD_OUEST,
	PONT_1_SUD_EST,
	PONT_1_NORD_EST,
	PONT_2_NORD_OUEST,
	PONT_2_SUD_OUEST,
	PONT_2_SUD_EST,
	PONT_2_NORD_EST,
	ILOTS_NORD_OUEST,
	ILOTS_SUD_OUEST,
	ILOTS_SUD_EST,
	ILOTS_NORD_EST,
	ILE_SUD_OUEST,
	ILE_SUD_EST,
	ILE_NORD_EST,
	ILE_NORD_OUEST,
	PONT_OUEST,
	PONT_SUD,
	PONT_EST,
	PONT_NORD,
	AIGLE_J1,
	AIGLE_J2,
	EAU,
	ILOT_NORD_OUEST,
	ILOT_SUD_OUEST,
	ILOT_SUD_EST,
	ILOT_NORD_EST,
	ILE_SUD_OUEST,
	ILE_SUD_EST,
	ILE_NORD_EST,
	ILE_NORD_OUEST,
	BTN_REPLAY,
	BTN_PLAY,
	BTN_PAUSE,
	BTN_NEXT,
	BTN_PREV,
	BTN_END,
	BTN_START,
	TERRITOIRE_1,
	TERRITOIRE_2,
	TERRITOIRE_1_2,
	CERCLE_GRIS,
	CERCLE_ROUGE,
	CERCLE_BLEU,
	CERCLE_JAUNE,
	CERCLE_VERT,
	FLECHE_TOURNE,
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

enum EditorMode {
	BACKGROUND,
	FOREGROUND,
	POINTS,
}
