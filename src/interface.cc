// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include <iostream>
#include <sstream>
#include <vector>

#include "api.hh"

// from api.cc
extern Api* api;

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& arr)
{
    os << "[";
    typename std::vector<T>::const_iterator it;
    for (it = arr.begin(); it != arr.end(); ++it)
    {
        if (it != arr.begin())
            os << ", ";
        os << *it;
    }
    os << "]";
    return os;
}


extern "C" erreur api_tourner_case(position pos)
{
    return api->tourner_case(pos);
}

extern "C" erreur api_activer_aigle(int id)
{
    return api->activer_aigle(id);
}

extern "C" erreur api_deplacer_aigle(int id, position destination)
{
    return api->deplacer_aigle(id, destination);
}

extern "C" dimension api_dimensions_carte()
{
    return api->dimensions_carte();
}

extern "C" etat_case api_info_case(position pos)
{
    return api->info_case(pos);
}

extern "C" std::vector<aigle> api_info_aigles()
{
    return api->info_aigles();
}

extern "C" std::vector<position> api_liste_villages(int joueur)
{
    return api->liste_villages(joueur);
}

extern "C" int api_points_action(int joueur)
{
    return api->points_action(joueur);
}

extern "C" int api_score(int joueur)
{
    return api->score(joueur);
}

extern "C" erreur api_debug_poser_drakkar(position pos, drakkar_debug drakkar)
{
    return api->debug_poser_drakkar(pos, drakkar);
}

extern "C" std::vector<action_hist> api_historique()
{
    return api->historique();
}

extern "C" std::vector<position> api_recuperer_territoire(int joueur)
{
    return api->recuperer_territoire(joueur);
}

extern "C" bool api_case_dans_rayon(int id)
{
    return api->case_dans_rayon(id);
}

extern "C" int api_moi()
{
    return api->moi();
}

extern "C" int api_adversaire()
{
    return api->adversaire();
}

extern "C" bool api_annuler()
{
    return api->annuler();
}

extern "C" int api_tour_actuel()
{
    return api->tour_actuel();
}

extern "C" void api_afficher_erreur(erreur v)
{
    api->afficher_erreur(v);
}

extern "C" void api_afficher_type_case(type_case v)
{
    api->afficher_type_case(v);
}

extern "C" void api_afficher_drakkar_debug(drakkar_debug v)
{
    api->afficher_drakkar_debug(v);
}

extern "C" void api_afficher_type_action(type_action v)
{
    api->afficher_type_action(v);
}

extern "C" void api_afficher_effet_aigle(effet_aigle v)
{
    api->afficher_effet_aigle(v);
}

extern "C" void api_afficher_position(position v)
{
    api->afficher_position(v);
}

extern "C" void api_afficher_dimension(dimension v)
{
    api->afficher_dimension(v);
}

extern "C" void api_afficher_aigle(aigle v)
{
    api->afficher_aigle(v);
}

extern "C" void api_afficher_etat_case(etat_case v)
{
    api->afficher_etat_case(v);
}

extern "C" void api_afficher_action_hist(action_hist v)
{
    api->afficher_action_hist(v);
}

std::ostream& operator<<(std::ostream& os, erreur v)
{
    switch (v)
    {
    case OK:
        os << "OK";
        break;
    case HORS_TOUR:
        os << "HORS_TOUR";
        break;
    case CASE_BLOQUEE:
        os << "CASE_BLOQUEE";
        break;
    case POSITION_INVALIDE:
        os << "POSITION_INVALIDE";
        break;
    case DESTINATION_INVALIDE:
        os << "DESTINATION_INVALIDE";
        break;
    case PLUS_DE_PA:
        os << "PLUS_DE_PA";
        break;
    case AIGLE_INVALIDE:
        os << "AIGLE_INVALIDE";
        break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, type_case v)
{
    switch (v)
    {
    case VILLAGE:
        os << "VILLAGE";
        break;
    case NORD_EST:
        os << "NORD_EST";
        break;
    case NORD_OUEST:
        os << "NORD_OUEST";
        break;
    case SUD_OUEST:
        os << "SUD_OUEST";
        break;
    case SUD_EST:
        os << "SUD_EST";
        break;
    case CASE_INVALIDE:
        os << "CASE_INVALIDE";
        break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, drakkar_debug v)
{
    switch (v)
    {
    case PAS_DE_DRAKKAR:
        os << "PAS_DE_DRAKKAR";
        break;
    case DRAKKAR_BLEU:
        os << "DRAKKAR_BLEU";
        break;
    case DRAKKAR_JAUNE:
        os << "DRAKKAR_JAUNE";
        break;
    case DRAKKAR_ROUGE:
        os << "DRAKKAR_ROUGE";
        break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, type_action v)
{
    switch (v)
    {
    case ACTION_TOURNER_CASE:
        os << "ACTION_TOURNER_CASE";
        break;
    case ACTION_ACTIVER_AIGLE:
        os << "ACTION_ACTIVER_AIGLE";
        break;
    case ACTION_DEPLACER_AIGLE:
        os << "ACTION_DEPLACER_AIGLE";
        break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, effet_aigle v)
{
    switch (v)
    {
    case EFFET_METEORE:
        os << "EFFET_METEORE";
        break;
    case EFFET_VIE:
        os << "EFFET_VIE";
        break;
    case EFFET_MORT:
        os << "EFFET_MORT";
        break;
    case EFFET_FEU:
        os << "EFFET_FEU";
        break;
    case EFFET_GEL:
        os << "EFFET_GEL";
        break;
    }
    return os;
}


std::ostream& operator<<(std::ostream& os, position v)
{
    os << "{ ";
    os << "colonne"
       << "=" << v.colonne;
    os << ", ";
    os << "ligne"
       << "=" << v.ligne;
    os << " }";
    return os;
}

std::ostream& operator<<(std::ostream& os, dimension v)
{
    os << "{ ";
    os << "hauteur"
       << "=" << v.hauteur;
    os << ", ";
    os << "largeur"
       << "=" << v.largeur;
    os << " }";
    return os;
}

std::ostream& operator<<(std::ostream& os, aigle v)
{
    os << "{ ";
    os << "identifiant"
       << "=" << v.identifiant;
    os << ", ";
    os << "joueur"
       << "=" << v.joueur;
    os << ", ";
    os << "pos"
       << "=" << v.pos;
    os << ", ";
    os << "effet"
       << "=" << v.effet;
    os << ", ";
    os << "puissance"
       << "=" << v.puissance;
    os << ", ";
    os << "tour_eclosion"
       << "=" << v.tour_eclosion;
    os << ", ";
    os << "existant"
       << "=" << v.existant;
    os << " }";
    return os;
}

std::ostream& operator<<(std::ostream& os, etat_case v)
{
    os << "{ ";
    os << "contenu"
       << "=" << v.contenu;
    os << ", ";
    os << "points"
       << "=" << v.points;
    os << ", ";
    os << "pos_case"
       << "=" << v.pos_case;
    os << " }";
    return os;
}

std::ostream& operator<<(std::ostream& os, action_hist v)
{
    os << "{ ";
    os << "action_type"
       << "=" << v.action_type;
    os << ", ";
    os << "debut"
       << "=" << v.debut;
    os << ", ";
    os << "fin"
       << "=" << v.fin;
    os << ", ";
    os << "identifiant_aigle"
       << "=" << v.identifiant_aigle;
    os << " }";
    return os;
}
