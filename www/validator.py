#! /usr/bin/env python3

from json import load as json_chargement
from sys import stdin
from os import environ

#try:
#    yaml_path = environ["PROLOGIN2024_YAML"]
#except KeyError:
#    raise EnvironmentError("PROLOGIN2024_YAML environment variable is undefined")

#with open(yaml_path) as stream:
#    config = chargement_sur(stream)

constantes = {}
#for constante in config["constant"]:
#    nom = constante["cst_name"]
#    valeur = constante["cst_val"]
#    constantes[nom] = valeur

LARGEUR_MIN = 10 #constantes["LARGEUR_MIN"]
LARGEUR_MAX = 100 #constantes["LARGEUR_MAX"]
HAUTEUR_MIN = 10 #constantes["HAUTEUR_MIN"]
HAUTEUR_MAX = 100 #constantes["HAUTEUR_MAX"]
POINTS_MIN = -100 #constantes["POINTS_MIN"]
POINTS_MAX = 100 #constantes["POINTS_MAX"]
NB_TOURS = 400#constantes["NB_TOURS"]

def extraire_propriete(json, nom):
    if nom not in json:
        raise ValueError(f"La propriété {nom} est manquante")
    return json[nom]

class Case:
    NORD_EST = '1'
    NORD_OUEST = '2'
    SUD_OUEST = '3'
    SUD_EST = '4'
    VILLAGE = 'X' # X

    def __new__(cls, caractere):
        match caractere:
            case '1':
                return Case.NORD_EST
            case '2':
                return Case.NORD_OUEST
            case '3':
                return Case.SUD_OUEST
            case '4':
                return Case.SUD_EST
            case 'X':
                return Case.VILLAGE
            case _:
                raise ValueError(f"Type de case `{caractere}` inconnue")

class Effet:
    FEU = 1
    VIE = 2
    METEORE = 3
    GEL = 4
    MORT = 5

    def __new__(cls, effet):
        match effet:
            case "FEU":
                return Effet.FEU
            case "VIE":
                return Effet.VIE
            case "METEORE":
                return Effet.METEORE
            case "GEL":
                return Effet.GEL
            case "MORT":
                return Effet.MORT
            case _:
                raise ValueError(f"Effet `{effet}` inconnu")

class Aigle:
    def __init__(self, effet, puissance, tour_eclosion):
        self.effet = effet
        self.puissance = puissance
        self.tour_eclosion = tour_eclosion

        if self.puissance < 0:
            if effet in (Effet.METEORE, Effet.GEL, Effet.MORT):
                raise ValueError(f"L'effet {effet} ne peut pas avoir une "
                                  "puissance négative")


class Joueur:
    def __init__(self, x, y):
        self.village = (x, y)

def lire_carte(carte_json, hauteur, largeur):
    carte = []

    if len(carte_json) != hauteur:
        raise ValueError(f"La carte devrait compter {hauteur} lignes, "
                         f"mais en contient {len(carte_json)}")

    for y, ligne_json in enumerate(carte_json):
        ligne = []
        if len(ligne_json) != largeur:
            raise ValueError(f"La ligne {y} de la carte_json devrait compter "
                             f"{largeur} caractères, mais en contient "
                             f"{len(ligne_json)}")

        for x, case_json in enumerate(ligne_json):
            try:
                case = Case(case_json)
            except ValueError as e:
                raise ValueError(f"Erreur dans la carte ligne {y} colonne {x}: "
                                 + repr(e))
            ligne.append(case)
        carte.append(ligne)

    return carte

def lire_gains(gains_json, hauteur, largeur):
    gains = []

    if len(gains_json) != hauteur - 1:
        raise ValueError(f"La gains devrait compter {hauteur - 1} lignes, "
                         f"mais en contient {len(gains_json)}")

    for y, ligne_json in enumerate(gains_json):
        ligne = []
        if len(ligne_json) != largeur - 1:
            raise ValueError(f"La ligne {y} de la gains_json devrait compter "
                             f"{largeur - 1} éléments, mais en contient "
                             f"{len(ligne_json)}")

        for x, gain in enumerate(ligne_json):
            if not POINTS_MIN <= gain <= POINTS_MAX:
                raise ValueError("Le nombre de points apportés par "
                                 f"l'emplacement ligne {y} colonne {x} "
                                 f"(`{gain}`) devrait être compris entre "
                                 f"{POINTS_MIN} et {POINTS_MAX}")
            ligne.append(gain)
        gains.append(ligne)

    return gains

