#include "Graphe.h"
#include <fstream>
#include <set> //au cas où, sil à l'affichage on ne veut pas de doublons pour les successeurs
//vu qu'il y a potentiellement plusieurs pistes d'un sommet à l'autre
#include <queue> //pour BFS

//HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);//Ici, on récupère un handle vers ce qui représente la sortie standard sous Windows.

using namespace std;

///constructeur
Graphe::Graphe(std::string nomFichier)
{
    std::ifstream ifs{nomFichier};
    if (!ifs)
        throw std::runtime_error( "Impossible d'ouvrir en lecture " + nomFichier );

    ifs >> m_nbrSommet;
    if ( ifs.fail() )
        throw std::runtime_error("Probleme lecture ordre du graphe");

    int numSom, altitude;
    std::string nomSom;
    for (int i=0; i<m_nbrSommet; ++i)
    {
        ifs>>numSom>>nomSom>>altitude;
        if ( ifs.fail() )
            throw std::runtime_error("Probleme lecture des sommets");
        m_sommet.push_back(new Sommet(numSom,nomSom,altitude));
    }


    /**AFFICHAGE DES SOMMETS**/

    /* std::cout<<"Nbr Sommets = "<< m_nbrSommet <<std::endl;
     std::cout<<"\nSOMMETS :"<<std::endl;
     for (auto s : m_sommet)
     {
         std::cout << s->getNumero() << " " << s->getNom() << " " << s->getAltitude() << std::endl;
     }

      std::cout<<"Nbr Sommets = "<< m_nbrSommet <<std::endl;
      std::cout<<"\nSOMMETS :"<<std::endl;
      for (auto s : m_sommet)
      {
          std::cout << s->getNumero() << " " << s->getNom() << " " << s->getAltitude() << std::endl;
      }*/



    ifs >> m_nbrArc;
    if ( ifs.fail() )
        throw std::runtime_error("Probleme lecture taille du graphe");

    int numArc, ext1,ext2;
    std::string nomArc, typeArc;
    for (int i=0; i<m_nbrArc; ++i)
    {
        ifs>>numArc>>nomArc>>typeArc>>ext1>>ext2;
        if ( ifs.fail() )
            throw std::runtime_error("Probleme lecture des arcs");
        m_arc.push_back(new Arc(numArc,nomArc,typeArc,m_sommet[ext1-1],m_sommet[ext2-1]));
    }

    /**AFFICHAGE DES ARCS**/
    /* std::cout<<"\nNbr arcs = "<< m_nbrArc <<std::endl;
     std::cout<<"\nARCS :"<<std::endl;
     for (auto a : m_arc)
     {
         std::cout << a->getNumero() << " " << a->getNom() << " " << a->getType()<<" " ;
         a->afficher();
     }*/

}


///destructeur
Graphe::~Graphe()
{
}


void Graphe::AffichageSommet()
{
    /**AFFICHAGE DES SOMMETS**/
    std::cout<<"Nbr Sommets = "<< m_nbrSommet <<std::endl;
    std::cout<<"\nSOMMETS :"<<std::endl;
    for (auto s : m_sommet)
    {
        std::cout << s->getNumero() << " " << s->getNom() << " " << s->getAltitude() << std::endl;
    }
}

void Graphe::AffichageArc()
{
    /**AFFICHAGE DES ARCS**/
    std::cout<<"\nNbr arcs = "<< m_nbrArc <<std::endl;
    std::cout<<"\nARCS :"<<std::endl;
    for (auto a : m_arc)
    {
        std::cout << a->getNumero() << " " << a->getNom() << " " << a->getType()<<" " ;
        a->afficher();
    }
}

///méthode permettant à l'utilisateur de savoir d'où il part et où il arrive
void Graphe::infoArc()
{
    int Num_Arc = 0; // variable contenant le numéro de l'arc saisie
    int Num_Arc2 = 0; // variable tempon pour blindage
    bool Num_Trouve = false;

    std::cout << "\n\nVeuillez saisir le numero d'un arc : " << std::endl;
    std::cin >> Num_Arc;


    while(Num_Trouve == false) // tant que l'arc n'a pas été trouvé
    {
        for(auto a : m_arc) //a pour arc; on parcourt le vector d'arc
        {
            if(Num_Arc == a->getNumero()) // si le numéro existe dans le vector
            {
                std::cout << "\nVous avez choisi le trajet suivant :" << std::endl;
                std::cout << "Numero: " << a->getNumero() << "\nNom: " << a->getNom() << "\nType: " << a->getType()<< std::endl;
                std::cout << "Point de depart: " << a->getSommetAdj().first->getNom() << " (" << a->getSommetAdj().first->getNumero()<< ")" << std::endl;
                std::cout << "Point d'arrivee: " << a->getSommetAdj().second->getNom() << " (" << a->getSommetAdj().second->getNumero() << ")" << std::endl;
                std::cout << "Duree entre ces deux points: " << a->getDuree()  << " minute(s)" <<std::endl;

                Num_Trouve = true; // alors on a trouvé l'arc
                break;
            }

            else if( (a->getNumero()>=95) && (Num_Arc != a->getNumero()) ) // sinon
            {
                std::cout << "\nCe trajet n'existe pas.\nVeuillez saisir a nouveau: ";
                std::cin >> Num_Arc2;
                Num_Arc = Num_Arc2;
                Num_Trouve = false; // on n'a toujours pas trouvé l'arc
            }
        }
    }
}

