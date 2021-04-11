#ifndef GRAPHE_H
#define GRAPHE_H
#include <vector>
#include <string>
#include <iostream>
#include "Sommet.h"
#include "Arc.h"


class Graphe
{
private:
    int m_nbrArc;//nbr de trajet dans le graphe
    int m_nbrSommet;// nbr de sommets dans le graphe
    std::vector <Sommet*>m_sommet;//c est le vecteur qui possede tous les sommets du graphe
    std::vector <Arc*>m_arc;//c est le vecteur qui possede toutes les arcs du graphe

public:
    ///constructeur et destructeur
    Graphe (std::string nomFichier);
    ~Graphe();

    /// 4.3 ///

    void infoArc(); // m�thode permettant � l'utilisateur de savoir d'o� il part et o� il arrive (en choisissant un trajet)
    void infoSommet(); // m�thode qui renseigne sur le sommet, � savoir les arcs (trajets) entrant et sortant

    /// 4.4 ///

    void donneOrientation(); //affecte l'orientation des arcs en parcourant la liste d'arcs, 0 pour entrant 1 pour sortant
    void trouverSuccesseurs(int numeroExt1);
    void parcoursBFS();

    /// 4.5 ///

    void chemin_interessant(); // m�thode chemin le plus int�ressant
    void saisi_depart_arrivee(std::vector<Arc*>arcs_tries); // saisi des sommets de d�part et d'arriv�e par l'utilisateur
    void Kruskal(int x1, int x2, std::vector<Arc*>arcs_tries); // algo Kruskal + affichage

    void Menu();
    void AffichageSommet();
    void AffichageArc();

};


#endif // GRAPHE_H
