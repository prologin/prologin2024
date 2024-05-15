#include <tuple>
#include <algorithm>
#include <utility>

#include "joueur.hh"

Joueur::Joueur(int sc , int pa, std::vector<Aigle> aigles_vec,
    std::vector<position> villages_vec)
    : score{sc},
    points_action{pa},
    aigles{std::move(aigles_vec)},
    villages{std::move(villages_vec)}
{}

std::vector<std::vector<bool>> Joueur::territoire(const Carte& carte) const
{
    const auto [largeur, hauteur] = carte.get_dimension();

    std::vector<std::vector<bool>> territoire(hauteur - 1, std::vector<bool>(largeur - 1));
    std::vector<std::pair<int, int>> pile;

    // On rajoute les îles autour des bases du joueur
    for (const position& base: villages)
    {
        int x = base.colonne;
        int y = base.ligne;

        for (int dy = -1; dy <= 0; dy++)
        {
            for (int dx = -1; dx <= 0; dx++)
            {
                int rx = x + dx;
                int ry = y + dy;
                if (!carte.emplacement_valide(rx, ry))
                    continue;

                if (territoire[ry][rx])
                    continue;

                if (carte.ile_presente(rx, ry))
                {
                    territoire[ry][rx] = true;
                    pile.emplace_back(rx, ry);
                }
            }
        }
    }

    while (!pile.empty())
    {
        // DFS à pile.
        // les iles de notre territoire sont ajoutées une unique fois
        // dans la pile grâce au tableau de marques.
        const auto [x, y] = pile.back();
        pile.pop_back();

        std::pair<int, int> voisins[] = {
            {x + 1, y},
            {x - 1, y},
            {x, y + 1},
            {x, y - 1}
        };

        for (const std::pair<int, int>& voisin: voisins)
        {
            const auto [rx, ry] = voisin;
            if (!carte.emplacement_valide(rx, ry))
                continue;

            if (territoire[ry][rx])
                continue;

            if (carte.ile_presente(rx, ry))
            {
                territoire[ry][rx] = true;
                pile.emplace_back(rx, ry);
            }
        }
    }

    return territoire;
}

std::vector<Aigle>::iterator Joueur::trouve_aigle(int aigle_id)
{
    const auto aigle_correct = [aigle_id](Aigle aigle) {
        return aigle.identifiant == aigle_id;
    };
    const auto it = std::find_if(aigles.begin(), aigles.end(), aigle_correct);
    return it;
}

std::vector<Aigle>::const_iterator Joueur::trouve_aigle(int aigle_id) const
{
    const auto aigle_correct = [aigle_id](Aigle aigle) {
        return aigle.identifiant == aigle_id;
    };

    const auto it = std::find_if(aigles.begin(), aigles.end(), aigle_correct);
    return it;
}
