#include "jeu.h"

int is_within_bounds(map *m, int x, int y) {
    return x >= 0 && x < m->n_col && y >= 0 && y < m->n_row;
}

//checks if the next box is valid or not
int is_path_clear(map *m, int x, int y) {
    int mid_row = m->n_row / 2;
    int mid_col = m->n_col / 2;
    if (y == mid_col) {
        if (x + 1 < m->n_row && m->map_binary[x + 1][y] != 3) {
            return 1;
        }
    } else if (x==mid_row) {
        if (y + 1 < m->n_col && m->map_binary[x][y + 1] != 3) {
            return 1;
        }
    }
    return 0;
}
//checks if the road is empty so the vehicule can start to move
int valid_vehicule(map *m, int vehicule_index) {
    int mid_row = m->n_row / 2;
    int mid_col = m->n_col / 2;
    int res = 1 ;
    int x = m->table_de_vehicules[vehicule_index].x_curr;
    int y = m->table_de_vehicules[vehicule_index].y_curr;
    if (y == mid_col){
        for(int i = x+1 ; i < m->n_row-1 ; i++ ){
            res = res && is_path_clear(m,x,y);
            x ++ ;
        }
    }
    else if ( x == mid_row){
        for(int i = y+1 ; i < m->n_col-1 ; i++ ){
            res = res && is_path_clear(m,x,y);
            y ++ ;
        }
    }

    m->table_de_vehicules[vehicule_index].status = res ;
    return res;


}


int all_vehicles_out(map *m) {
    for (int i = 0; i < m->n_veh; i++) {
        if (valid_vehicule(m, i)) {
            return 0;
        }
    }
    return 1;
}

void move_down(map *m, int vehicule_index) {
    if (m->table_de_vehicules[vehicule_index].status == 0) {
        return; // Vehicle is inactive, do nothing
    }

    int x = m->table_de_vehicules[vehicule_index].x_curr;
    int y = m->table_de_vehicules[vehicule_index].y_curr;

    if (x + 1 >= m->n_row) {
        m->table_de_vehicules[vehicule_index].status = 0;
        m->map_binary[x][y] == 1;
        return;
    }

   // m->table_de_vehicules[vehicule_index].x_prec = x;
   // m->table_de_vehicules[vehicule_index].y_prec = y;
    m->table_de_vehicules[vehicule_index].x_curr ++;

    m->map_binary[x][y] = 0;  // Clear old position
    m->map_binary[m->table_de_vehicules[vehicule_index].x_curr][y] = 3;
}

void move_right(map *m, int vehicule_index) {
    if (m->table_de_vehicules[vehicule_index].status == 0) {
        return;
    }

    int x = m->table_de_vehicules[vehicule_index].x_curr;
    int y = m->table_de_vehicules[vehicule_index].y_curr;

    if (y + 1 >= m->n_col) {
        m->table_de_vehicules[vehicule_index].status = 0;
        m->map_binary[x][y] == 1;
        return;
    }

   // m->table_de_vehicules[vehicule_index].x_prec = x;
   // m->table_de_vehicules[vehicule_index].y_prec = y;
    m->table_de_vehicules[vehicule_index].y_curr ++;

    m->map_binary[x][y] = 0;  // Clear old position
    m->map_binary[x][m->table_de_vehicules[vehicule_index].y_curr] = 3;
}

void deplacer_vehicule(map *m) {
    while (!all_vehicles_out(m)) {
        for (int i = 0; i < m->n_veh; i++) {
            if (valid_vehicule(m, i)) {
                if (m->table_de_vehicules[i].d == RIGHT) {
                    move_right(m, i);
                } else if(m->table_de_vehicules[i].d == DOWN)   {
                    move_down(m, i);
                }
                afficher_map(m);
                sleep(1);
            }

        }
    }
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
    for(int i = 0 ; i <10 ; i++){
        //v = valid_vehicule(&m, i);

        //v = is_path_clear(&m,m.table_de_vehicules[i].x_curr,m.table_de_vehicules[i].y_curr);
        v = valid_vehicule(&m,i);

        printf("%d", v);
    }

    deplacer_vehicule(&m);


   // printf("\n");


   // print_binary_map(&m);
    //clearScreen();
    //afficher_map(m);
    return 0;
}
