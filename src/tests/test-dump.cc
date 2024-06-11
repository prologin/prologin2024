#include <gtest/gtest.h>

#include "json.hpp"
#include "test-helpers.hh"

using nlohmann::json;
using namespace nlohmann::literals;

TEST_F(ApiTestColibri, DumpSimple) {
  auto &state = api->game_state();
  const auto json = state.dump();
  const auto json_expected = R"(
    {
      "actions": [],
      "jeu": {
        "aigles": [
          {
            "effet": "GEL",
            "id": 0,
            "pos": {
              "x": 2,
              "y": 1
            },
            "puissance": 4,
            "tour_eclosion": 42
          }
        ],
        "carte": [
          "4332X",
          "42443",
          "312X3",
          "X4343"
        ],
        "debug": [
          [ 0, 0, 0, 0, 0 ],
          [ 0, 0, 0, 0, 0 ],
          [ 0, 0, 0, 0, 0 ],
          [ 0, 0, 0, 0, 0 ]
        ],
        "gains": [
          [ 3, 2, 4, 10 ],
          [ 0, -4, -4, 0 ],
          [ 10, 4, 2, 3 ]
        ],
        "hauteur": 4,
        "joueur1": {
          "aigles": [],
          "score": 0,
          "score_total": 0,
          "villages": [
            {
              "x": 0,
              "y": 3
            },
            {
              "x": 3,
              "y": 2
            }
          ]
        },
        "joueur2": {
          "aigles": [],
          "score": 0,
          "score_total": 0,
          "villages": [
            {
              "x": 4,
              "y": 0
            }
          ]
        },
        "largeur": 5,
        "territoire": [
          [ 0, 0, 2, 2, 0 ],
          [ 0, 0, 0, 0, 0 ],
          [ 1, 1, 1, 0, 0 ],
          [ 0, 0, 0, 0, 0 ]
        ],
        "villages_libres": []
      },
      "tour": {
        "fin": false,
        "id_tour": 0,
        "joueur_actuel": 0
      }
    }
      )"_json;
  EXPECT_EQ(json_expected, json);
}

TEST_F(ApiTestColibri, TestDrakkar) {
  auto &state = api->game_state();
  EXPECT_EQ(api->debug_poser_drakkar(position{2, 3}, DRAKKAR_JAUNE), OK);

  EXPECT_EQ(api->debug_poser_drakkar(position{2, 4}, DRAKKAR_BLEU),
            POSITION_INVALIDE);
  EXPECT_EQ(api->debug_poser_drakkar(position{2, 1}, DRAKKAR_BLEU), OK);

  EXPECT_EQ(api->debug_poser_drakkar(position{4, 4}, DRAKKAR_ROUGE),
            POSITION_INVALIDE);
  EXPECT_EQ(api->debug_poser_drakkar(position{3, 2}, DRAKKAR_ROUGE), OK);

  const auto json = state.dump();
  const auto json_expected = R"(
    {
      "actions": [
        {
            "id": 2,
            "position": {
                "x": 2,
                "y": 3
            },
            "type": "action_debug_poser_drakkar"
        },
        {
            "id": 3,
            "position": {
                "x": 2,
                "y": 1
            },
            "type": "action_debug_poser_drakkar"
        },
        {
            "id": 1,
            "position": {
                "x": 3,
                "y": 2
            },
            "type": "action_debug_poser_drakkar"
        }
      ],
      "jeu": {
        "aigles": [
          {
            "effet": "GEL",
            "id": 0,
            "pos": {
              "x": 2,
              "y": 1
            },
            "puissance": 4,
            "tour_eclosion": 42
          }
        ],
        "carte": [
          "4332X",
          "42443",
          "312X3",
          "X4343"
        ],
        "debug": [
          [ 0, 0, 0, 0, 0 ],
          [ 0, 0, 3, 0, 0 ],
          [ 0, 0, 0, 1, 0 ],
          [ 0, 0, 2, 0, 0 ]
        ],
        "gains": [
          [ 3, 2, 4, 10 ],
          [ 0, -4, -4, 0 ],
          [ 10, 4, 2, 3 ]
        ],
        "hauteur": 4,
        "joueur1": {
          "aigles": [],
          "score": 0,
          "score_total": 0,
          "villages": [
            {
              "x": 0,
              "y": 3
            },
            {
              "x": 3,
              "y": 2
            }
          ]
        },
        "joueur2": {
          "aigles": [],
          "score": 0,
          "score_total": 0,
          "villages": [
            {
              "x": 4,
              "y": 0
            }
          ]
        },
        "largeur": 5,
        "territoire": [
          [ 0, 0, 2, 2, 0 ],
          [ 0, 0, 0, 0, 0 ],
          [ 1, 1, 1, 0, 0 ],
          [ 0, 0, 0, 0, 0 ]
        ],
        "villages_libres": []
      },
      "tour": {
        "fin": false,
        "id_tour": 0,
        "joueur_actuel": 0
      }
    }
      )"_json;
  EXPECT_EQ(json_expected, json);
}

