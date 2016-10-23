#include <iostream>
#include <deque>
#include <algorithm>
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include "Graphe.h"
#include "Sommet.h"
#include "Arete.h"
#include "Appli.h"
using namespace std;

///////////////////////////////
// Exercice 1.1
///////////////////////////////
void calculer_c3(Graphe & g, Arete e){

    Sommet src = g.source(e);
    Sommet dest = g.destination(e);
    set<Sommet>_voisins_src = g.voisins(src);
    set<Sommet>_voisins_dest = g.voisins(dest);

    set<Sommet>::iterator itSrc = _voisins_src.begin();
    set<Sommet>::iterator itDest = _voisins_dest.begin();

    bool commun;
    int voisins_communs = 0;

    for(itSrc; itSrc != _voisins_src.end(); itSrc++)
    {
        commun = false;
        itDest = _voisins_dest.begin();

        while(!commun && itDest != _voisins_dest.end())
        {
            if((*itSrc).id() == (*itDest).id())
            {
                voisins_communs++;
                commun = true;
            }
            else
                itDest++;
        }
    }

    int nb_cycle = _voisins_dest.size() + _voisins_src.size() - voisins_communs -2;
    float nb =((float)(voisins_communs) / (float)(nb_cycle))*1.0;
    g.metriqueArete(e, nb);
}

///////////////////////////////
// Exercice 1.2
///////////////////////////////
void calculer_mesure(Graphe &g){
    for(Arete e : g.aretes())
    {
        calculer_c3(g, e);
    }
}

///////////////////////////////
// Exercice 2.1
///////////////////////////////
void filtrer_arete(Graphe &g, float borne){
    for(Arete e : g.aretes())
    {
        if(g.metriqueArete(e) <= borne)
        {
            g.supprimerArete(e);
        }
    }
}

///////////////////////////////
// Exercice 2.2
///////////////////////////////

void tremaux(Graphe &g, set<Sommet> &connexe, Sommet &n, set<Sommet> &nonTraite)
{
    connexe.insert(n);
    nonTraite.erase(n);
    for(auto e : g.voisins(n))
    {
        if(nonTraite.find(e) != nonTraite.end())
        {
            tremaux(g,connexe,e,nonTraite);
        }
    }
}

void calculer_composantes_connexes(Graphe &g, vector<set<Sommet> > &comp_connexes)
{
    set<Sommet> connexe;
    set<Sommet> nonTraites = g.sommets();
    Sommet n;

    while(!nonTraites.empty())
    {
        n = *nonTraites.begin();
        tremaux(g, connexe, n, nonTraites);
        comp_connexes.push_back(connexe);
        connexe.clear();
    }

}

///////////////////////////////
// Exercice 2.3
///////////////////////////////
void trouver_groupes(Graphe g, float borne, vector<set<Sommet> > & groupes)
{
    calculer_mesure(g);
    filtrer_arete(g, borne);
    calculer_composantes_connexes(g, groupes);
}

///////////////////////////////
// Exercice 3.1.1
///////////////////////////////
float densite_intra(Graphe &g, set<Sommet> & c)
{

    float nb_total_aretes;
    float arete_communaute = 0;

    if(c.size() == 1)
        nb_total_aretes = 1;
    else
        nb_total_aretes = c.size()*(c.size()-1)/2;

    for(Sommet s : c)
    {
        for(Sommet som : g.voisins(s))
        {
            if(c.find(som) != c.end())
            {
                arete_communaute++;
            }
        }
    }

    arete_communaute/=2;
    return (arete_communaute / nb_total_aretes);

}

///////////////////////////////
// Exercice 3.1.2
///////////////////////////////
float densite_intra(Graphe &g, vector<set<Sommet> >& communautes)
{
    float densite = 0;
    for(set<Sommet> s : communautes)
    {
        densite += densite_intra(g, s);
    }
    return (densite / communautes.size());
}

///////////////////////////////
// Exercice 3.2.1
///////////////////////////////
float densite_inter(Graphe &g, set<Sommet> &c_i, set<Sommet> c_j)
{
    float arete_inter = 0;
    float tot_arete = c_j.size() * c_i.size();

    for(Sommet s : c_i)
    {
        for(auto e : g.voisins(s))
        {
            if(c_j.find(e) != c_j.end())
            {
                arete_inter++;
            }
        }
    }
    return (arete_inter/tot_arete);
}

///////////////////////////////
// Exercice 3.2.2
///////////////////////////////
float densite_inter(Graphe &g, vector<set<Sommet> >& communautes)
{
    if(communautes.size() == 1)
        return 1;
    else {

        float densite = 0;
        for(int i=0; i< communautes.size() -1 ; i++)
        {
            for(int j = i+1; j<communautes.size() ; j++)
            {
                densite += densite_inter(g, communautes[i], communautes[j]);
            }
        }

        densite /=2;
        float k_inter = (communautes.size() * (communautes.size() -1)) / 2;
        return (densite / k_inter);
    }
}

///////////////////////////////
// Exercice 3.3.1
///////////////////////////////
float evaluer_qualite(Graphe g, vector<set<Sommet> > & comp_connexes)
{
    return (densite_intra(g,comp_connexes) - densite_inter(g, comp_connexes));
}

///////////////////////////////
// Exercice 3.3.2
///////////////////////////////
float trouver_meilleure_borne(Graphe &g, float pas = 0.1f)
{
    float borne, qualite;
    float best = 0.f;
    vector<set<Sommet> > groupes;
    for(float i = 0; i< 1 ; i+=pas)
    {
        trouver_groupes(g,i,groupes);
        qualite = evaluer_qualite(g,groupes);

        if(qualite > best)
        {
            best = qualite;
            borne = i;
        }
        groupes.clear();
    }
    return borne;
}

///////////////////////////////
// Exercice 4
///////////////////////////////
void colorier_sommets(Graphe & g, vector<set<Sommet> > & comp_connexes)
{
    Couleur c1{244,19,19, 60};
    Couleur c2{27,21,238, 60};
    Couleur c3{22,162,7, 60};
    Couleur c4{244,244,7, 60};
    Couleur c5{27,238,238, 60};
    Couleur c6{162,14,7, 162};

    Couleur c;

    for(int i=0; i< comp_connexes.size() ; i++)
    {
        if(i%6==0)
            c = c1;
        else if(i%6==1)
            c = c2;
        else if(i%6==2)
            c= c3;
        else if(i%6==3)
            c = c4;
        else if(i%6==4)
            c = c5;
        else if(i%6==5)
            c= c6;

        for(auto e : comp_connexes[i])
        {            
            g.couleurSommet(e,c);
        }
    }
}


int main()
{
    // chargement et création des graphe et fenetre d'affichage
    string fichier = "graphe.txt";
    Graphe g;

    if(!g.charger(fichier)){
        cout << "Erreur de chargement de fichier" << endl;
        return 1;
    }

    ////////////////////////////////
    // Exercice 5
    ///////////////////////////////
    vector<set<Sommet> > communaute;
    float borne = trouver_meilleure_borne(g);
    trouver_groupes(g, borne, communaute);
    colorier_sommets(g, communaute);

    cout << "MEILLEURE BORNE = " << borne << endl;
    cout << "NB COMMUNAUTE = " << communaute.size() << endl;

    ////////////////////////////////

    Appli app;
    app.setGraphe(g);
    app.run();

    return 0;
}

