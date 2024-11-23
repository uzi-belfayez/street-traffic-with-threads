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

int valid_cordinates(map *m){
    int i,j;
    int x = m->table_de_vehicules[i].x_curr ;
    int y = m->table_de_vehicules[i].y_curr ;
    for(i = 0 ; i < m->n_veh ; i++){
                for(j = i+1 ; j < m->n_veh ; j++){
                    if ( ( (x != m->table_de_vehicules[j].x_curr) || (y != m->table_de_vehicules[j].y_curr ) ) && ( (m->map_binary[x][y] == 1) || (m->map_binary[x][y] == 2) ) )
                    return 1 ;
                    else return -1;
                }
           }
}

void placer_vehicule(map *m){
    int i = 0;
    int valid = 0;
    m->table_de_vehicules = (int *)malloc(m->n_veh * sizeof(int *));

    do{
        m->table_de_vehicules[i].x_curr = rand() % (m->n_col + 1);
        m->table_de_vehicules[i].y_curr = rand() % (m->n_row + 1);
        valid = valid_cordinates(&m);
        i++;
    }while(valid && i<m->n_veh);

}

