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
        printf( "\n" );
}

void afficher_map(map m){

    clearScreen();

    int mid_row = m.n_row / 2;
    int mid_col = m.n_col / 2;

    for (int i = 0; i < m.n_row; i++) {
        for (int j = 0; j < m.n_col; j++) {
            if(m.map_binary[i][j] == 2)
                printf("|");
            else if(m.map_binary[i][j] == 1)
                printf("-");
            else if(m.map_binary[i][j] == 3)
                printf("*");
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
    }
}


