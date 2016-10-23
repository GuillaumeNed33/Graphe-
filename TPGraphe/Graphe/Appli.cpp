#include <set>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "Appli.h"
#include "Coord.h"
#include "Couleur.h"
using namespace std;
using namespace sf;


/*
 constantes et fonction auxiliaires
 pour l'implémentation de Appli
*/

const float Appli::RAYON          = 2.0;
const string Appli::FICHIER_FONT = "FreeSans.ttf";

// ---------------------------------------

Appli::Appli()
    : m_fenetre {sf::VideoMode {800, 600, 32},
                 "Exemple 2",
                 sf::Style::Close
                 }
    , m_sommet{RAYON}, m_montre_etiquette{false}

{
    m_fenetre.setFramerateLimit(50);

    m_font.loadFromFile(FICHIER_FONT);
    m_etiquette.setFont(m_font);
    m_etiquette.setColor(sf::Color::Red);
    m_etiquette.setCharacterSize(RAYON*4);

    m_sommet.setOrigin({RAYON, RAYON});
    m_sommet.setOutlineColor(sf::Color::Black);
    m_sommet.setOutlineThickness(0.5);
}

// ---------------------------------------
void Appli::run() {
    while(m_fenetre.isOpen()){
        traiter_evenements();
        dessiner();
    }

}

// ---------------------------------------
void Appli::setGraphe(Graphe &g){
    m_g = &g;
}

// ---------------------------------------

void Appli::traiter_evenements()
{
    sf::Event evenement;
    while (m_fenetre.pollEvent(evenement)) {
        switch (evenement.type) {
        case sf::Event::Closed : // gestionnaire de fenêtre
            m_fenetre.close();
            break;
        case  sf::Event::KeyPressed : // clavier
            switch (evenement.key.code) {
            case  sf::Keyboard::Escape :  // touche échappement
                m_fenetre.close();
                break;
            case  sf::Keyboard::L :  // touche échappement
                m_montre_etiquette = !m_montre_etiquette;
                break;
            default:
                // autre touche
                break;
            };
        default :
            break;
        }
    }
}

// ---------------------------------------

void Appli::dessiner()
{
    m_fenetre.clear(sf::Color::White);

    //dessin des aretes
    for(auto e : m_g->aretes()){
        Sommet src = m_g->source(e);
        Sommet dest = m_g->destination(e);

        Coord c_src = m_g->positionSommet(src);
        Coord c_dest = m_g->positionSommet(dest);

        Couleur couleur_src = m_g->couleurSommet(src);
        Couleur couleur_dest = m_g->couleurSommet(dest);

        sf::Color col_src {couleur_src[0],couleur_src[1],couleur_src[2],couleur_src[3]};
        sf::Color col_dest {couleur_dest[0],couleur_dest[1],couleur_dest[2],couleur_dest[3]};
        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(c_src.m_x, c_src.m_y), col_src),
            sf::Vertex(sf::Vector2f(c_dest.m_x, c_dest.m_y), col_dest)
        };
        m_fenetre.draw(line, 2, sf::Lines);
    }

    // dessin des sommets
    for(auto n : m_g->sommets()){
        Coord c = m_g->positionSommet(n);
        Couleur col = m_g->couleurSommet(n);
        sf::Vector2f pos {c.m_x, c.m_y};
        m_sommet.setPosition(pos);
        m_sommet.setFillColor(Color(col[0], col[1], col[2], col[3]));
        m_fenetre.draw(m_sommet);

        string s = m_g->etiquetteSommet(n);
        m_etiquette.setString(s.c_str());

        if(m_montre_etiquette){
            // redimensionne l'etiquette
            FloatRect bb = m_etiquette.getLocalBounds();      // retourne les positions et taille réelles de s
            float height_factor = (4.*RAYON) / bb.height;  // facteur de mise à l'échelle pour la largeur
            m_etiquette.setScale(height_factor, height_factor);

            // centre l'etiquette sur le sommet
            bb = m_etiquette.getGlobalBounds();
            pos.x -= bb.width/2.;
            pos.y -= bb.height/2.;
            m_etiquette.setPosition(pos);
            m_fenetre.draw(m_etiquette);
        }

    }

    m_fenetre.display();

}
