#include <iostream>
#include "Graphe.h"

using namespace std;

int main()
{
    Graphe g{"data_arcs.txt"};


   // g.Menu(); //c'est le menu

    g.trouverSuccesseurs(3);

    return 0;
}
