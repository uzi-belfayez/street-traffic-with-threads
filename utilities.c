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
        printf( "\n\n\n\n\n\n\n\n\n\n" );
}

void afficher_map(map m){
    int mid_row = m.n_row / 2;
    int mid_col = m.n_col / 2;

    for (int i = 0; i < m.n_row; i++) {
        for (int j = 0; j < m.n_col; j++) {
            if (i == mid_row && j == mid_col) {

                printf("+");
            } else if (i == mid_row) {

                printf("-");
            } else if (j == mid_col) {

                printf("|");
            } else {

                printf(" ");
            }
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
            if (i == mid_row || j == mid_col) {
                m->map_binary[i][j] = 1;
            } else {
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

void placer_vehicule(vehicule *v, map *m){

}

