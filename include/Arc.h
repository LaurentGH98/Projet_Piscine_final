#ifndef ARC_H
#define ARC_H
#include "Sommet.h"
#include <iostream>

class Arc
{
 private:
    std::pair<Sommet*,Sommet*> m_SommetAdjacent; // plus facile d'utilisation + associer les 2 extrémités d'un arc en un seul attribut pour un arc
    int m_duree; //temps du trajet en secondes correspondant à la pondération (pour Dijkstra)
    std::string m_nom;
    int m_numero;
    std::string m_type;
    float m_denivele; //mètres de denivelé pour un trajet
    int m_orientation;//0 si arc sortant du sommet et 1 si arc entrant du sommet

public:
    ///constructeur et destructeur
    Arc(int num, std::string nom, std::string type, Sommet* ext1,Sommet* ext2);
    ~Arc();

    ///getters
    int getDuree()const;
    std::string getNom()const;
    int getNumero()const;
    std::string getType()const;
    std::pair<Sommet*,Sommet*> getSommetAdj()const;


    float getDenivele() const;
    int getOrientation() const;

    ///setter(s)
    void setOrientation(int orientation);

    ///methode affiche les attributs d'un arc
    void afficher()const;

};

#endif // ARC_H
