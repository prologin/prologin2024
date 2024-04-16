#pragma once

#include <vector>

#include "constant.hh"

class Carte
{
public:
    Carte() = default;
    Carte(const Carte& carte) = default;
    ~Carte() = default;
    Carte(const std::vector<std::string>& texte, const std::vector<std::vector<int>>& gain);
    std::pair<int, int> get_dimension();
    type_case get_case(int x, int y);
    void set_case(int x, int y, type_case nouvelle_case);
    int get_gain(int x, int y);
    bool case_valide(int x, int y);
    bool emplacement_valide(int x, int y);
    bool ile_presente(int x, int y);

private:
    std::vector<std::vector<type_case>> grille_;
    int largeur_;
    int hauteur_;
    std::vector<std::vector<int>> gain_;
};
