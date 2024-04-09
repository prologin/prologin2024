#include <src/carte.hh>

Carte::Carte(const std::vector<std::string>& texte, const std::vector<std::vector<int>>& gain)
    : largeur_{ texte[0].size() },
    hauteur_{ texte.size() },
    gain_{ gain }
{
    grille_.resize(hauteur_, vector<int>(largeur_));
    for (int y = 0; y < hauteur_; y++)
    {
        for (int x = 0; x < largeur_; x++)
        {
            switch (texte[y][x])
            {
                case 'X':
                    grille_[y][x] = VILLAGE_NEUTRE;
                    break;
                case '1':
                    grille_[y][x] = NORD_OUEST;
                    break;
                case '2':
                    grille_[y][x] = NORD_EST;
                    break;
                case '3':
                    grille_[y][x] = SUD_OUEST;
                    break;
                case '4':
                    grille_[y][x] = SUD_EST;
                    break;
            }
        }
    }
}
std::Pair<int, int> Carte::get_dimension()
{
    return {this->largeur_, this->hauteur_};
}

Case Carte::get_case(int x, int y)
{
    if (!case_valide(x, y))
        return CASE_INVALIDE;
    else
        return this->grille_[y][x];
}

int get_gain(int x, int y)
{
    if (!emplacement_valide(x, y))
        return 0;
    else
        return this->gain_[y][x];
}

bool case_valide(int x, int y)
{
    return x >= 0 && x < this->largeur_ && y >= 0 && y < this->hauteur_;
}
bool emplacement_valide(int x, int y)
{
    return x >= 0 && x < this->largeur_ - 1 && y >= 0 && y < this->hauteur_ - 1;
}
bool ile_presente(int x, int y)
{
    if (!emplacement_valide(x, y))
        return false;
    return get_case(x, y) != SUD_EST && get_case(x + 1, y) != SUD_OUEST
        && get_case(x, y + 1) != NORD_EST && get_case(x + 1, y + 1) != NORD_OUEST;
}


