TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Arete.cpp \
    Graphe.cpp \
    Appli.cpp \
    Coord.cpp \
    Sommet.cpp \
    Couleur.cpp

HEADERS += \
    Arete.h \
    Graphe.h \
    Appli.h \
    Coord.h \
    Sommet.h \
    Couleur.h \
    Propriete.hpp

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra
LIBS           += -lsfml-graphics -lsfml-window -lsfml-system