///Pour connaître les arcs/trajets entrants et sortants à un sommet choisi
void Graphe::infoSommet()
{

    int numSomChoisi;
    bool boucle(false);

    std::cout<<"\nSur quel point de station souhaitez vous vous renseigner ?(Entrez un numero) ";

    while (boucle==false)   //blindage de la saisie des sommets pour les entiers
    {
        std::cout<<std::endl;
        std::cin>> numSomChoisi;

        if( numSomChoisi>=1 && numSomChoisi<=37)
            boucle=true;
        else
        {
            std::cout<<"Ce point de station n'existe pas, reessayez : ";
        }

    }



    std::cout<<std::endl;
    std::cout<<"Voici toutes les pistes ou remontees sortant du point de station "<<numSomChoisi<<" :"<<std::endl;
    std::cout<<std::endl;

    /*Premiere boucle pour trouver les sommets sortants*/
    for(auto elem : m_arc) //on parcourt la liste des arcs du graphe
    {
        if(elem->getSommetAdj().first->getNumero() == numSomChoisi) //si le numéro du sommet choisi correspond à la première extrémité
            //alors le trajet est forcément sortant
        {
            std::cout<<"Ce trajet se dirigeant vers : "<<elem->getSommetAdj().second->getNom()<<" s'appelle : "<<elem->getNom();

            /*Conditions pour donné le nom du type de trajet */
            if((elem->getType()) == "V")
                std::cout<<" et c'est une piste verte.";

            if((elem->getType()) == "B")
                std::cout<<" et c'est une piste bleue.";

            if((elem->getType()) == "R")
                std::cout<<" et c'est une piste rouge.";

            if((elem->getType()) == "N")
                std::cout<<" et c'est une piste noire.";

            if((elem->getType()) == "KL")
                std::cout<<" et c'est une piste de kilometre lance.";

            if((elem->getType()) == "SURF")
                std::cout<<" et c'est un snowpark.";

            if((elem->getType()) == "TPH")
                std::cout<<" et c'est un telepherique.";

            if((elem->getType()) == "TC")
                std::cout<<" et c'est des telecabines.";

            if((elem->getType()) == "TSD")
                std::cout<<" et c'est un telesiege debrayable.";

            if((elem->getType()) == "TS")
                std::cout<<" et c'est un telesiege.";

            if((elem->getType()) == "TK")
                std::cout<<" et c'est un teleski.";

            if((elem->getType()) == "BUS")
                std::cout<<" et c'est une navette.";

            std::cout<<std::endl;

        }
    }

    std::cout<<std::endl;
    std::cout<<"Voici toutes les pistes ou remontes permattant d'aller au point de station "<<numSomChoisi<<" :"<<std::endl;
    std::cout<<std::endl;

    /*Deuxieme boucle pour trouver les somments entrant*/
    for(auto elem : m_arc) //même démarche
    {
        if(elem->getSommetAdj().second->getNumero() == numSomChoisi)
        {
            std::cout<<"Ce trajet arrive de : "<<elem->getSommetAdj().first->getNom()<<" s'appelle : "<<elem->getNom();

            /*Conditions pour donné le nom du type de trajet */
            if((elem->getType()) == "V")
                std::cout<<" et c'est une piste verte.";

            if((elem->getType()) == "B")
                std::cout<<" et c'est une piste bleue.";

            if((elem->getType()) == "R")
                std::cout<<" et c'est une piste rouge.";

            if((elem->getType()) == "N")
                std::cout<<" et c'est une piste noire.";

            if((elem->getType()) == "KL")
                std::cout<<" et c'est une piste de kilometre lance.";

            if((elem->getType()) == "SURF")
                std::cout<<" et c'est un snowpark.";

            if((elem->getType()) == "TPH")
                std::cout<<" et c'est un telepherique.";

            if((elem->getType()) == "TC")
                std::cout<<" et c'est des telecabines.";

            if((elem->getType()) == "TSD")
                std::cout<<" et c'est un telesiege debrayable.";

            if((elem->getType()) == "TS")
                std::cout<<" et c'est un telesiege.";

            if((elem->getType()) == "TK")
                std::cout<<" et c'est un teleski.";

            if((elem->getType()) == "BUS")
                std::cout<<" et c'est une navette.";

            std::cout<<std::endl;

        }
    }
}


