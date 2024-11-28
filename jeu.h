#ifndef JEU_H
#define JEU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <windows.h>
#include <conio.h>

#ifndef DIRECTION_ENUM
#define DIRECTION_ENUM

enum direction { DOWN,RIGHT};
#endif


#ifndef VEHICULE_STRUCT
#define VEHICULE_STRUCT
typedef struct {
    int x_curr;
    int y_curr;
    int x_prec;
    int y_prec;
    enum direction d;
    int status;
} vehicule;
#endif

#ifndef FEU_STRUCT
#define FEU_STRUCT
typedef struct {
    int x;
    int y;
    char etat;
} feu;
#endif

#ifndef MAP_STRUCT
#define MAP_STRUCT
typedef struct {
    int n_row;
    int n_col;
    int n_veh;
    int **map_binary;
    vehicule *table_de_vehicules;
    int n_horiz_routes;
    int n_vert_routes;
    int n_feux;
    feu *feux_positions;
} map;
#endif

void deplacer_vehicule(map *m );
int valid_vehicule(map *m, int vehicule_index);
int is_path_clear(map *m, int x, int y);
int valid_vehicule(map *m, int vehicule_index);
int all_vehicles_out(map *m);
void move_right(map *m, int vehicule_index);
void move_down(map *m, int vehicule_index);
void deplacer_vehicule(map *m);
void placer_feux(map *m);

#endif // JEU_H
