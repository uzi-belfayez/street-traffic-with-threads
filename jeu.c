#include "jeu.h"


int main(){

    map m ;
    m.n_col = 0;
    m.n_row = 0;
    m.n_veh = 0;

    srand(time(NULL));

    lire_map("map.txt",&m);
    generate_binary_map(&m);
    placer_vehicule(&m);
    afficher_map(m);
    print_binary_map(&m);

    return 0;
}