def lire_joueur(joueur_json, carte):
    allowed_keys = ("x", "y")
    for key in joueur_json:
        if key not in allowed_keys:
            raise ValueError(f"La clé {key} n'est pas reconnue")

    x = extraire_propriete(joueur_json, "x")
    y = extraire_propriete(joueur_json, "y")

    hauteur = len(carte)
    largeur = len(carte[0])
    if not (0 <= x < largeur and 0 <= y < hauteur):
        raise ValueError(f"La position {x}, {y} est en dehors de la carte")

    if carte[y][x] != Case.VILLAGE:
        raise ValueError(f"La case ligne {y}, colonne {x} n'est pas un village")

    return Joueur(x, y)

def lire_aigles(aigles_json, hauteur, largeur):
    aigles = []
    for aigle_json in aigles_json:
        allowed_keys = ("pos", "effet", "puissance", "tour_eclosion")
        for key in aigle_json:
            if key not in allowed_keys:
                raise ValueError(f"La clé {key} n'est pas reconnue")
        pos_json = extraire_propriete(aigle_json, "pos")
        x = extraire_propriete(pos_json, "x")
        y = extraire_propriete(pos_json, "y")

        if not (0 <= x < largeur - 1 and 0 <= y < hauteur - 1):
            raise ValueError(f"L'aigle en position {x}, {y} n'est pas sur un "
                             f"emplacement valide")

        effet_json = extraire_propriete(aigle_json, "effet")
        effet = Effet(effet_json)

        puissance = extraire_propriete(aigle_json, "puissance")

        tour_eclosion = extraire_propriete(aigle_json, "tour_eclosion")
        if not 0 <= tour_eclosion < NB_TOURS:
            raise ValueError(f"L'éclosion prévue tour {tour_eclosion} est en "
                              "dehors de la durée de la partie")

        aigles.append(Aigle(effet, puissance, tour_eclosion))

    return aigles



def valide_carte(json):
    allowed_keys = ("largeur", "hauteur", "carte", "gains", "joueur1", "joueur2", "aigles")
    for key in json:
        if key not in allowed_keys:
            raise ValueError(f"La clé {key} n'est pas reconnue")

    largeur = extraire_propriete(json, "largeur")
    if not LARGEUR_MIN <= largeur <= LARGEUR_MAX:
        raise ValueError(f"Largeur incorrecte: {largeur} n'est pas dans "
                         f"l'intervalle [{LARGEUR_MIN}, {LARGEUR_MAX}]")

    hauteur = extraire_propriete(json, "hauteur")
    if not HAUTEUR_MIN <= hauteur <= HAUTEUR_MAX:
        raise ValueError(f"Hauteur incorrecte: {hauteur} n'est pas dans "
                         f"l'intervalle [{HAUTEUR_MIN}, {HAUTEUR_MAX}]")

    carte_json = extraire_propriete(json, "carte")
    carte = lire_carte(carte_json, hauteur, largeur)

    gains_json = extraire_propriete(json, "gains")
    gains = lire_gains(gains_json, hauteur, largeur)

    joueur1_json = extraire_propriete(json, "joueur1")
    try:
        joueur1 = lire_joueur(joueur1_json, carte)
    except ValueError as e:
        raise ValueError("Erreur en lisant le joueur 1: " + repr(e))

    joueur2_json = extraire_propriete(json, "joueur2")
    try:
        joueur2 = lire_joueur(joueur2_json, carte)
    except ValueError as e:
        raise ValueError("Erreur en lisant le joueur 2: " + repr(e))

    if joueur1.village == joueur2.village:
        raise ValueError("Les deux joueurs possèdent le même village, ligne "
                         f"{joueur1.village[1]} colonne {joueur1.village[0]}")

    aigles_json = extraire_propriete(json, "aigles")
    aigles = lire_aigles(aigles_json, hauteur, largeur)

if __name__ == '__main__':
    json = json_chargement(stdin)
    valide_carte(json)
