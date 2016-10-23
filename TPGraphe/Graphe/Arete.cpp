#include "Arete.h"

int Arete::m_curId = 0;

Arete::Arete(bool valide) : m_id(valide ? m_curId++ : -1)
{}

int Arete::id() const {
    return m_id;
}

bool Arete::valide() const{
    return (m_id != -1);
}

bool Arete::operator<(const Arete &e) const {
    return m_id < e.m_id;
}

bool Arete::operator==(const Arete &e) const{
    return m_id == e.m_id;
}
