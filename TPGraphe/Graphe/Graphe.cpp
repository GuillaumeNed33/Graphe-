#include <fstream>
#include <assert.h>
#include <iostream>
#include <vector>

#include "Graphe.h"
using namespace std;


void decouper(const string& str, vector<string>& tokens, const string& delimiters = " ") {
    if (str.length() == 0) return;
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    string::size_type pos     = str.find_first_of(delimiters, lastPos);
    while (string::npos != pos || string::npos != lastPos) {
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        string::size_type pos_tmp = str.find_first_of(delimiters, pos+1);
        if(pos_tmp - pos == 1){
            lastPos = pos_tmp;
            pos = pos_tmp;
        }
        else {
            lastPos = str.find_first_not_of(delimiters, pos);
            pos = str.find_first_of(delimiters, lastPos);
        }
    }
}


Graphe::Graphe() {}

/*

    print #sommets;position;couleur

    for n in graph.getNodes():
        print str(n.id) + ";" + str(viewLayout[n][0]) + " " + str(viewLayout[n][1]) + ";" + str(viewColor[n][0]) + " "+ str(viewColor[n][1]) + " " +str(viewColor[n][2])

    print "#aretes"

    for e in graph.getEdges():
        print str(graph.source(e).id) + " " + str(graph.target(e).id)
*/

bool Graphe::charger(std::string fichier) {
    map<unsigned int , Sommet> idSommet;

    // Ajout des sommets
    bool stop = false;
    std::ifstream in(fichier.c_str());
    while (!in.eof() && !stop) {
        char ligne[10000];
        in.getline(ligne, 10000);
        string lignes(ligne);
        //cout << "1 : " << lignes << endl;
        if(lignes == "#sommet;position;couleur;etiquette")
            continue;
        else if(lignes == "#aretes"){
            stop = true;
        }
        else {
            vector<string> items;
            decouper(lignes, items, ";");

            if(items.size() != 4) return false;

            int id = stoi(items[0].c_str());
            if(idSommet.find(id) != idSommet.end())
                return false; // deux fois le meme identifiant dans le fichier
            Sommet n = ajouterSommet();
            idSommet[id] = n;

            // position
            string position = items[1];
            vector<string> positions;
            decouper(position, positions, " ");
            if(positions.size() != 2)
                return false;
            Coord pos {stof(positions[0]), stof(positions[1])};
            positionSommet(n, pos);

            // couleur
            string couleur = items[2];
            vector<string> couleurs;
            decouper(couleur, couleurs, " ");
            if(couleurs.size() != 4) return false;

            Couleur col {stoi(couleurs[0]), stoi(couleurs[1]),
                        stoi(couleurs[2]), stoi(couleurs[3])};
            couleurSommet(n, col);

            // etiquette
            etiquetteSommet(n, items[3]);
        }
    }
    // Ajout des aretes
    while (!in.eof()) {
        char ligne[10000];
        in.getline(ligne, 10000);
        string lignes(ligne);
        //cout << "2 : " << lignes << endl;
        if(lignes != "") {// derniere ligne "vide"
            vector<string> items;
            decouper (lignes, items, " ");
            if(items.size() != 2) return false;

            int id1 = stoi(items[0]);
            int id2 = stoi(items[1]);

            // test si les id sont corrects
            if(idSommet.find(id1) == idSommet.end() || idSommet.find(id2)==idSommet.end())
                return false;
            Sommet n1 = idSommet[id1];
            Sommet n2 = idSommet[id2];
            ajouterArete(n1,n2);
        }
    }

    return true;
}


Sommet Graphe::ajouterSommet(){
    Sommet n;
    m_sommets.insert(n);
    m_positions.changer(n, Coord{});
    m_couleurs.changer(n,COULEUR_DEFAUT);
    m_labels.changer(n,ETIQUETTE_DEFAUT);
    m_metrique.changer(n, METRIQUE_DEFAUT);

    return n;
}

void Graphe::supprimerSommet(const Sommet &n){

    // Suppression des aretes incidentes
    assert(m_incidentes.find(n) != m_incidentes.end());
    for(auto e : m_incidentes.at(n))
        supprimerArete(e);
    m_incidentes.erase(m_incidentes.find(n));

    // Suppresion des sommets
    assert(m_sommets.find(n) != m_sommets.end());
    m_sommets.erase(m_sommets.find(n));

    // MAJ des propriétés
    m_positions.supprimer(n);
    m_couleurs.supprimer(n);
    m_labels.supprimer(n);
    m_metrique.supprimer(n);

}