/// méthode qui permet d'obtenir le chemin le plus interessant (montées/descentes)
void Graphe::chemin_interessant()
{
    int niveau = 3;
    std::string choix_utilisateur = "Q"; // initialisation par defaut
    std::vector<std::string> choix_tot; // vector pour stocker tous les choix de l'utilisateur
    std::vector<Arc*>arcs_tries; // vector contenant tous les arcs sauf ceux que veux eviter l'utilisateur


    //int interet[12]; // 0-Rien // 1-Piste Verte // 2-Piste bleue // 3-Piste Rouge // n4-Piste Noire // 5-Piste de Kilometre Lance // 6-Snowpark
    // 7-Telepherique // 8-Telecabine // 9-Telesiege debrayable // 10-Telesiege // 11-Teleski // 12-Navette

    /*
    for(int i = 1; i <= 6; i++) // positif si descente
    {
        interet[i] = +10*i;
    }
    for(int i = 7; i<=12; i++) // negatif si remontée
    {
        interet[i] = -10*i;
    }*/

    if(niveau == 1) // debutant
    {
        std::cout << "Vous ne pouvez pas emprunter de pistes noires ni de pistes rouges.\n" <<std::endl;
        std::cout << "Z-FIN.\nV-Piste Verte\nB-Piste bleue\nKL-Piste de Kilometre Lance\nSURF-Snowpark" << std::endl;
        std::cout << "TPH-Telepherique\nTC-Telecabine\nTSD-Telesiege debrayable\nTS-Telesiege\nTK-Teleski\nBUS-Navette" << std::endl;
        std::cout << "\nQue voulez-vous eviter d'autre ? \n(Veuillez saisir 'Z' pour terminer votre saisi)\n" << std::endl;

        while(choix_utilisateur != "Z")
        {

            std::cin >> choix_utilisateur;
            //blindage saisie
            if((choix_utilisateur!= "V") && (choix_utilisateur!= "B") && (choix_utilisateur!= "KL") && (choix_utilisateur!= "SURF") && (choix_utilisateur!= "TPH") && (choix_utilisateur!= "TC") && (choix_utilisateur!= "TSD") && (choix_utilisateur!= "TS") && (choix_utilisateur!= "TK") && (choix_utilisateur!= "BUS") && choix_utilisateur!= "Z" )
            {
                while((choix_utilisateur!= "V") && (choix_utilisateur!= "B") && (choix_utilisateur!= "KL") && (choix_utilisateur!= "SURF") && (choix_utilisateur!= "TPH") && (choix_utilisateur!= "TC") && (choix_utilisateur!= "TSD") && (choix_utilisateur!= "TS") && (choix_utilisateur!= "TK") && (choix_utilisateur!= "BUS") && choix_utilisateur!= "Z" )
                {
                    char choix_utilisateur2;
                    std::cout << "Saisie invalide. Veuillez saisir a nouveau :" << std::endl;
                    std::cin >> choix_utilisateur2;
                    choix_utilisateur = choix_utilisateur2;
                }
            }
            //saisie pour sortir de la boucle
            else if(choix_utilisateur=="Z")
            {
                std::cout << "Saisie valide!\n" <<std::endl;
            }

            choix_tot.push_back(choix_utilisateur); // on stock tous les choix de l'utilisateur dans un vector
        }


        // parcours de tous les choix de l'utilisateur
        for(unsigned int k=0; k<choix_tot.size(); k++)
        {
            // parcours de tous les arcs
            for(unsigned int i=0; i<m_arc.size(); i++)
            {
                // si c le premier choix (k=0) et si on ne prend pas en compte les pistes Noires et les choix de l'utilisateur
                if( (k==0) && (choix_tot[k] != m_arc[i]->getType()) && (m_arc[i]->getType()!="N") && (m_arc[i]->getType()!="R"))
                {
                    // alors on rempli un vecteur avec toutes ces conditions
                    arcs_tries.push_back(m_arc[i]);
                }
                // si c + que le premier choix (k>0)
                else if(k>0)
                {
                    // parcours du vecteur déjà rempli auparavant pour k=0
                    for(unsigned int j=0; j<arcs_tries.size(); j++)
                    {
                        // si le choix de l'utilisateur est parmi le vecteur rempli auparavant
                        if(choix_tot[k] == arcs_tries[j]->getType())
                        {
                            // alors on supprime aussi ces conditions
                            arcs_tries.erase(arcs_tries.begin()+j);
                        }
                    }
                }
            }
        }

        //affichage
        /*for(auto tri : arcs_tries)
        {
            std::cout << "APRES : " << tri->getType() << std::endl;
        }*/
    }

    else if(niveau == 2) // intermediaire
    {
        std::cout << "Vous ne pouvez pas emprunter de pistes noires.\n" <<std::endl;
        std::cout << "Z-Rien\nV-Piste Verte\nB-Piste bleue\nR-Piste Rouge\nKL-Piste de Kilometre Lance\nSURF-Snowpark" << std::endl;
        std::cout << "TPH-Telepherique\nTC-Telecabine\nTSD-Telesiege debrayable\nTS-Telesiege\nTK-Teleski\nBUS-Navette" << std::endl;
        std::cout << "\nQue voulez-vous eviter d'autre ?\n(Veuillez saisir 0 pour terminer votre saisi)\n" <<std::endl;

        while(choix_utilisateur != "Z")
        {
            std::cin >> choix_utilisateur;
            //blindage saisie
            if((choix_utilisateur!= "V") && (choix_utilisateur!= "B") && (choix_utilisateur!= "R") && (choix_utilisateur!= "KL") && (choix_utilisateur!= "SURF") && (choix_utilisateur!= "TPH") && (choix_utilisateur!= "TC") && (choix_utilisateur!= "TSD") && (choix_utilisateur!= "TS") && (choix_utilisateur!= "TK") && (choix_utilisateur!= "BUS") && choix_utilisateur!= "Z" )
            {
                while((choix_utilisateur!= "V") && (choix_utilisateur!= "B") && (choix_utilisateur!= "R") && (choix_utilisateur!= "KL") && (choix_utilisateur!= "SURF") && (choix_utilisateur!= "TPH") && (choix_utilisateur!= "TC") && (choix_utilisateur!= "TSD") && (choix_utilisateur!= "TS") && (choix_utilisateur!= "TK") && (choix_utilisateur!= "BUS") && choix_utilisateur!= "Z" )
                {
                    char choix_utilisateur2;
                    std::cout << "Saisie invalide. Veuillez saisir a nouveau :" << std::endl;
                    std::cin >> choix_utilisateur2;
                    choix_utilisateur = choix_utilisateur2;
                }
            }
            //saisie pour sortir de la boucle
            else if(choix_utilisateur=="Z")
            {
                std::cout << "Saisie valide!" <<std::endl;
            }
            choix_tot.push_back(choix_utilisateur); // on stock tous les choix de l'utilisateur dans un vector
        }

        // parcours de tous les choix de l'utilisateur
        for(unsigned int k=0; k<choix_tot.size(); k++)
        {
            // parcours de tous les arcs
            for(unsigned int i=0; i<m_arc.size(); i++)
            {
                // si c le premier choix (k=0) et si on ne prend pas en compte les pistes Noires et Rouges et les choix de l'utilisateur
                if( (k==0) && (choix_tot[k] != m_arc[i]->getType()) && (m_arc[i]->getType()!="N"))
                {
                    // alors on rempli un vecteur avec toutes ces conditions
                    arcs_tries.push_back(m_arc[i]);
                }
                // si c + que le premier choix (k>0)
                else if(k>0)
                {
                    // parcours du vecteur déjà rempli auparavant pour k=0
                    for(unsigned int j=0; j<arcs_tries.size(); j++)
                    {
                        // si le choix de l'utilisateur est parmi le vecteur rempli auparavant
                        if(choix_tot[k] == arcs_tries[j]->getType())
                        {
                            // alors on supprime aussi ces conditions
                            arcs_tries.erase(arcs_tries.begin()+j);
                        }
                    }
                }
            }
        }

        //affichage
        /*for(auto tri : arcs_tries)
        {
            std::cout << "APRES : " << tri->getType() << std::endl;
        }*/
    }

    else if(niveau == 3) // professionnel
    {
        std::cout << "Z-Rien\nV-Piste Verte\nB-Piste bleue\nR-Piste Rouge\nN-Piste Noire\nKL-Piste de Kilometre Lance\nSURF-Snowpark" << std::endl;
        std::cout << "TPH-Telepherique\nTC-Telecabine\nTSD-Telesiege debrayable\nTS-Telesiege\nTK-Teleski\nBUS-Navette" << std::endl;
        std::cout << "\nQue voulez-vous eviter ? \n(Veuillez saisir 0 pour terminer votre saisi)\n" <<std::endl;

        while(choix_utilisateur != "Z")
        {
            std::cin >> choix_utilisateur;
            //blindage saisie
            if((choix_utilisateur!= "V") && (choix_utilisateur!= "B") && (choix_utilisateur!= "R") && (choix_utilisateur!= "N") && (choix_utilisateur!= "KL") && (choix_utilisateur!= "SURF") && (choix_utilisateur!= "TPH") && (choix_utilisateur!= "TC") && (choix_utilisateur!= "TSD") && (choix_utilisateur!= "TS") && (choix_utilisateur!= "TK") && (choix_utilisateur!= "BUS") && choix_utilisateur!= "Z" )
            {
                while((choix_utilisateur!= "V") && (choix_utilisateur!= "B") && (choix_utilisateur!= "R") && (choix_utilisateur!= "N") && (choix_utilisateur!= "KL") && (choix_utilisateur!= "SURF") && (choix_utilisateur!= "TPH") && (choix_utilisateur!= "TC") && (choix_utilisateur!= "TSD") && (choix_utilisateur!= "TS") && (choix_utilisateur!= "TK") && (choix_utilisateur!= "BUS") && choix_utilisateur!= "Z" )
                {
                    char choix_utilisateur2;
                    std::cout << "Saisie invalide. Veuillez saisir a nouveau :" << std::endl;
                    std::cin >> choix_utilisateur2;
                    choix_utilisateur = choix_utilisateur2;
                }
            }
            //saisie pour sortir de la boucle
            else if(choix_utilisateur=="Z")
            {
                std::cout << "Saisie valide!" <<std::endl;
            }
            choix_tot.push_back(choix_utilisateur); // on stock tous les choix de l'utilisateur dans un vector
        }

        // parcours de tous les choix de l'utilisateur
        for(unsigned int k=0; k<choix_tot.size(); k++)
        {
            // parcours de tous les arcs
            for(unsigned int i=0; i<m_arc.size(); i++)
            {
                // si c le premier choix (k=0) et si on ne prend pas en compte les choix de l'utilisateur
                if( (k==0) && (choix_tot[k] != m_arc[i]->getType()) )
                {
                    // alors on rempli un vecteur avec toutes ces conditions
                    arcs_tries.push_back(m_arc[i]);
                }
                // si c + que le premier choix (k>0)
                else if(k>0)
                {
                    // parcours du vecteur déjà rempli auparavant pour k=0
                    for(unsigned int j=0; j<arcs_tries.size(); j++)
                    {
                        // si le choix de l'utilisateur est parmi le vecteur rempli auparavant
                        if(choix_tot[k] == arcs_tries[j]->getType())
                        {
                            // alors on supprime aussi ces conditions
                            arcs_tries.erase(arcs_tries.begin()+j);
                        }
                    }
                }
            }
        }

        //affichage
        /*for(auto tri : arcs_tries)
        {
            std::cout << "APRES : " << tri->getType() << std::endl;
        }*/
    }

    //demande du sommet de depart/arrivee
    saisi_depart_arrivee(arcs_tries);
}

