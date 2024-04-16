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

    std::pair<int, int> get_dimension() const;
    type_case get_case(int x, int y) const;
    void set_case(int x, int y, type_case nouvelle_case);
    int get_gain(int x, int y) const;
    bool case_valide(int x, int y) const;
    bool emplacement_valide(int x, int y) const;
    bool ile_presente(int x, int y) const;

private:
    std::vector<std::vector<type_case>> grille_;
    int largeur_;
    int hauteur_;
    std::vector<std::vector<int>> gain_;
};