void Graphe::supprimerArete(const Arete &e){
    assert(m_aretes.find(e) != m_aretes.end());
    assert(m_extremites.find(e) != m_extremites.end());

    Sommet src = source(e);
    Sommet dest = destination(e);

    m_aretes.erase(m_aretes.find(e));
    m_extremites.erase(m_extremites.find(e));

    assert(m_incidentes.find(src) != m_incidentes.end());
    assert(m_incidentes.find(dest) != m_incidentes.end());

    set<Arete> & src_incidentes = m_incidentes[src];
    assert(src_incidentes.find(e) != src_incidentes.end());
    src_incidentes.erase(src_incidentes.find(e));

    set<Arete> & dest_incidentes = m_incidentes[dest];
    assert(dest_incidentes.find(e) != dest_incidentes.end());
    dest_incidentes.erase(dest_incidentes.find(e));

    m_couleurs.supprimer(e);
    m_labels.supprimer(e);
    m_metrique.supprimer(e);
}

set<Sommet> Graphe::sommets() const {
    return m_sommets;
}

set<Arete> Graphe::aretes() const {
    return m_aretes;
}

set<Sommet> Graphe::voisins(Sommet n) const{
    assert(m_incidentes.find(n) != m_incidentes.end());
    set<Sommet> voisinage;
    for(auto e : m_incidentes.at(n)){
        voisinage.insert(source(e).id() == n.id() ? destination(e) : source(e));
    }
    return voisinage;
}

set<Arete> Graphe::incidentes(Sommet n) const{
    assert(m_incidentes.find(n) != m_incidentes.end());
    return m_incidentes.at(n);
}

Arete Graphe::ajouterArete(const Sommet &n1, const Sommet & n2) {
    assert(m_sommets.find(n1) != m_sommets.end()
            && m_sommets.find(n2) != m_sommets.end());
    Arete e;
    m_aretes.insert(e);
    m_extremites[e] = pair<Sommet, Sommet> (n1, n2);
    m_incidentes[n1].insert(e);
    m_incidentes[n2].insert(e);
    m_couleurs.changer(e, COULEUR_DEFAUT);
    m_labels.changer(e, ETIQUETTE_DEFAUT);
    m_metrique.changer(e,METRIQUE_DEFAUT);
    return e;
}

Sommet Graphe::source(const Arete &e) const {
    assert(m_aretes.find(e) != m_aretes.end());
    return m_extremites.at(e).first;
}

Sommet Graphe::destination(const Arete &e) const {
    assert(m_aretes.find(e) != m_aretes.end());
    return m_extremites.at(e).second;
}

int Graphe::degre(const Sommet &n) const {
    assert(m_sommets.find(n) != m_sommets.end());
    return m_incidentes.at(n).size();
}

int Graphe::nbSommets() const {
    return m_sommets.size();
}

int Graphe::nbAretes() const{
    return m_aretes.size();
}

// Position
void Graphe::positionSommet(Sommet n, Coord c){
    m_positions.changer(n,c);
}

Coord Graphe::positionSommet(Sommet n) const{
    return m_positions.valeur(n);
}

// Couleur
void Graphe::couleurSommet(Sommet n, Couleur c){
    m_couleurs.changer(n,c);
}

Couleur Graphe::couleurSommet(Sommet n){
    return m_couleurs.valeur(n);
}

void Graphe::couleurArete(Arete e, Couleur c){
    m_couleurs.changer(e,c);
}

Couleur Graphe::couleurArete(Arete e){
    return m_couleurs.valeur(e);
}

// Etiquette
void Graphe::etiquetteSommet(Sommet n, std::string etiquette){
    m_labels.changer(n,etiquette);
}

string Graphe::etiquetteSommet(Sommet n) const{
    return m_labels.valeur(n);
}

void Graphe::etiquetteArete(Arete e, std::string etiquette){
    m_labels.changer(e,etiquette);
}

string Graphe::etiquetteArete(Arete e) const{
    return m_labels.valeur(e);
}

// Métrique
void Graphe::metriqueSommet(Sommet n, float valeur){
    m_metrique.changer(n,valeur);
}

float Graphe::metriqueSommet(Sommet n) const{
    return m_metrique.valeur(n);
}

void Graphe::metriqueArete(Arete e, float valeur){
    m_metrique.changer(e,valeur);
}

float Graphe::metriqueArete(Arete e) const{
    return m_metrique.valeur(e);
}


Sommet Graphe::sommet(const Coord &c, int rayon) {
    Sommet resultat = Sommet(false);
    for(auto n : m_sommets){
        Coord coord = m_positions.valeur(n);
        float dist = (c.m_x-coord.m_x)*(c.m_x-coord.m_x) + (c.m_y-coord.m_y)*(c.m_y-coord.m_y);
        if(dist < rayon * rayon){
            resultat = n;
        }
    }
    return resultat;
}