/// saisi les sommets de depart et d'arrive et ensuite appel l'algo de Kruskal
void Graphe::saisi_depart_arrivee(std::vector<Arc*>arcs_tries)
{
    int x1,x2;
    std::cout<<"\nSommet de depart: ";
    std::cin>>x1;
    std::cout<<"Sommet d'arrive : ";
    std::cin>>x2;
    Kruskal(x1, x2, arcs_tries);
}

/// algo de Kruskal
void Graphe::Kruskal(int x1, int x2, std::vector<Arc*>arcs_tries)
{
    std::vector <Arc*> arc_final; // vector qui stock tous les arcs qui vont être affichés
    bool fin = false; // booléen devient true lorsque on a atteint x2

    while((arc_final.size() <(unsigned int)m_nbrSommet -1) && (fin==false))
    {

        int MIN=9999; // initialisation par défaut
        int act; // variable pour stocker l'arc

        // recherche de la plus petite arc
        for(unsigned int i =0 ; i<arcs_tries.size(); i++)
        {
            if(arcs_tries[i]->getSommetAdj().first->getNumero() == x1) // on détecte le sommet de départ dans le vector d'arc
            {
                if(arcs_tries[i]->getDuree() < MIN) // on prend l'arc qui a la plus petite pondération
                {
                    act=i; // on stock cet arc
                    MIN = arcs_tries[i]->getDuree();
                }
            }
        }



        // vérifie si l'arc ne fait pas de boucle
        if(arcs_tries[act]->getSommetAdj().first->verif_connexe(arcs_tries[act]->getSommetAdj().second) == true)
        {
            arc_final.push_back(arcs_tries[act]); // on met cet arc dans un vector

            // ajout des nouvelles composantes connexes
            arcs_tries[act]->getSommetAdj().first->addConnexe( arcs_tries[act]->getSommetAdj().second->getNumero());
            arcs_tries[act]->getSommetAdj().second->addConnexe( arcs_tries[act]->getSommetAdj().first->getNumero());


            for(unsigned int i=0 ; i<arcs_tries[act]->getSommetAdj().first->getConnexe().size() ; i++)
            {
                // ajout connexité sommet2 avec sommet1
                arcs_tries[act]->getSommetAdj().second->addConnexe(arcs_tries[act]->getSommetAdj().first->getConnexe()[i]);
            }

            for(unsigned int i=0 ; i<arcs_tries[act]->getSommetAdj().second->getConnexe().size() ; i++)
            {
                // ajout connexité sommet1 avec sommet2
                arcs_tries[act]->getSommetAdj().first->addConnexe(arcs_tries[act]->getSommetAdj().second->getConnexe()[i]);
            }

            if(arcs_tries[act]->getSommetAdj().second->getNumero() == x2) // si l'extrémité 2 est le sommet x2 saisi par l'utilisateur
            {
                fin = true; // alors on sort de la boucle while
            }

        }
        //supprimer l'arc séléctionnée du vector de base
        arcs_tries.erase(arcs_tries.begin()+ act);
    }

    // affichage des arcs conservées (final)
    std::cout <<std::endl << "\nARCS CONSERVEES : "<<std::endl;
    int poids=0;
    for(unsigned int i =0 ; i<arc_final.size(); i++)
    {
        poids=poids+arc_final[i]->getDuree(); // on récupère le poids final (pondération de tous les arcs séléctionnées)
        std::cout << arc_final[i]->getSommetAdj().first->getNumero() << " --> " <<  arc_final[i]->getSommetAdj().second->getNumero() << "   Poids: " << arc_final[i]->getDuree();
        std::cout << "   Type : " << arc_final[i]->getType() << std::endl;
    }

    std::cout <<std::endl <<"poids de l'arbre: "<<poids<<std::endl;

}


