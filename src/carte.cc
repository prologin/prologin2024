#include "carte.hh"

Carte::Carte(const std::vector<std::string>& texte, const std::vector<std::vector<int>>& gain)
    : largeur_{ static_cast<int>(texte[0].size()) },
    hauteur_{ static_cast<int>(texte.size()) },
    gain_{ gain }
{
    grille_.resize(hauteur_, std::vector<type_case>(largeur_));
    for (int y = 0; y < hauteur_; y++)
    {
        for (int x = 0; x < largeur_; x++)
        {
            switch (texte[y][x])
            {
                case 'X':
                    grille_[y][x] = VILLAGE;
                    break;
                case '1':
                    grille_[y][x] = NORD_EST;
                    break;
                case '2':
                    grille_[y][x] = NORD_OUEST;
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

std::pair<int, int> Carte::get_dimension() const
{
    return {largeur_, hauteur_};
}

type_case Carte::get_case(int x, int y) const
{
    if (!case_valide(x, y))
        return CASE_INVALIDE;
    else
        return grille_[y][x];
}

void Carte::set_case(int x, int y, type_case nouvelle_case)
{
    if (!case_valide(x, y))
        return;
    grille_[y][x] = nouvelle_case;
}

int Carte::get_gain(int x, int y) const
{
    if (!emplacement_valide(x, y))
        return 0;
    else
        return gain_[y][x];
}

bool Carte::case_valide(int x, int y) const
{
    return x >= 0 && x < largeur_ && y >= 0 && y < hauteur_;
}

bool Carte::emplacement_valide(int x, int y) const
{
    return x >= 0 && x < largeur_ - 1 && y >= 0 && y < hauteur_ - 1;
}

bool Carte::ile_presente(int x, int y) const
{
    if (!emplacement_valide(x, y))
        return false;
    return get_case(x, y) != SUD_EST && get_case(x + 1, y) != SUD_OUEST
        && get_case(x, y + 1) != NORD_EST && get_case(x + 1, y + 1) != NORD_OUEST;
}


