from api import *
from sys import stderr
from enum import Enum
from heapq import *
from typing import Dict

DEBUG = True
def debug(*args, **kwargs):
    if DEBUG:
        print(*args, **kwargs, file=stderr)

hauteur = 0
largeur = 0
me = 0
other = 0
carte : List[List[etat_case]] = []
souhait : List[List[Dict[type_case, bool]]] = []
mon_territoire : List[List[bool]] = []
territoire_adverse: List[List[bool]] = []
direction = ((type_case.NORD_OUEST, type_case.NORD_EST),
             (type_case.SUD_OUEST,  type_case.SUD_EST))

def oppose(direction: type_case):
    if direction == type_case.NORD_OUEST:
        return type_case.SUD_EST
    if direction == type_case.NORD_EST:
        return type_case.SUD_OUEST
    if direction == type_case.SUD_EST:
        return type_case.NORD_OUEST
    if direction == type_case.SUD_OUEST:
        return type_case.NORD_EST
    return type_case.CASE_INVALIDE

# Fonction appelée au début de la partie.
def partie_init():
    global hauteur, largeur, me, other
    largeur, hauteur = dimensions_carte()
    me = moi()
    other = adversaire()

    for y in range(hauteur):
        ligne = []
        ligne_souhait = []
        for x in range(largeur):
            ligne.append(info_case((x, y)))
            ligne_souhait.append({
                type_case.NORD_EST: False,
                type_case.NORD_OUEST: False,
                type_case.SUD_EST: False,
                type_case.SUD_OUEST: False,
            })
        carte.append(ligne)
        souhait.append(ligne_souhait)

    for y in range(hauteur - 1):
        mon_territoire.append([False] * (largeur - 1))
        territoire_adverse.append([False] * (largeur - 1))

    for x, y in recuperer_territoire(me):
        mon_territoire[y][x] = True
    for x, y in recuperer_territoire(other):
        territoire_adverse[y][x] = True

def update_carte():
    for y in range(hauteur):
        for x in range(largeur):
            case = info_case((x, y))
            carte[y][x] = case

    for y in range(hauteur - 1):
        for x in range(largeur - 1):
            mon_territoire[y][x] = False
            territoire_adverse[y][x] = False

    for x, y in recuperer_territoire(me):
        mon_territoire[y][x] = True

    for x, y in recuperer_territoire(other):
        territoire_adverse[y][x] = True


def print_carte():
    to_display = [[' '] * largeur * 2 for _ in range(hauteur * 2)]
    display_map = {
        type_case.NORD_EST:      ((("┘"," "),("┐","┌")),(("╝"," "),("╗","╔"))),
        type_case.NORD_OUEST:    (((" ","└"),("┐","┌")),((" ","╚"),("╗","╔"))),
        type_case.SUD_OUEST:     ((("┘","└"),(" ","┌")),(("╝","╚"),(" ","╔"))),
        type_case.SUD_EST:       ((("┘","└"),("┐"," ")),(("╝","╚"),("╗"," "))),
        type_case.VILLAGE:       ((("█","█"),("█","█")),(("█","█"),("█","█"))),
        type_case.CASE_INVALIDE: ((("?","?"),("?","?")),(("?","?"),("?","?"))),
    }
    for y in range(hauteur):
        for x in range(largeur):
            disabled, enabled = display_map[carte[y][x].contenu]
            for dy in range(2):
                for dx in range(2):
                    ey = y + dy - 1
                    ex = x + dx - 1
                    if (0 <= ex < largeur - 1 and 0 <= ey < hauteur - 1 and
                        (mon_territoire[ey][ex] or territoire_adverse[ey][ex])):
                        to_display[2*y+dy][2*x+dx] = enabled[dy][dx]
                    else:
                        to_display[2*y+dy][2*x+dx] = disabled[dy][dx]

    debug('┌' + '─' * largeur * 2 + '┐')
    for line in to_display:
        debug('│', *line, '│', sep = '')
    debug('└' + '─' * largeur * 2 + '┘')

def print_souhait():
    to_display = [[' '] * largeur * 2 for _ in range(hauteur * 2)]
    for y in range(hauteur):
        for x in range(largeur):
            if souhait[y][x][type_case.NORD_OUEST]:
                to_display[2*y][2*x] = '┘'
            if souhait[y][x][type_case.NORD_EST]:
                to_display[2*y][2*x+1] = '└'
            if souhait[y][x][type_case.SUD_OUEST]:
                to_display[2*y+1][2*x] = '┐'
            if souhait[y][x][type_case.SUD_EST]:
                to_display[2*y+1][2*x+1] = '┌'

    debug('┌' + '─' * largeur * 2 + '┐')
    for line in to_display:
        debug('│', *line, '│', sep = '')
    debug('└' + '─' * largeur * 2 + '┘')


def case_bordant_emplacement(x:int, y:int):
    if 0 <= x < largeur and 0 <= y < hauteur:
        yield x, y, type_case.NORD_OUEST
    if 0 <= x+1 < largeur and 0 <= y < hauteur:
        yield x+1, y, type_case.NORD_EST
    if 0 <= x < largeur and 0 <= y+1 < hauteur:
        yield x, y+1, type_case.SUD_OUEST
    if 0 <= x+1 < largeur and 0 <= y+1 < hauteur:
        yield x+1, y+1, type_case.SUD_EST