void Graphe::Menu()
{
    int choix(0);//pour menu
    bool valide(false);//pour blindage
    bool boucle(false);

    std::cout<<std::endl;
    std::cout<<"********************************************************************************"<<std::endl;
    std::cout<<"**************************************BONJOUR!**********************************"<<std::endl;
    std::cout<<"********************************************************************************"<<std::endl;
    std::cout<<std::endl;


    while(boucle == false )
    {
        std::cout<<std::endl;
        std::cout<<"**************************************MENU**************************************"<<std::endl;
        std::cout<<"/1/  Information sur un trajet"<<std::endl;
        std::cout<<"/2/  Information sur un de nos points de stations"<<std::endl;
        std::cout<<"/3/  Information sur les plus courts chemin depuis votre position"<<std::endl;
        std::cout<<"/4/  Information sur le chemin le plus rapide entre 2 points"<<std::endl;
        std::cout<<"/5/  Information sur le chemin le plus interressant pour vous"<<std::endl;
        std::cout<<"/6/  Information sur nos points de stations et nos trajets"<<std::endl;
        std::cout<<"/7/  Information flots"<<std::endl;
        std::cout<<"/8/  Quitter la borne"<<std::endl;
        std::cout<<std::endl;
        std::cout<<"Quelle information souhaitez-vous connaitre par rapport a notre magnifique station ?(Entrez un numero)  "<<std::endl;


        while (valide==false)   //blindage de la saisie des sommets pour les entiers
        {
            std::cin>> choix;

            if( choix>=1 && choix<=8)
                valide=true;
            else
            {
                std::cout<<"Choix impossible, veuillez recommencer : ";
            }
        }

        switch(choix)
        {
        case 1:
            system("cls");
            std::cout<<"#############################################"<<std::endl;
            std::cout<<"#           .-~~\\                           #"<<std::endl;
            std::cout<<"#          /     \\ _                        #"<<std::endl;
            std::cout<<"#          ~x   .-~_)_                      #"<<std::endl;
            std::cout<<"#            ~x*.-~   ~-.                   #"<<std::endl;
            std::cout<<"#        _   ( /         \\   _              #"<<std::endl;
            std::cout<<"#        ||   T  o  o     Y  ||             #"<<std::endl;
            std::cout<<"#      ==:l   l   <       !  I;==           #"<<std::endl;
            std::cout<<"#         \\\\   \\  .__/   /  //              #"<<std::endl;
            std::cout<<"#          \\\\ ,r*-,___.-'r.//               #"<<std::endl;
            std::cout<<"#           }^ \\.( )   _.'//.               #"<<std::endl;
            std::cout<<"#          /    }~Xi--~  //  \\              #"<<std::endl;
            std::cout<<"#          Y    Y I\\ \\    *    Y            #"<<std::endl;
            std::cout<<"#          |    | |o\\ \\        |            #"<<std::endl;
            std::cout<<"#          |    l_l  Y T       |  -Row      #"<<std::endl;
            std::cout<<"#          l      *o l_j       !            #"<<std::endl;
            std::cout<<"#           \\                 /             #"<<std::endl;
            std::cout<<"#    ___,.---^.     o       .^---.._____    #"<<std::endl;
            std::cout<<"#*~~~          *           ~            ~~~*#"<<std::endl;
            std::cout<<"#############################################"<<std::endl;
            infoArc();
            valide=false;
            break;
        case 2:
            system("cls");
            std::cout<<"#############################################################"<<std::endl;
            std::cout<<"#        _    .  ,   .           .                          #"<<std::endl;
            std::cout<<"#    *  / \\_ *  / \\_      _  *        *   /\\'__        *    #"<<std::endl;
            std::cout<<"#      /    \\  /    \\,   ((        .    _/  /  \\  *'.       #"<<std::endl;
            std::cout<<"#   . /\\/\\  /\\/ :' __ \\_  `          _^/  ^/    `--.        #"<<std::endl;
            std::cout<<"#    /    \\/  \\  _/  \\-'\\      *    /.' ^_   \\_   .'\\  *    #"<<std::endl;
            std::cout<<"#  /\\  .-   `. \\/     \\ /==~=-=~=-=-;.  _/ \\ -. `_/   \\     #"<<std::endl;
            std::cout<<"# /  `-.__ ^   / .-'.--\\ =-=~_=-=~=^/  _ `--./ .-'  `-      #"<<std::endl;
            std::cout<<"#/        `.  / /       `.~-^=-=~=^=.-'      '-._ `._       #"<<std::endl;
            std::cout<<"#############################################################"<<std::endl;
            infoSommet();
            valide=false;
            break;
        case 3:
            system("cls");
            std::cout<<"#############################################################################"<<std::endl;
            std::cout<<"#          /\\                                                               #"<<std::endl;
            std::cout<<"#         /**\\                                                              #"<<std::endl;
            std::cout<<"#        /****\\   /\\                                                        #"<<std::endl;
            std::cout<<"#       /      \\ /**\\                                                       #"<<std::endl;
            std::cout<<"#      /  /\\    /    \\        /\\    /\\  /\\      /\\            /\\/\\/\\  /\\    #"<<std::endl;
            std::cout<<"#     /  /  \\  /      \\      /  \\/\\/  \\/  \\  /\\/  \\/\\  /\\  /\\/ / /  \\/  \\   #"<<std::endl;
            std::cout<<"#    /  /    \\/ /\\     \\    /    \\ \\  /    \\/ /   /  \\/  \\/  \\  /    \\   \\  #"<<std::endl;
            std::cout<<"#   /  /      \\/  \\/\\   \\  /      \\    /   /    \\                           #"<<std::endl;
            std::cout<<"#__/__/_______/___/__\\___\\__________________________________________________#"<<std::endl;
            std::cout<<"#############################################################################"<<std::endl;
            //bfs les plus courts chemins
            valide=false;
            break;
        case 4:
            system("cls");
            std::cout<<"###############################"<<std::endl;
            std::cout<<"#            o\\               #"<<std::endl;
            std::cout<<"#  _________/__\\__________    #"<<std::endl;
            std::cout<<"#  |                  - (  |  #"<<std::endl;
            std::cout<<"# ,'-.                 . `-|  #"<<std::endl;
            std::cout<<"#(____ .       ,-.    '   ||  #"<<std::endl;
            std::cout<<"#  |          /\\,-\\   ,-.     #"<<std::endl;
            std::cout<<"#  |      ,-./     \\ /'.-\\ |  #"<<std::endl;
            std::cout<<"#  |     /-.,\\      /     \\|  #"<<std::endl;
            std::cout<<"#  |    /     \\    ,-.     \\  #"<<std::endl;
            std::cout<<"#  |___/_______\\__/___\\_____\\ #"<<std::endl;
            std::cout<<"###############################"<<std::endl;
            //et dijkstra (4.4)
            valide=false;
            break;
        case 5:
            system("cls");
            std::cout<<"######################################"<<std::endl;
            std::cout<<"#            *                       #"<<std::endl;
            std::cout<<"#            XX                      #"<<std::endl;
            std::cout<<"#           MMMMM                    #"<<std::endl;
            std::cout<<"#           //(00                    #"<<std::endl;
            std::cout<<"#        .:.....                     #"<<std::endl;
            std::cout<<"#        .:::::::::                  #"<<std::endl;
            std::cout<<"#       :: %%%%%% ::.                #"<<std::endl;
            std::cout<<"#      ::  ::::::  :::::::I)         #"<<std::endl;
            std::cout<<"#      (%  ::::::         |          #"<<std::endl;
            std::cout<<"#      /   |   /_____     |          #"<<std::endl;
            std::cout<<"#     /    |         ))   |          #"<<std::endl;
            std::cout<<"#    /      ------/ //    |          #"<<std::endl;
            std::cout<<"#   /            / //     |          #"<<std::endl;
            std::cout<<"#  /            / //      |          #"<<std::endl;
            std::cout<<"# *            ZZZZ       *          #"<<std::endl;
            std::cout<<"#    _________ZZZZZZ_________//_//   #"<<std::endl;
            std::cout<<"#------------------------------------#"<<std::endl;
            std::cout<<"######################################"<<std::endl;

            //chemin interessant (4.5)==kruskal
            chemin_interessant();
            valide=false;

            break;
        case 6:
            system("cls");
            std::cout<<"##################################################"<<std::endl;
            std::cout<<"#                /----|       .         .        #"<<std::endl;
            std::cout<<"#  .            /     [   .        .         .   #"<<std::endl;
            std::cout<<"#         ______|---- _|__     .        .        #"<<std::endl;
            std::cout<<"#.     _--    --\\_<\\_//   \\-----           .     #"<<std::endl;
            std::cout<<"#     _  _--___   \\__/     ___  -----_ **     *  #"<<std::endl;
            std::cout<<"#*  _- _-      --_         /  [ ----__  --_  *   #"<<std::endl;
            std::cout<<"#*/__-      .    [           _[  *** --_  [*     #"<<std::endl;
            std::cout<<"#  [*/ .          __[/-----__/   [**     [*/     #"<<std::endl;
            std::cout<<"#        .     /--  /            /               #"<<std::endl;
            std::cout<<"#     .        /   /   /[----___/        .       #"<<std::endl;
            std::cout<<"#             /   /*[  !   /==/              .   #"<<std::endl;
            std::cout<<"# .          /   /==[   |/==/      .             #"<<std::endl;
            std::cout<<"#          _/   /=/ | _ |=/   .               .  #"<<std::endl;
            std::cout<<"#         /_   //  / _ _//              .        #"<<std::endl;
            std::cout<<"#.        [ '//    |__//    .    .            .  #"<<std::endl;
            std::cout<<"#        /==/  .  /==/                .          #"<<std::endl;
            std::cout<<"#      /==/     /==/                       .     #"<<std::endl;
            std::cout<<"#     /==/     /==/       .       .    .         #"<<std::endl;
            std::cout<<"#  _/==/    _/==/            .                   #"<<std::endl;
            std::cout<<"# [|*      [|*                   White Powder!!  #"<<std::endl;
            std::cout<<"##################################################"<<std::endl;
            //info listes sommets et arcs
            AffichageSommet();
            AffichageArc();
            valide=false;
            break;
        case 7:
            system("cls");
            std::cout<<"#####################################################################"<<std::endl;
            std::cout<<"# -   -       -   -     -  -  -    -     -       -   -   -  -    -  #"<<std::endl;
            std::cout<<"#- -   (  -      -    -  -   -   -    -   -    -   -   -    -     - #"<<std::endl;
            std::cout<<"#-  (     )   (   |\\  -  -       -     -      -    -  --    -  - -  #"<<std::endl;
            std::cout<<"# -   ) -  -   )  ) \\  -       _  .---.  -    -    -    -      -   -#"<<std::endl;
            std::cout<<"#-  -   -    -  (/   \\   -  - (_)-.   _\\ - -    -     -  -      -  -#"<<std::endl;
            std::cout<<"# -     .--.  -  \\   | -     -     \\ /u( -   -   -      -      -   -#"<<std::endl;
            std::cout<<"#   .-./ ___\\    /\\  )    -   -  - _\\_-/   -     -   -   -  -    -  #"<<std::endl;
            std::cout<<"# - `-'\\|. .| - // \\/   -  -   -  /    \\     -     --       -    - -#"<<std::endl;
            std::cout<<"#  -   __\\-/__ (_)    -  -   - - /  \\ \\ \\___    -       -    -   -  #"<<std::endl;
            std::cout<<"# -   (  \\:/  // | -  --  -  -  /   /\\ \\___/)     -  -  -    -   -.-#"<<std::endl;
            std::cout<<"#   -  \\ \\:  //V /  -  -     - /`._/ -\\___(_) \\        -   -   .-'  #"<<std::endl;
            std::cout<<"#_-  - (\\ \\_//`-' - -  -  -    |\\  \\  -      \\ \\  -  -    -   /     #"<<std::endl;
            std::cout<<"# `. - (_\\(_)) -    -   -  - - | \\  \\   -  -  \\ \\___ .-. _   |      #"<<std::endl;
            std::cout<<"#   \\  \\  // /  -  -   -   -  /  /)  )-     -  \\/__ (  _( ) /       #"<<std::endl;
            std::cout<<"#    \\ |  |  | -  -    -  -  /  //  /   - -   -|   `-.(   )'        #"<<std::endl;
            std::cout<<"#     `--------------------------------------------------'          #"<<std::endl;
            std::cout<<"#####################################################################"<<std::endl;
            //flots(4.6)
            valide=false;
            break;
        case 8:
            //Quitter
            std::cout<<"#####################################################################"<<std::endl;
            std::cout<<"#                     ,---.           ,---.                         #"<<std::endl;
            std::cout<<"#                    / /*`.\\.--***--./,'*\\ \\                        #"<<std::endl;
            std::cout<<"#                    \\ \\    _       _    / /                        #"<<std::endl;
            std::cout<<"#                     `./  / __   __ \\  \\,'                         #"<<std::endl;
            std::cout<<"#                      /    /_O)_(_O\\    \\                          #"<<std::endl;
            std::cout<<"#                      |  .-'  ___  `-.  |                          #"<<std::endl;
            std::cout<<"#                   .--|       \\_/       |--.                       #"<<std::endl;
            std::cout<<"#                 ,'    \\   \\   |   /   /    `.                     #"<<std::endl;
            std::cout<<"#                /       `.  `--^--'  ,'       \\                    #"<<std::endl;
            std::cout<<"#             .-*****-.    `--.___.--'     .-*****-.                #"<<std::endl;
            std::cout<<"#.-----------/         \\------------------/         \\--------------.#"<<std::endl;
            std::cout<<"#| .---------\\         /----------------- \\         /------------. |#"<<std::endl;
            std::cout<<"#| |          `-`--`--'                    `--'--'-'             | |#"<<std::endl;
            std::cout<<"#| |                                                             | |#"<<std::endl;
            std::cout<<"#| |                                                             | |#"<<std::endl;
            std::cout<<"#| |                PROFITEZ BIEN DE VOTRE SEJOUR !!             | |#"<<std::endl;
            std::cout<<"#| |                                                             | |#"<<std::endl;
            std::cout<<"#| |                                                             | |#"<<std::endl;
            std::cout<<"#| |                                                             | |#"<<std::endl;
            std::cout<<"#| |                        A TRES BIENTOT !!                    | |#"<<std::endl;
            std::cout<<"#| |                                                             | |#"<<std::endl;
            std::cout<<"#| |                                                             | |#"<<std::endl;
            std::cout<<"#| |                                                             | |#"<<std::endl;
            std::cout<<"#| |                                                             | |#"<<std::endl;
            std::cout<<"#| |                                                             | |#"<<std::endl;
            std::cout<<"#| |_____________________________________________________________| |#"<<std::endl;
            std::cout<<"#|_________________________________________________________________|#"<<std::endl;
            std::cout<<"#                   )__________|__|__________(                      #"<<std::endl;
            std::cout<<"#                  |            ||            |                     #"<<std::endl;
            std::cout<<"#                  |____________||____________|                     #"<<std::endl;
            std::cout<<"#                    ),-----.(      ),-----.(                       #"<<std::endl;
            std::cout<<"#                  ,'   ==.   \\    /  .==    `.                     #"<<std::endl;
            std::cout<<"#                 /            )  (            \\                    #"<<std::endl;
            std::cout<<"#                 `==========='    `==========='                    #"<<std::endl;
            std::cout<<"#####################################################################"<<std::endl;

            boucle=true;
            break;

        }

    }


//HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);//Ici, on récupère un handle vers ce qui représente la sortie standard sous Windows.
//SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_INTENSITY);
//Ici, on change les attributs. Tu as droit aux 3 couleurs primaire informatique (rouge, vert et bleu)
//que tu peux mélanger entre elle pour faire plus de couleurs (par exemple vert + bleu = jaune)...Tu peux coupler ces mélanges de couleur avec une intensité de couleur
// plus élevé avec INTENSITY...Pour l'exemple du jaune, ca passerait d'un jaune sale à un jaune lumineux. Ici, ça met du blanc flashy sur fond rouge flashy.

    /*std::cout << "La boutique se compose donc de : " << std::endl << std::endl;
        SetConsoleTextAttribute(hConsole, 12);
        std::cout << "Cartes CREATURES";
        SetConsoleTextAttribute(hConsole, 2);
        std::cout<<"                                Cartes ENERGIE";
        SetConsoleTextAttribute(hConsole, 6);
        std::cout<<"                             Cartes Speciales" << std::endl << std::endl;
        SetConsoleTextAttribute(hConsole, 7);*/
}






