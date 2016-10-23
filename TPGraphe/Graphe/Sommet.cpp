#include "Sommet.h"

int Sommet::m_curId = 0;

Sommet::Sommet(bool valide) : m_id(valide ? m_curId++ : -1)
{}

int Sommet::id() const {
    return m_id;
}


bool Sommet::valide() const{
    return (m_id != -1);
}

bool Sommet::operator<(const Sommet &n) const{
    return m_id < n.m_id;
}

bool Sommet::operator==(const Sommet &n) const{
    return m_id == n.m_id;
}
