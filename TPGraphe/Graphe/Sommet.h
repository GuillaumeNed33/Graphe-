#ifndef NOEUD_H
#define NOEUD_H

class Sommet
{
private :
    static int m_curId;
    int m_id;
public:
    Sommet(bool valide = true);

    int id() const;

    bool valide() const;
    bool operator<(const Sommet &n) const;
    bool operator==(const Sommet &n) const;
};

#endif // NOEUD_H