/*SOURCES: Art by Joan Stark--> https://www.asciiart.eu/nature/mountains
                            --> https://www.asciiart.eu/sports-and-outdoors/skiing
                            --> https://www.asciiart.eu/nature/snows
                          -->   https://www.asciiart.eu/holiday-and-events/christmas/snowmen*/


void Graphe::donneOrientation() //ou "affecteOrientation" car on ne fait que set l'orientation, on ne la retrourne pas
{
    int numSommetActuel=1;//compteur
    for (auto elem : m_sommet) //pour parcourir 37 sommets
    {
        for (auto elem : m_arc)
        {
            if(elem->getSommetAdj().first->getNumero()==numSommetActuel) //si le sommet actuel de la boucle est l'ext1, alors on a un arc sortant
            {
                elem->setOrientation(1); //1 pour sortant
            }
            else if (elem->getSommetAdj().second->getNumero()==numSommetActuel) //sinon (si le sommet actuel de la boucle est l'ext2, alors il est entrant)
                elem->setOrientation(0); //0 pour entrant
            numSommetActuel++;
        }
    }


}

///Trouve les successeurs d'un sommet (on peut par exemple passer en paramètre getSommetAdj.first)
///Cette méthode donne les sommets successeurs, or pour le BFS on ne tient pas compte de la pondération et des différents trajets
///du coup on peut virer les doublons (car potentiellement plusieurs trajets entre un sommet et un autre) et faire un set
///puisqu'on ne veut que les sommets.
///Le BFS recherche le plus court chemin en nombre d'arcs, donc 1 seul trajet d'un sommet à l'autre suffit
void Graphe::trouverSuccesseurs(int numeroExt1)
{
    //on appelle donneOrientation pour affecter l'orientation des arcs
    //donneOrientation(); Pas nécessaire finalement

    std::set<Sommet*> listeSucc; //set des successeurs pour éviter doublons


    for(auto elem : m_arc)
    {
        if (elem->getSommetAdj().first->getNumero()==numeroExt1)
        {
            //on ajoute les successeurs du sommet actuel à la liste
            listeSucc.insert(elem->getSommetAdj().second);
            //on l'ajoute aux m_successeurs
            m_sommet[numeroExt1-1]->ajouterSuccesseur(elem->getSommetAdj().second);
        }
    }

    //on affiche les successeurs du sommet
    std::cout<<"Le(s) successeur(s) de "<<numeroExt1<<" est/sont : "<< std::endl;
    for (auto elem : listeSucc)
    {
        std::cout<<elem->getNumero()<<std::endl;
    }

}


