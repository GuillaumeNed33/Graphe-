#ifndef PROPRIETE_H
#define PROPRIETE_H
#include <map>
#include "Sommet.h"
#include "Arete.h"

template<typename T>
class Propriete
{
private:
    std::map<Sommet, T> m_valeur_sommets;
    std::map<Arete, T> m_valeur_aretes;

public:
    Propriete() = default;

    void changer(Sommet n, const T &val);
    void changer(Arete e, const T &val);

    const T & valeur(Sommet n) const;
    const T & valeur(Arete e) const;

    void supprimer(Sommet n);
    void supprimer(Arete e);
};

template<typename T>
void Propriete<T>::changer(Sommet n, const T &val){
    m_valeur_sommets[n] = val;
}

template<typename T>
void Propriete<T>::changer(Arete e, const T &val){
    m_valeur_aretes[e] = val;
}

template<typename T>
const T & Propriete<T>::valeur(Sommet n) const{
    assert(m_valeur_sommets.find(n) != m_valeur_sommets.end());
   return m_valeur_sommets.at(n);
}

template<typename T>
const T & Propriete<T>::valeur(Arete e) const{
   assert(m_valeur_aretes.find(e) != m_valeur_aretes.end());
   return m_valeur_aretes.at(e);
}

template<typename T>
void Propriete<T>::supprimer(Sommet n) {
    assert(m_valeur_sommets.find(n) != m_valeur_sommets.end());
    m_valeur_sommets.erase(m_valeur_sommets.find(n));
}

template<typename T>
void Propriete<T>::supprimer(Arete e) {
    assert(m_valeur_aretes.find(e) != m_valeur_aretes.end());
    m_valeur_aretes.erase(m_valeur_aretes.find(e));
}

#endif // PROPRIETE_H