def emplacement_bordant_case(x:int, y:int):
    if 0 <= x-1 < largeur - 1 and 0 <= y-1 < hauteur - 1:
        yield x-1, y-1, type_case.NORD_OUEST
    if 0 <= x < largeur - 1 and 0 <= y-1 < hauteur - 1:
        yield x, y-1, type_case.NORD_EST
    if 0 <= x-1 < largeur - 1 and 0 <= y < hauteur - 1:
        yield x-1, y, type_case.SUD_OUEST
    if 0 <= x < largeur - 1 and 0 <= y < hauteur - 1:
        yield x, y, type_case.SUD_EST

def essaie_mettre_ilot(x:int, y:int, direction:type_case):
    souhait[y][x][direction] = True

def essaie_construire_ile(x:int, y:int):
    for cx, cy, direction in case_bordant_emplacement(x, y):
        essaie_mettre_ilot(cx, cy, oppose(direction))

def essaie_construire_chaine(cx:int, cy:int, ex:int, ey:int):
    cost = [[float('inf')] * (largeur - 1) for _ in range(hauteur - 1)]
    pred: list[list[tuple[int, int] | None]]\
         = [[None] * (largeur - 1) for _ in range(hauteur - 1)]

    def optimal_path_from(x, y):
        while pred[y][x] is not None:
            yield x, y
            x, y = pred[y][x]
        yield x, y

    cost[ey][ex] = 0
    queue: list[tuple[int, int, int]]\
          = [(0, ex, ey)]

    sxf, syf = None, None
    visited = [[False] * (largeur-1) for _ in range(hauteur-1)]
    while len(queue) > 0:
        t, x, y = heappop(queue)

        finished = False
        for sx, sy, _ in case_bordant_emplacement(x, y):
            if (cx, cy) == (sx, sy):
                sxf, syf = x, y
                finished = True
                break

        if finished:
            break

        for nx, ny in ((x-1, y), (x, y-1), (x, y+1), (x+1, y)):
            if not (0 <= nx < largeur-1 and 0 <= ny < hauteur-1):
                continue

            if (nx, ny) in optimal_path_from(x, y):
                continue

            for (ax, ay) in optimal_path_from(x, y):
                visited[ay][ax] = True

            imp = False
            for tlx, tly in ((x-1, y-1), (x-1, y), (x, y-1), (x, y)):
                if tlx >= largeur - 2 or tly >= hauteur - 2:
                    continue
                if visited[tly][tlx] and visited[tly][tlx+1] and visited[tly+1][tlx] and visited[tly+1][tlx+1]:
                    imp = True
                    break

            for (ax, ay) in optimal_path_from(x, y):
                visited[ay][ax] = False

            if imp:
                continue

            nt = t - carte[ny][nx].gains + 1
            if nt < cost[ny][nx]:
                pred[ny][nx] = (x, y)
                cost[ny][nx] = nt
                heappush(queue, (nt, nx, ny))

    if sxf == None or syf == None:
        return False
    if cost[syf][sxf] > 0:
        return False

    if DEBUG:
        for ax, ay in optimal_path_from(sxf, syf):
            debug_poser_drakkar((ax, ay), drakkar_debug.DRAKKAR_BLEU)

    for ax, ay in optimal_path_from(sxf, syf):
        essaie_construire_ile(ax, ay)

    return True


def place_aigle(aigle: aigle):
    if aigle.effet == effet_aigle.EFFET_FEU:
        meilleur_emplacement = (0, 0)
        meilleur_gain = -float('inf')
        for (x, y) in recuperer_territoire(me):
            candidat_gain = carte[y][x].gains
            if candidat_gain > meilleur_gain:
                meilleur_gain = candidat_gain
                meilleur_emplacement = (x, y)
        deplacer_aigle(aigle.identifiant, meilleur_emplacement)

def applique_souhait():
    for y in range(hauteur):
        for x in range(largeur):
            if carte[y][x].contenu == type_case.VILLAGE:
                continue
            while souhait[y][x][carte[y][x].contenu]:
                result = tourner_case((x, y))
                carte[y][x] = info_case((x, y))
                if result != erreur.OK:
                    return False
    return True

def debut_tour():
    update_carte()
    print_carte()

    for y in range(hauteur):
        for x in range(largeur):
            souhait[y][x] = {
                type_case.NORD_EST: False,
                type_case.NORD_OUEST: False,
                type_case.SUD_EST: False,
                type_case.SUD_OUEST: False,
            }

    for aigle in info_aigles():
        if aigle.joueur == me:
            if aigle.effet == effet_aigle.EFFET_VIE and tour_actuel() >= NB_TOURS - 2:
                activer_aigle(aigle.identifiant)

# Fonction appelée à chaque tour.
def jouer_tour():
    debut_tour()

    vx, vy = liste_villages(me)[0]
    for aigle in info_aigles():
        ex, ey = aigle.pos
        if aigle.joueur == -1 and aigle.tour_eclosion <= tour_actuel():
            essaie_construire_chaine(vx, vy, ex, ey)
            if not applique_souhait():
                return fin_tour()

    scores = []
    for y in range(hauteur-1):
        for x in range(largeur-1):
            scores.append((carte[y][x].gains, x, y))

    scores.sort(reverse=True)
    for _, ex, ey in scores:
        essaie_construire_chaine(vx, vy, ex, ey)
        if not applique_souhait():
            return fin_tour()


def fin_tour():
    print_souhait()
    update_carte()
    for aigle in info_aigles():
        if aigle.joueur == me:
            place_aigle(aigle)


# Fonction appelée à la fin de la partie.
def partie_fin():
    # TODO
    pass
