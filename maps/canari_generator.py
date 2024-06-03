from random import randint, shuffle, choice
from json import dumps

WIDTH = 100
E_WIDTH = WIDTH - 1
HEIGHT = 100
E_HEIGHT = HEIGHT - 1
emplacements = [ [False] * E_WIDTH for _ in range(E_HEIGHT) ]

def impossibility(x, y):
    for sx, sy in ((x-1, y-1), (x-1, y), (x, y-1), (x, y)):
        if sx < 0 or sy < 0 or sx >= E_WIDTH-1 or sy >= E_HEIGHT-1:
            continue
        if emplacements[sy][sx] and emplacements[sy][sx+1] and emplacements[sy+1][sx] and emplacements[sy+1][sx+1]:
            return True
    return False

def random_pos():
    return randint(0, E_WIDTH-1), randint(0, E_HEIGHT-1)

starts = [random_pos() for _ in range(2)]
lists = [[], []]
for i in range(2):
    x, y = starts[i]
    emplacements[y][x] = True
    lists[i].append((x, y))


while all(len(l) > 0 for l in lists):
    i = randint(0, 1)
    k = randint(0, len(lists[i]) - 1)
    x, y = lists[i].pop(k)

    succ = [(x+1, y), (x, y+1), (x-1, y), (x, y-1)]
    shuffle(succ)
    for nx, ny in succ:
        if not (0 <= nx < E_WIDTH and 0 <= ny < E_HEIGHT):
            continue
        if emplacements[ny][nx]:
            continue
        if randint(0, 3) == 0:
            continue

        emplacements[ny][nx] = True
        if impossibility(nx, ny):
            emplacements[ny][nx] = False
            continue

        lists[i].append((nx, ny))

carte = [[ [False, False, False, False] for _ in range(WIDTH)] for _ in range(HEIGHT)]
for ey in range(E_HEIGHT):
    for ex in range(E_WIDTH):
        if emplacements[ey][ex]:
            carte[ey][ex][3] = True
            carte[ey][ex+1][2] = True
            carte[ey+1][ex][0] = True
            carte[ey+1][ex+1][1] = True


t_carte = []
for line in carte:
    t_line = []
    for cand in line:
        if randint(0, 1) != 0:
            pos = [i for i in range(4) if not cand[i]]
        else:
            pos = list(range(4))
        if randint(0, 10) == 0:
            t_line.append('X')
        else:
            t_line.append(str(choice(pos) + 1))
    t_carte.append(t_line)

for i in range(2):
    x, y = starts[i]
    t_carte[y][x] = 'X'

t_carte = [''.join(line) for line in t_carte]

gains = [
    [1 if emplacements[ey][ex] else -10 for ex in range(E_WIDTH)]
    for ey in range(E_HEIGHT)
]

aigles = []
for _ in range(HEIGHT * WIDTH // 40):
    x, y = random_pos()
    effet = choice([
        "GEL", "VIE", "METEORE", "MORT", "FEU"
    ])
    if effet == "FEU":
        puissance = choice([-2, -1, 0, 2, 3])
    elif effet == "VIE":
        puissance = choice([1, 2, 3])
    else:
        puissance = randint(0, 1)
    tour_eclosion = randint(0, 399)

    aigles.append(
        {
            "pos": {
                "x": x,
                "y": y
            },
            "effet": effet,
            "puissance": puissance,
            "tour_eclosion": tour_eclosion
        }
    )
                

json = {
    "hauteur": HEIGHT,
    "largeur": WIDTH,
    "carte": t_carte,
    "gains": gains,
    "joueur1": {
        "x": starts[0][0],
        "y": starts[0][1],
    },
    "joueur2": {
        "x": starts[1][0],
        "y": starts[1][1],
    },
    "aigles": aigles
}
print(dumps(json))
