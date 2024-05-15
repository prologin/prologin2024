#pragma once

#include <vector>
#include <string>

#include "constant.hh"

class Carte
{
public:
    Carte(const std::vector<std::string>& texte, const std::vector<std::vector<int>>& gain);
    Carte() = default;
    Carte(const Carte& carte) = default;
    Carte& operator=(const Carte& carte) = default;
    ~Carte() = default;

    [[nodiscard]] std::pair<int, int> get_dimension() const; // largeur_, hauteur_
    [[nodiscard]] type_case get_case(int x, int y) const;
    void set_case(int x, int y, type_case nouvelle_case);
    [[nodiscard]] int get_gain(int x, int y) const;
    [[nodiscard]] bool case_valide(int x, int y) const;
    [[nodiscard]] bool emplacement_valide(int x, int y) const;
    [[nodiscard]] bool ile_presente(int x, int y) const;

private:
    std::vector<std::vector<type_case>> grille_;
    int largeur_;
    int hauteur_;
    std::vector<std::vector<int>> gain_;
};
