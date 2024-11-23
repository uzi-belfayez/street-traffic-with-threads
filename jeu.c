#include "jeu.h"

int valid_vehicule(map *m, int vehicule_index) {
    int mid_row = m->n_row / 2;
    int mid_col = m->n_col / 2;

    int x = m->table_de_vehicules[vehicule_index].x_curr;
    int y = m->table_de_vehicules[vehicule_index].y_curr;

    if(m->table_de_vehicules[vehicule_index].x_curr == mid_row){
        m->table_de_vehicules[vehicule_index].d= DOWN;
    }
    else m->table_de_vehicules[vehicule_index].d = RIGHT;

    int direction = m->table_de_vehicules[vehicule_index].d;

    if(direction == RIGHT){
        for(int i = x + 1 ; i < m->n_row ; i++){
            if(m->map_binary[i][y] == 3)
                return 0;
        }
        return 1 ;
    }
    else {
          for(int i = y+ 1 ; i < m->n_col ; i++){
            if(m->map_binary[x][i] == 3)
                return 0;
        }
        return 1 ;
    }

    return 0;
}

void deplacer_vehicule(map *m ){

}

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

    int v;
    for(int i = 0 ; i <3 ; i++){
        v = valid_vehicule(&m, i);
        printf("%d", v);
    }

    //print_binary_map(&m);
    //clearScreen();
    //afficher_map(m);
    return 0;
}
