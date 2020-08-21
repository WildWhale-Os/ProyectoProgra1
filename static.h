#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>


#define TAM 40
#define WIDTH 12
#define HEIGHT 17
#define menuNum 4
#define buttonNum 4
#define delta 1e-10

SDL_Window* screen = NULL;
SDL_Renderer* renderer = NULL;
SDL_Color blanco = { 255, 255, 255, 255 };
SDL_Color blancoC = { 255, 255, 255, 255 };
Mix_Music* bgm; //coment: declaramos las variables que guardaran la musica y efectos de sonido
Mix_Music* GO;
Mix_Chunk* sfx;
Mix_Chunk* bot;
Mix_Chunk* bot2;
Mix_Chunk* pausa;

int combo = 4;
int VolM = 20; //coment declaramos el volumen
int mute = 0;  //coment declaramos una variable para silenciar el audio

char* colors[] = {
    "assets/Cubos/Block1.png",
    "assets/Cubos/Block2.png",
    "assets/Cubos/Block3.png",
    "assets/Cubos/Block4.png",
    "assets/Cubos/Block5.png",
    "assets/Cubos/Block7.png",
    "assets/Cubos/Block6.png",
    "assets/Cubos/Block8.png"
};

typedef struct
{
    SDL_Texture* pos[HEIGHT][WIDTH];
} Tablero;

typedef struct
{
    double x;
    double y;
} Coor;

typedef struct
{
    Coor central;
    Coor laterales[3];
    SDL_Texture* images;

} Piezas;

typedef struct
{
    char nombre[30];
    long long puntaje;
} Records;

typedef struct
{
    char* sPuntos;
    long long puntos;
    long long lineasEliminadas;
    char* slineas;
} TableroPuntaje;

Coor Tetraminos[][3] = {
    { { 0, -1 }, { 0, 1 }, { 1, 1 } },  //L
    { { -1, 0 }, { 0, 1 }, { 1, 0 } },  //T
    { { 0, -1 }, { 0, 1 }, { -1, 1 } }, // L inversa
    { { 1, 0 }, { 0, 1 }, { 1, 1 } },   // cuadradro
    { { -1, 0 }, { 0, 1 }, { 1, 1 } },  // Z
    { { -1, 1 }, { 0, 1 }, { 1, 0 } },  // Z Inversa
    { { -1, 0 }, { 1, 0 }, { 2, 0 } },  // |

};

typedef struct
{
    Tablero tablero;
    TableroPuntaje score;
    FILE* DB;
    Records top10[10];
    Piezas actFigure;
    Piezas nextFigure;
    SDL_Texture* fondos[10];
} Tetris;