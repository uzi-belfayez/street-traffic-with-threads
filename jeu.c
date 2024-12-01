#include "jeu.h"

map m_global;
pthread_mutex_t map_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t feux_mutex = PTHREAD_MUTEX_INITIALIZER;

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

    int x = m->table_de_vehicules[vehicule_index].x_curr;
    int y = m->table_de_vehicules[vehicule_index].y_curr;



    m->table_de_vehicules[vehicule_index].x_curr ++;

    m->map_binary[x][y] = 2;
    m->map_binary[m->table_de_vehicules[vehicule_index].x_curr][y] = 3;
}

void move_right(map *m, int vehicule_index) {


    int x = m->table_de_vehicules[vehicule_index].x_curr;
    int y = m->table_de_vehicules[vehicule_index].y_curr;



    m->table_de_vehicules[vehicule_index].y_curr ++;

    m->map_binary[x][y] = 1;  // Clear old position
    m->map_binary[x][m->table_de_vehicules[vehicule_index].y_curr] = 3;
}


void deplacer_vehicule(map *m) {
    while (!all_vehicles_out(m)){
        int v;
        for (int i = 0; i < m->n_veh; i++) {


        while (valid_vehicule(m,i)) {
            if (m->table_de_vehicules[i].d == RIGHT) {
                if (m->table_de_vehicules[i].y_curr < m->n_col - 1) {
                    move_right(m, i);
                } else {
                    m->table_de_vehicules[i].status = 0;
                    m->map_binary[m->table_de_vehicules[i].x_curr][m->table_de_vehicules[i].y_curr] = 1;
                    break;
                }
            } else if (m->table_de_vehicules[i].d == DOWN) {
                if (m->table_de_vehicules[i].x_curr < m->n_row - 1) {
                    move_down(m, i);
                } else {
                    m->table_de_vehicules[i].status = 0;
                    m->map_binary[m->table_de_vehicules[i].x_curr][m->table_de_vehicules[i].y_curr] = 2;
                    break;
                }
            }
            afficher_map(m);
            sleep(1);
        }

    }
    afficher_map(m);
    sleep(1);
    }

}

//PART 2

int valid_vehicule_p2(map *m, int vehicule_index) {
    int x = m->table_de_vehicules[vehicule_index].x_curr;
    int y = m->table_de_vehicules[vehicule_index].y_curr;

    // Vérification si le véhicule quitte la carte
    if (x >= m->n_row || y >= m->n_col)
        return 0;

    if (m->table_de_vehicules[vehicule_index].d == RIGHT) {
        if (y + 1 < m->n_col && m->map_binary[x][y + 1] != 3) {
            return 1; // Mouvement possible
        }
    } else if (m->table_de_vehicules[vehicule_index].d == DOWN) {
        if (x + 1 < m->n_row && m->map_binary[x + 1][y] != 3) {
            return 1; // Mouvement possible
        }
    }

    return 0; // Mouvement bloqué ou hors carte
}


void move_down_p2(map *m, int vehicule_index) {
    int x = m->table_de_vehicules[vehicule_index].x_curr;
    int y = m->table_de_vehicules[vehicule_index].y_curr;

    // Vérifier si le véhicule quitte la carte
    if (x + 1 >= m->n_row) {
        m->map_binary[x][y] = 2; // Trace verticale : '|'
        return;
    }

    m->table_de_vehicules[vehicule_index].x_curr++;

    m->map_binary[x][y] = 2; // Libère la position précédente
    m->map_binary[m->table_de_vehicules[vehicule_index].x_curr][y] = 3; // Position actuelle
}

void move_right_p2(map *m, int vehicule_index) {
    int x = m->table_de_vehicules[vehicule_index].x_curr;
    int y = m->table_de_vehicules[vehicule_index].y_curr;

    // Vérifier si le véhicule quitte la carte
    if (y + 1 >= m->n_col) {
        m->map_binary[x][y] = 1; // Trace horizontale : '-'
        return;
    }

    m->table_de_vehicules[vehicule_index].y_curr++;

    m->map_binary[x][y] = 1; // Libère la position précédente
    m->map_binary[x][m->table_de_vehicules[vehicule_index].y_curr] = 3; // Position actuelle
}


void *gestion_feu(void *arg) {
    feu *f = (feu *)arg;

    while (1) {
        pthread_mutex_lock(&feux_mutex); // Lock before modifying state
        printf("Feu at (%d, %d) is %c. Switching state...\n", f->x, f->y, f->etat);
        if (f->etat == 'R') {
            f->etat = 'V'; // Green
        } else {
            f->etat = 'R'; // Red
        }
        printf("Feu at (%d, %d) is now %c.\n", f->x, f->y, f->etat);
        pthread_mutex_unlock(&feux_mutex); // Unlock after modification

        sleep(5); // Wait for 5 seconds before switching again
    }
    return NULL;
}


void start_feux_threads(map *m) {
    printf("Creating threads for %d feux...\n", m->n_feux);
    m->threads_feux = (pthread_t *)malloc(m->n_feux * sizeof(pthread_t));
    if (!m->threads_feux) {
        printf("Failed to allocate memory for threads_feux.\n");
        exit(1);
    }

    for (int i = 0; i < m->n_feux; i++) {
        printf("Starting thread for feu %d at (%d, %d)\n",
               i, m->feux_positions[i].x, m->feux_positions[i].y);
        if (pthread_create(&m->threads_feux[i], NULL, gestion_feu, (void *)&m->feux_positions[i]) != 0) {
            printf("Failed to create thread for feu %d\n", i);
            exit(1);
        }
    }
    printf("All threads started.\n");
}


