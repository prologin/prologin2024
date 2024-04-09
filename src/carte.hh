#pragma once

#include <vector>

#include "case.hh"

class Carte
{
public:
    Carte(const std::vector<std::string>& texte, const std::vector<std::vector<int>>& gain);
    std::pair<int, int> get_dimension();
    Case get_case(int x, int y);
    int get_gain(int x, int y);
    bool case_valide(int x, int y);
    bool emplacement_valide(int x, int y);
    bool ile_presente(int x, int y);

private:
    std::vector<std::vector<Case>> grille_;
    int largeur_;
    int hauteur_;
    std::vector<std::vector<int>> gain_;
};