///ParcoursBFS
void Graphe::parcoursBFS()
{
    int numeroSommetDeDepart=-1;
    std::vector<Sommet*> listePredecesseurs(m_nbrSommet); //stocke les numéros des prédecesseurs, de taille l'ordre du graphe
    for (int i=0; i<(int)listePredecesseurs.size(); ++i)
        listePredecesseurs[i] = nullptr;


    std::queue <Sommet*> file;  //déclaration d'une file

    //demande à l'utilisateur le point de départ (en le blindant
    std::cout<<"A partir de quel sommet souhaitez vous demarrer ?"<<std::endl;
    std::cout<<std::endl;

    while (numeroSommetDeDepart<=0 || numeroSommetDeDepart>=m_nbrSommet)
    {
        std::cout<<"Saisir un numero valide : ";
        std::cin>>numeroSommetDeDepart;
    }

    for (auto elem : m_sommet)
    {
        if (elem->getNumero()==numeroSommetDeDepart)
        {
            file.push(elem);
            elem->setCouleur(1);
        }
    }

///------------------lowl modificationw-------------///
    for(auto elem : m_sommet)
    {
        trouverSuccesseurs(elem->getNumero());
    }
///fin des modificationws

    while (!(file.empty()))     ///Tant que la file n'est pas vide
    {
        for (auto sommet : file.front()->getSuccesseur())      //parcours des adjacents du premier sommet de la file
        {
            if (sommet->getCouleur () == 0) //si le sommet est blanc
            {
                file.push(sommet); //on l'enfile
                sommet->setCouleur (1); //on le met en gris
                listePredecesseurs [sommet->getNumero()] = file.front() ;
            }
        }

        if (file.front()->getCouleur() == 1) //si le premier sommet de la file est gris
            file.front()->setCouleur(2); //on le passe en noir

        file.pop(); //on défile
    }


    ///------------------------------------PARTIE AFFICHAGE-------------------------------///
    std::cout << "Parcours BFS a partir du sommet " << numeroSommetDeDepart <<std::endl;

    int numPredecesseur =0;

    for (int i=0; i<listePredecesseurs.size(); ++i)  ///Parcours du vecteur de predecesseur
    {
        if (listePredecesseurs[i]!= nullptr)
        {
            std::cout << i;
            numPredecesseur = i;

            while (listePredecesseurs[numPredecesseur]!=nullptr)     ///Tant que il existe des pr�decesseurs
            {
                std::cout << " <-- " << listePredecesseurs[numPredecesseur]->getNumero();
                numPredecesseur = listePredecesseurs[numPredecesseur]->getNumero();
            }
            std::cout << std::endl;
        }
        else
        {
            cout<<"Sommet d'arrivee confondu avec le sommet de depart."<<endl;
        }
    }
    std::cout << std::endl;
}
