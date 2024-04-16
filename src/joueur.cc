#include "joueur.hh"

int Joueur::territoire(const Carte& carte) const
{
    int largeur, hauteur;
    std::tie(largeur, hauteur) = carte.get_dimension();

    std::vector<std::vector<bool>> marques(hauteur, std::vector<bool>(largeur));
    std::vector<std::pair<int, int>> pile;
    int territoire = 0;

    // On rajoute les îles autour des bases du joueur
    for (const position& base: villages)
    {
        int x = base.colonne;
        int y = base.ligne;

        for (int dy = 0; dy <= 1; dy++)
        {
            for (int dx = 0; dx <= 1; dx++)
            {
                int rx = x + dx;
                int ry = y + dy;
                if (marques[ry][rx])
                    continue;

                if (carte.ile_presente(rx, ry))
                {
                    marques[ry][rx] = true;
                    pile.push_back({rx, ry});
                }
            }
        }
    }

    while (!pile.empty)
    {
        // DFS à pile.
        // les iles de notre territoire sont ajoutées une unique fois
        // dans la pile grâce au tableau de marques.
        territoire++;
        int x, y;
        std::tie(x, y) = pile.back();
        pile.pop_back();

        std::pair<int, int> voisins[] = {
            {x + 1, y},
            {x - 1, y},
            {x, y + 1},
            {x, y - 1}
        };

        for (const std::pair<int, int>& voisin: voisins)
        {
            int rx, ry;
            std::tie(rx, ry) = voisin;
            if (marques[ry][rx])
                continue;

            if (carte.ile_presente(rx, ry))
            {
                marques[ry][rx] = true;
                pile.push_back({rx, ry});
            }
        }
    }

    return territoire;
}