TEST_F(ApiTestCoq, DumpSimple) {
  auto &state = api->game_state();
  const auto json = state.dump();
  const auto json_expected = R"(
      {
          "actions": [],
          "jeu": {
              "aigles": [
                  {
                      "effet": "FEU",
                      "id": 0,
                      "pos": {
                          "x": 1,
                          "y": 5
                      },
                      "puissance": 3,
                      "tour_eclosion": 10
                  },
                  {
                      "effet": "VIE",
                      "id": 1,
                      "pos": {
                          "x": 3,
                          "y": 5
                      },
                      "puissance": 10,
                      "tour_eclosion": 10
                  },
                  {
                      "effet": "VIE",
                      "id": 2,
                      "pos": {
                          "x": 5,
                          "y": 5
                      },
                      "puissance": 10,
                      "tour_eclosion": 10
                  },
                  {
                      "effet": "FEU",
                      "id": 3,
                      "pos": {
                          "x": 7,
                          "y": 5
                      },
                      "puissance": 3,
                      "tour_eclosion": 10
                  },
                  {
                      "effet": "FEU",
                      "id": 4,
                      "pos": {
                          "x": 1,
                          "y": 9
                      },
                      "puissance": 3,
                      "tour_eclosion": 10
                  },
                  {
                      "effet": "VIE",
                      "id": 5,
                      "pos": {
                          "x": 3,
                          "y": 9
                      },
                      "puissance": 10,
                      "tour_eclosion": 10
                  },
                  {
                      "effet": "VIE",
                      "id": 6,
                      "pos": {
                          "x": 5,
                          "y": 9
                      },
                      "puissance": 10,
                      "tour_eclosion": 10
                  },
                  {
                      "effet": "FEU",
                      "id": 7,
                      "pos": {
                          "x": 7,
                          "y": 9
                      },
                      "puissance": 3,
                      "tour_eclosion": 10
                  },
                  {
                      "effet": "MORT",
                      "id": 8,
                      "pos": {
                          "x": 1,
                          "y": 7
                      },
                      "puissance": 42,
                      "tour_eclosion": 10
                  },
                  {
                      "effet": "METEORE",
                      "id": 9,
                      "pos": {
                          "x": 3,
                          "y": 7
                      },
                      "puissance": 3,
                      "tour_eclosion": 10
                  },
                  {
                      "effet": "METEORE",
                      "id": 10,
                      "pos": {
                          "x": 5,
                          "y": 7
                      },
                      "puissance": 3,
                      "tour_eclosion": 10
                  },
                  {
                      "effet": "MORT",
                      "id": 11,
                      "pos": {
                          "x": 7,
                          "y": 7
                      },
                      "puissance": 42,
                      "tour_eclosion": 10
                  }
              ],
              "carte": [
                  "4444X33333",
                  "4444433333",
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
              "debug": [
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ]
              ],
              "gains": [
                  [0,    0,    0,    0,    0,    0,    0,    0,    0],
                  [0,    0,    0,    0,    0,    0,    0,    0,    0],
                  [0,    0,    0,    0,    0,    0,    0,    0,    0],
                  [10,   0,    1,    0,    1,    0,    1,    0,    10],
                  [-100, -100, -100, -100, -100, -100, -100, -100, -100],
                  [-100, 0,    -100, 0,    -100, 0,    -100, 0,    -100],
                  [-100, -100, -100, -100, -100, -100, -100, -100, -100],
                  [-100, 0,    -100, 0,    -100, 0,    -100, 0,    -100],
                  [-100, -100, -100, -100, -100, -100, -100, -100, -100],
                  [-100, 0,    -100, 0,    -100, 0,    -100, 0,    -100],
                  [-100, -100, -100, -100, -100, -100, -100, -100, -100],
                  [10,   0,    1,    0,    1,    0,    1,    0,    10],
                  [0,    0,    0,    0,    0,    0,    0,    0,    0],
                  [0,    0,    0,    0,    0,    0,    0,    0,    0],
                  [0,    0,    0,    0,    0,    0,    0,    0,    0]
              ],
              "hauteur": 16,
              "joueur1": {
                  "aigles": [],
                  "score": 0,
                  "score_total": 0,
                  "villages": [
                      {
                          "x": 4,
                          "y": 0
                      }
                  ]
              },
              "joueur2": {
                  "aigles": [],
                  "score": 0,
                  "score_total": 0,
                  "villages": [
                      {
                          "x": 5,
                          "y": 15
                      }
                  ]
              },
              "largeur": 10,
              "territoire": [
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
                  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ]
              ],
              "villages_libres": []
          },
          "tour": {
              "fin": false,
              "id_tour": 0,
              "joueur_actuel": 0
          }
      }
  )"_json;

  EXPECT_EQ(json_expected, json);
}