void afficher_map_v2(map *m) {
    pthread_mutex_lock(&feux_mutex); // Lock to safely access shared feux_positions
    clearScreen();

    for (int i = 0; i < m->n_row; i++) {
        for (int j = 0; j < m->n_col; j++) {
            char output = ' '; // Default output for empty spaces

            if (m->map_binary[i][j] == 1) {
                output = '-'; // Horizontal road
            } else if (m->map_binary[i][j] == 2) {
                output = '|'; // Vertical road
            } else if (m->map_binary[i][j] == 3) {
                output = '*'; // Vehicle
            } else if (m->map_binary[i][j] == 4) { // Check for traffic light
                // Find the corresponding traffic light in feux_positions
                for (int k = 0; k < m->n_feux; k++) {
                    if (m->feux_positions[k].x == i && m->feux_positions[k].y == j) {
                        output = m->feux_positions[k].etat; // 'R' or 'V'
                        break;
                    }
                }
            }

            printf("%c", output);
        }
        printf("\n");
    }

    pthread_mutex_unlock(&feux_mutex); // Unlock after accessing shared feux_positions
}


void *deplacer_vehicule_p2(void *arg) {
    int vehicule_index = *((int *)arg);
    free(arg); // Free dynamically allocated memory for index
    printf("Thread started for vehicle %d\n", vehicule_index);

    while (1) {
        pthread_mutex_lock(&m_global.lock); // Lock the map for safe updates

        int x = m_global.table_de_vehicules[vehicule_index].x_curr;
        int y = m_global.table_de_vehicules[vehicule_index].y_curr;

        // Arrêter le déplacement si le véhicule est sorti
        if (x == -1 || y == -1) {
            pthread_mutex_unlock(&m_global.lock);
            break;
        }

        // Déterminer la direction et tenter le mouvement
        if (m_global.table_de_vehicules[vehicule_index].d == RIGHT) {
            if (valid_vehicule_p2(&m_global, vehicule_index)) {
                move_right(&m_global, vehicule_index);
            } else {
                m_global.map_binary[x][y] = 1;
                pthread_mutex_unlock(&m_global.lock);
                sleep(1); // Attendre avant de réessayer
                continue;
            }
        } else if (m_global.table_de_vehicules[vehicule_index].d == DOWN) {
            if (valid_vehicule_p2(&m_global, vehicule_index)) {
                move_down(&m_global, vehicule_index);
            } else {
                m_global.map_binary[x][y] = 2;
                pthread_mutex_unlock(&m_global.lock);
                sleep(1); // Attendre avant de réessayer
                continue;
            }
        }

        pthread_mutex_unlock(&m_global.lock);
        sleep(1); // Ralentir le mouvement pour plus de visibilité
    }

    pthread_exit(NULL);
}

void lancer_deplacement(map *m) {
    pthread_mutex_init(&m->lock, NULL); // Initialize the mutex lock

    for (int i = 0; i < m->n_veh; i++) {
        pthread_t thread_id;
        int *vehicule_index = malloc(sizeof(int));
        if (!vehicule_index) {
            perror("Failed to allocate memory for vehicle index");
            exit(EXIT_FAILURE);
        }

        *vehicule_index = i; // Pass the vehicle index to the thread

        if (pthread_create(&thread_id, NULL, deplacer_vehicule_p2, vehicule_index) != 0) {
            perror("Failed to create thread");
            exit(EXIT_FAILURE);
        }

        sleep(1); // Delay for 1 second before creating the next thread
    }
}


int main(){
// PART 1
/*
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
    for(int i = 0 ; i <m.n_veh ; i++){

        v = valid_vehicule(&m,i);

        printf("%d", v);
    }

    deplacer_vehicule(&m);

*/

// PART 2
  /*  map m;
    m.n_col = 0;
    m.n_row = 0;
    m.n_veh = 0;

    pthread_t g_feu;

    srand(time(NULL));

    lire_map_v2("map.txt", &m);

    generate_binary_map_v2(&m);
    placer_feux(&m);
    placer_vehicule_v2(&m);
    afficher_map_v2(&m);

    for(int i = 0 ; i < m.n_veh ; i++){
        printf("%d", m.table_de_vehicules[i].d);
    }
*/


 /* printf("%d \n", m.n_feux);

    for (int i = 0 ; i<m.n_feux ; i++){
        printf("(%d,%d) %c\n",m.feux_positions[i].x,m.feux_positions[i].y,m.feux_positions[i].etat );
    } */

   /* start_feux_threads(&m);

       while (1) {
        afficher_map_v2(&m);
        sleep(1);
    } */


    pthread_t g_feu;

    srand(time(NULL));

    lire_map_v2("map.txt", &m_global);

    generate_binary_map_v2(&m_global);
    placer_feux(&m_global);
    placer_vehicule_v2(&m_global);
    afficher_map_v2(&m_global);

    for(int i = 0 ; i < m_global.n_veh ; i++){
        printf("%d", m_global.table_de_vehicules[i].d);
    }


pthread_mutex_init(&m_global.lock, NULL);
  lancer_deplacement(&m_global);

    while (1) {
        //printf("entered the while loop \n");
        system("cls");
        afficher_map_v2(&m_global);
        sleep(1);
        //printf("ended sleeping\n");
    }
pthread_mutex_destroy(&m_global.lock);

    return 0;
}
