#ifndef JEU_H
#define JEU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <ncurses.h>
#endif

#ifndef DIRECTION_ENUM
#define DIRECTION_ENUM

enum direction { UP,DOWN,RIGHT,LEFT };
#endif


#ifndef VEHICULE_STRUCT
#define VEHICULE_STRUCT
typedef struct {
    int x_curr;
    int y_curr;
    int x_prec;
    int y_prec;
    enum direction d;
} vehicule;
#endif

#ifndef MAP_STRUCT
#define MAP_STRUCT
typedef struct {
    int n_row;
    int n_col;
    int n_veh;
    int **map_binary;
    vehicule *table_de_vehicules;
} map;
#endif

void deplacer_vehicule(map *m );
int valid_vehicule(map *m, int vehicule_index);

#endif // JEU_H
