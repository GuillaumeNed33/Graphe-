#ifndef COULEUR_H
#define COULEUR_H

class Couleur
{
private :
    int m_canaux [4];
public:
    Couleur();
    Couleur(int r, int g, int b, int a);

    Couleur interpoler(const Couleur &c, float valeur);
    const int& operator[](int canal) const;
    int& operator[](int canal) ;
    bool operator==(const Couleur & c) const;
};

#endif // COULEUR_H
