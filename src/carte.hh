#pragma once

#include <vector>
#include <string>

#include "constant.hh"

class Carte
{
public:
    // TODO: demander a un ACU
    Carte(const std::vector<std::string>& texte, const std::vector<std::vector<int>>& gain);
    Carte() = default;
    Carte(const Carte& carte) = default;
    Carte& operator=(const Carte& carte) = default;
    ~Carte() = default;

    std::pair<int, int> get_dimension();
    type_case get_case(int x, int y);
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
