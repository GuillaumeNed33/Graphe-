#include <assert.h>

#include "Couleur.h"

Couleur::Couleur() : Couleur {0,0,0, 255} {}

Couleur::Couleur(int r, int g, int b, int a) {
    m_canaux[0] = r;
    m_canaux[1] = g;
    m_canaux[2] = b;
    m_canaux[3] = a;
}

Couleur Couleur::interpoler(const Couleur &c, float valeur){
    int r = m_canaux[0] + (c.m_canaux[0]-m_canaux[0])*valeur;
    int g = m_canaux[1] + (c.m_canaux[1]-m_canaux[1])*valeur;
    int b = m_canaux[2] + (c.m_canaux[2]-m_canaux[2])*valeur;
    int a = m_canaux[3] + (c.m_canaux[3]-m_canaux[3])*valeur;
    return Couleur{r,g,b,a};

}

const int& Couleur::operator[](int canal) const {
    assert(canal>= 0 && canal <= 3);
    return m_canaux[canal];
}
int& Couleur::operator[](int canal) {
    assert(canal>= 0 && canal <= 3);
    return m_canaux[canal];
}

bool Couleur::operator==(const Couleur & c) const{
    return m_canaux[0]==c.m_canaux[0] &&
            m_canaux[1]==c.m_canaux[1] &&
            m_canaux[2]==c.m_canaux[2] &&
            m_canaux[3]==c.m_canaux[3];

}
