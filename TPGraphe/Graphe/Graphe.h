#ifndef GRAPHE_H
#define GRAPHE_H
#include <map>
#include <set>
#include <string>

#include "Sommet.h"
#include "Arete.h"
#include "Coord.h"
#include "Couleur.h"
#include "Propriete.hpp"

const Couleur COULEUR_DEFAUT = Couleur{255,0,0,255};
const std::string ETIQUETTE_DEFAUT = "";
const float METRIQUE_DEFAUT = 0.f;

class Graphe
{
private :
    std::set<Sommet> m_sommets;
    std::set<Arete> m_aretes;

    std::map<Sommet, std::set<Arete> > m_incidentes;
    std::map<Arete, std::pair<Sommet,Sommet> > m_extremites;

    // Proprietes
    Propriete<Couleur> m_couleurs;
    Propriete<Coord> m_positions;
    Propriete<std::string> m_labels;
    Propriete<float> m_metrique;

public:
    Graphe();

    bool charger(std::string fichier);

    Sommet ajouterSommet();
    Arete ajouterArete(const Sommet &n1, const Sommet & n2);

    void supprimerSommet(const Sommet &n);
    void supprimerArete(const Arete &e);

    std::set<Sommet> sommets() const;
    std::set<Arete> aretes() const;

    std::set<Sommet> voisins(Sommet n) const;
    std::set<Arete> incidentes(Sommet n) const;

    Sommet source(const Arete &e) const;
    Sommet destination(const Arete &e) const;

    int degre(const Sommet &n) const;

    int nbSommets() const;
    int nbAretes() const;

    // Position
    void positionSommet(Sommet n, Coord c);
    Coord positionSommet(Sommet n) const;

    // Couleur
    void couleurSommet(Sommet n, Couleur c);
    Couleur couleurSommet(Sommet n);
    void couleurArete(Arete e, Couleur c);
    Couleur couleurArete(Arete e);

    // Etiquette
    void etiquetteSommet(Sommet n, std::string etiquette);
    std::string etiquetteSommet(Sommet n) const;
    void etiquetteArete(Arete e, std::string etiquette);
    std::string etiquetteArete(Arete e) const;

    // Etiquette
    void metriqueSommet(Sommet n, float valeur);
    float metriqueSommet(Sommet n) const;
    void metriqueArete(Arete e, float valeur);
    float metriqueArete(Arete e) const;

    Sommet sommet(const Coord &c, int rayon);
};

#endif // GRAPHE_H
