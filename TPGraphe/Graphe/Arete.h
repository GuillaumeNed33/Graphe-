#ifndef ARETE_H
#define ARETE_H

class Arete
{
private :
    static int m_curId;
    int m_id;
public:

    Arete(bool valide = true);
    int id() const;
    bool valide() const;
    bool operator<(const Arete &e) const;
    bool operator==(const Arete &n) const;
};

#endif // ARETE_H
