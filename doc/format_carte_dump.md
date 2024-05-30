# Format de la carte

> **NOTE**: Voir <prologin2024.yml> pour les enums / structs.

## Format textuel

| Type de la case | Symbole |
| --------------- | ------- |
| Village         | X       |
| Nord est        | 1       |
| Nord ouest      | 2       |
| Sud ouest       | 3       |
| Sud est         | 4       |


## JSON carte

```json
{
    // Carte
    "carte": [
        "142314333341322",
        "142X14333341322",
        "142314333341X22",
    ],
    "gains": [
        [1, 2, 3, -1, 2],
        [1, 2, -1, -1, 2],
        [1, 2, 3, -1, 2],
    ],
    // Joueurs
    "joueur1": {
        "x": 0,
        "y": 3
    },
    "joueur2": {
        "x": 4,
        "y": 0
    },
    // Aigles
    "aigles": [
         {
             "pos": {
                 "x": 618,
                 "y": 42
             },
             "effet": "GEL",
             "puissance": 1,
             "tour_eclosion": 20
         }
    ]
}
```

# Format du dump

Le dump contient N+1 lignes représentant le tour 0 à N inclus, chacune contient ce JSON:

```json
{
  // Liste des actions
  "actions":[
  {"position":{"x":1,"y":1},"type":"action_tourner_case"}
  ],
  // Etat du jeu au début de ce tour
  "jeu": {
      // Liste des aigles sauvages (non capturés)
      "aigles": [
        {"effet":"FEU","id":0,"pos":{"x":1,"y":5},"puissance":3,"tour_eclosion":10},
        {"effet":"VIE","id":2,"pos":{"x":5,"y":5},"puissance":10,"tour_eclosion":10},
        {"effet":"MORT","id":8,"pos":{"x":1,"y":7},"puissance":42,"tour_eclosion":10},
        {"effet":"METEORE","id":9,"pos":{"x":3,"y":7},"puissance":3,"tour_eclosion":10}
      ],
      // Pareil que dans la section `Format de la carte`
      "carte": [
        "4444X33333",
        "4144433333",
        "4444433333",
        "4444433333",
        "4444433333",
        "4222431113",
        "4442341333",
        "4113214223",
        "1442341332",
        "1113214222",
        "1333124442",
        "1111122222",
        "1111122222",
        "1111122222",
        "1111122222",
        "11111X2222"
      ],
      "debug":[
        [0,0,0,0,0,0,0,0,0,0], // 0 = pas de debug, 1 = rouge, 2 = vert, 3 = bleu
        [0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0]
      ],
      "gains": [
        [0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0],
        [10,0,1,0,1,0,1,0,10],
        [-100,-100,-100,-100,-100,-100,-100,-100,-100],
        [-100,0,-100,0,-100,0,-100,0,-100],
        [-100,-100,-100,-100,-100,-100,-100,-100,-100],
        [-100,0,-100,0,-100,0,-100,0,-100],
        [-100,-100,-100,-100,-100,-100,-100,-100,-100],
        [-100,0,-100,0,-100,0,-100,0,-100],
        [-100,-100,-100,-100,-100,-100,-100,-100,-100],
        [10,0,1,0,1,0,1,0,10],
        [0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0]
      ],
      // dimension de la carte
      "hauteur":16,
      "largeur":10,
      "joueur1": {
          // Liste des aigles possédés par le joueur
          "aigles": [],
          // Score du tour actuel
          "score": 0,
          // Score total
          "score_total": 0,
          // Liste des villages du joueurs
          "villages": [
              {"x":4,"y":0}
          ]
      },
      "joueur2": {
          "aigles": [],
          "score": 0,
          "score_total": 0,
          "villages": [
              {"x":5,"y":15}
          ]
      },
      // Carte du territoire des joueurs
      // 0 = libre, 1 = joueur1, 2 = joueur2, 3 = appartient au 2 joueurs
      "territoire": [
        [0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0]
        ],
      // Liste des villages libres (non capturés)
      "villages_libres": []
  },
  // Celui qui joue
  "tour": {
      "fin":false,
      "id_tour":1,
    "joueur_actuel":1
  }
}
```
