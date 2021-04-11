#include <iostream>
#include "Graphe.h"

using namespace std;

int main()
{
    Graphe g{"data_arcs.txt"};

   // g.chemin_interessant();
   // g.Menu(); //c'est le menu

    g.trouverSuccesseurs(3);
    //g.parcoursBFS();
    return 0;
}
