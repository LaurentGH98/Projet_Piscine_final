#ifndef SOMMET_H
#define SOMMET_H
#include <string>
#include <vector>
#include <set>

//Sommet = point de la station

class Sommet
{
private:
    int m_numero;
    std::string m_nom;
    int m_altitude;
    int m_couleur;//pour BFS : 0 pour blanc 1 pour gris 2 pour noir
    std::set<Sommet*> m_successeurs; //chaque sommet possède des sommets successeurs, le set évite les doublons

    bool m_marque; //pour Dijkstra

    std::vector<int >m_connexe;//tenta nagihan dijkstra


public:
    ///constructeur et destructeur
    Sommet(int num, std::string nom, int altitude); //surcharge pour ces 3 attributs mais initialisation du bool m_marque à 0
                                                    //en effet pour Dijkstra, les sommets sont d'abord non marqué
    ~Sommet();

    ///getters
    int getNumero() const;
    int getAltitude() const;
    std::string getNom() const;
    int getCouleur() const;
    bool getMarque() const;
    std::set<Sommet*> getSuccesseur() const;

    ///setters
    void setMarque (bool marque);
    void setCouleur (int couleur);
    std::set<Sommet*>* setSuccesseur()
        {
            return &m_successeurs;
        }

    ///méthodes
    void ajouterSuccesseur(Sommet* voisin); ///ajoute un sommet adjacent pour BFS
    void afficherSommet()const; //on affiche le numéro du sommet et/ou son nom; pour Dijkstra

    /// Kruskal
    bool verif_connexe(Sommet * S2);
    std::vector<int>getConnexe();
    void addConnexe(int A);

};


#endif // SOMMET_H
