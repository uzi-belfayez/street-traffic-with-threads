#include "jeu.h"

void lire_map(const char *fichier, map *m) {

    FILE *file = fopen(fichier, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }


    if (fscanf(file, "%d %d", &m->n_row, &m->n_col) != 2) {
        printf("Erreur lors de la lecture des deux premiers nombres.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }


    if (fscanf(file, "%d", &m->n_veh) != 1) {
        printf("Erreur lors de la lecture du troisième nombre.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }


    fclose(file);
}

void clearScreen(){
    int n;
    for (n = 0; n < 10; n++)
        printf( "\n\n\n\n\n\n\n" );
}

void afficher_map(map *m){

    clearScreen();

    for (int i = 0; i < m->n_row; i++) {
        for (int j = 0; j < m->n_col; j++) {
            if(m->map_binary[i][j] == 2)
                printf("|");
            else if(m->map_binary[i][j] == 1)
                printf("-");
            else if(m->map_binary[i][j] == 3)
                printf("*");
            else if(m->map_binary[i][j] == 4)
                printf("R");
            else if(m->map_binary[i][j] == 5)
                printf("V");
            else printf(" ");
        }
        printf("\n");
        }

}


void generate_binary_map(map *m) {

    m->map_binary = (int **)malloc(m->n_row * sizeof(int *));
    for (int i = 0; i < m->n_row; i++) {
        m->map_binary[i] = (int *)malloc(m->n_col * sizeof(int));
    }

    int mid_row = m->n_row / 2;
    int mid_col = m->n_col / 2;


    for (int i = 0; i < m->n_row; i++) {
        for (int j = 0; j < m->n_col; j++) {
            if (j == mid_col) {
                m->map_binary[i][j] = 2;
            }
            else if (i == mid_row){
               m->map_binary[i][j] = 1;
            }
             else {
                m->map_binary[i][j] = 0;
            }
        }
    }
}

void print_binary_map(map *m) {
    for (int i = 0; i < m->n_row; i++) {
        for (int j = 0; j < m->n_col; j++) {
            printf("%d ", m->map_binary[i][j]);
        }
        printf("\n");
    }
}

int valid_cordinates(map *m, int x, int y) {
    if (!(m->map_binary[x][y] == 1 || m->map_binary[x][y] == 2)) {
        return 0;
    }

    return 1;
}

void placer_vehicule(map *m) {
    int mid_row = m->n_row / 2;
    int mid_col = m->n_col / 2;

    m->table_de_vehicules = (vehicule *)malloc(m->n_veh * sizeof(vehicule));

    for (int j = 0; j < m->n_veh; j++) {
        int valid = 0, x, y;

        do {
            // Ensure the vehicle does not spawn on the borders
            x = 1 + rand() % (m->n_row - 2);
            y = 1 + rand() % (m->n_col - 2);
            valid = valid_cordinates(m, x, y);
        } while (!valid);

        m->table_de_vehicules[j].x_curr = x;
        m->table_de_vehicules[j].y_curr = y;
        m->table_de_vehicules[j].status = 1;

        m->map_binary[x][y] = 3;

        if(x == mid_row)
            m->table_de_vehicules[j].d = RIGHT;
        else if (y == mid_col)
            m->table_de_vehicules[j].d = DOWN;
    }
}





// PART 2

void lire_map_v2(const char *fichier, map *m) {
    FILE *file = fopen(fichier, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    if (fscanf(file, "%d %d", &m->n_row, &m->n_col) != 2) {
        printf("Erreur lors de la lecture des dimensions de la carte.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    if (fscanf(file, "%d", &m->n_veh) != 1) {
        printf("Erreur lors de la lecture du nombre de véhicules.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }


    if (fscanf(file, "%d %d", &m->n_horiz_routes, &m->n_vert_routes) != 2) {
        printf("Erreur lors de la lecture des routes horizontales et verticales.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fclose(file);
}

void generate_binary_map_v2(map *m) {

    m->map_binary = (int **)malloc(m->n_row * sizeof(int *));
    for (int i = 0; i < m->n_row; i++) {
        m->map_binary[i] = (int *)malloc(m->n_col * sizeof(int));
    }


    for (int i = 0; i < m->n_row; i++) {
        for (int j = 0; j < m->n_col; j++) {
            m->map_binary[i][j] = 0;
        }
    }


    int horiz_spacing = m->n_row / (m->n_horiz_routes + 1);
    for (int k = 1; k <= m->n_horiz_routes; k++) {
        int row = k * horiz_spacing;
        for (int j = 0; j < m->n_col; j++) {
            m->map_binary[row][j] = 1;
        }
    }


    int vert_spacing = m->n_col / (m->n_vert_routes + 1);
    for (int k = 1; k <= m->n_vert_routes; k++) {
        int col = k * vert_spacing;
        for (int i = 0; i < m->n_row; i++) {
            m->map_binary[i][col] = 2;
        }
    }
}

void placer_vehicule_v2(map *m) {

    m->table_de_vehicules = (vehicule *)malloc(m->n_veh * sizeof(vehicule));

    for (int j = 0; j < m->n_veh; j++) {
        int valid = 0, x, y;

        do {

            x = rand() % m->n_row;
            y = rand() % m->n_col;


            if (m->map_binary[x][y] == 1 || m->map_binary[x][y] == 2) {
                valid = 1;


                for (int k = 0; k < j; k++) {
                    if ((m->table_de_vehicules[k].x_curr == x &&
                        m->table_de_vehicules[k].y_curr == y )||
                        m->map_binary[x][y] == 4 || m->map_binary[x][y] == 5)   {
                        valid = 0;
                        break;
                    }
                }
            }
        } while (!valid);


        m->table_de_vehicules[j].x_curr = x;
        m->table_de_vehicules[j].y_curr = y;
        m->table_de_vehicules[j].status = 1;


        if (m->map_binary[x][y] == 1)
            m->table_de_vehicules[j].d =RIGHT;
        else if (m->map_binary[x][y] == 2)
            m->table_de_vehicules[j].d = DOWN;


        m->map_binary[x][y] = 3;


    }
}

void placer_feux(map *m) {
    m->n_feux = 0;


    m->feux_positions = (feu *)malloc(m->n_row * m->n_col * sizeof(feu));

    for (int i = 1; i < m->n_row - 1; i++) {
        for (int j = 1; j < m->n_col - 1; j++) {
            // Check if the current point is part of an intersection
            if ((m->map_binary[i][j] == 1 &&
                 (m->map_binary[i - 1][j] == 2 || m->map_binary[i - 1][j] == 3) &&
                 (m->map_binary[i + 1][j] == 2 || m->map_binary[i + 1][j] == 3 )) ||
                (m->map_binary[i][j] == 2 &&
                 (m->map_binary[i][j - 1] == 1 || m->map_binary[i][j - 1] == 3 ) &&
                 (m->map_binary[i][j + 1] == 1 || m->map_binary[i][j + 1] == 3))
                ) {
                m->feux_positions[m->n_feux].etat = 'R';


                if(m->feux_positions[m->n_feux].etat == 'R')
                    m->map_binary[i][j] = 4;
                else if(m->feux_positions[m->n_feux].etat == 'V')
                     m->map_binary[i][j] = 5;

                m->feux_positions[m->n_feux].x = i;
                m->feux_positions[m->n_feux].y = j;

                m->n_feux++;
            }
        }
    }


    m->feux_positions = realloc(m->feux_positions, m->n_feux * sizeof(feu));
}
//final

void placer_feux_pf(map *m) {
    m->n_feux = 0;

    // Allocate memory for the maximum possible number of traffic lights
    m->feux_positions = (feu *)malloc(m->n_row * m->n_col * sizeof(feu));

    for (int i = 1; i < m->n_row - 1; i++) {
        for (int j = 1; j < m->n_col - 1; j++) {
            // Check if the current point is part of an intersection
            if ((m->map_binary[i][j] == 1 &&
                 (m->map_binary[i - 1][j] == 2 || m->map_binary[i - 1][j] == 3) &&
                 (m->map_binary[i + 1][j] == 2 || m->map_binary[i + 1][j] == 3)) ||
                (m->map_binary[i][j] == 2 &&
                 (m->map_binary[i][j - 1] == 1 || m->map_binary[i][j - 1] == 3) &&
                 (m->map_binary[i][j + 1] == 1 || m->map_binary[i][j + 1] == 3))) {

                // Place a traffic light on the vertical edge of the intersection
                m->feux_positions[m->n_feux].etat = 'R'; // Red
                m->feux_positions[m->n_feux].x = i-1;  // Above the intersection
                m->feux_positions[m->n_feux].y = j-1;
                m->map_binary[i - 1][j-1] = 4; // Mark on the map as vertical light
                m->n_feux++;

                // Place a traffic light on the horizontal edge of the intersection
                m->feux_positions[m->n_feux].etat = 'V'; // Blue
                m->feux_positions[m->n_feux].x = i + 1;
                m->feux_positions[m->n_feux].y = j + 1; // Left of the intersection
                m->map_binary[i + 1][j + 1] = 5; // Mark on the map as horizontal light
                m->n_feux++;
            }
        }
    }

    // Reallocate memory to fit the actual number of traffic lights
    m->feux_positions = realloc(m->feux_positions, m->n_feux * sizeof(feu));
}












