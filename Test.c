//
// Created by tomasb on 7/2/20.
//
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>

#define TAM 40
#define WIDTH 12
#define HEIGHT 17
SDL_Window *screen = NULL;
SDL_Renderer *renderer = NULL;

typedef struct {
    SDL_Texture *pos[HEIGHT][WIDTH];
} Tablero;

typedef struct {
    int x;
    int y;
} Coor;

typedef struct {
    Coor central;
    Coor laterales[3];
    SDL_Texture *images;

} Piezas;
Coor Tetraminos[][3] = {
        {{0,  -1}, {0, 1}, {1,  1}}, //L
        {{-1, 0},  {0, 1}, {1,  0}}, //T
        {{0,  -1}, {0, 1}, {-1, 1}}, // L inversa
        {{1,  0},  {0, 1}, {1,  1}}, // cuadradro
        {{-1, 0},  {0, 1}, {1,  1}}, // Z
        {{-1, 1},  {0, 1}, {1,  0}}, // Z Inversa
        {{0,  -1}, {0, 1}, {0,  2}}, // |

};

void Init();

void DrawCubes(SDL_Renderer *renderer, int x, int y, SDL_Texture *image);

void DrawPlayGround(SDL_Renderer *renderer, Tablero *t);

void DrawFigure(SDL_Renderer *renderer, Piezas *pieza);

void CrearPieza(Piezas *p, SDL_Texture *image);

Coor PiezaPos(int n, Piezas *p);

void CambioCoord(Coor *coordenadas);

void RotarPieza(Piezas *pieza);

void LimpiarTablero(Tablero *t, SDL_Texture *images[]);

void DetensionPieza(Tablero *t, Piezas *p);

int HayColision(Tablero *t, Piezas *p);

SDL_Texture *LoadTexture(char *path, SDL_Renderer *renderer);

int FilaCompleta(Tablero *t, int fila);

void EliminarFila(Tablero *t, int fila);

int LineasELiminadas(Tablero *t);

void Close(SDL_Renderer *renderer, SDL_Window *window, SDL_Texture *textura[]);

int main() {
    srand(time(NULL));
    Init();
    Tablero t;
    SDL_Texture *texturas[] = {LoadTexture("assets/VerticalWall.png", renderer),    //0
                               LoadTexture("assets/RightCorner.png", renderer), //1
                               LoadTexture("assets/LeftCorner.png", renderer),  //2
                               LoadTexture("assets/HorizontaalWall.png", renderer), //3
                               LoadTexture("assets/blueblock.png", renderer),   //4 (4-10 cubeImages)
                               LoadTexture("assets/greenblcok.png", renderer),  //5
                               LoadTexture("assets/orangeblock.png", renderer), //6
                               LoadTexture("assets/pinkblock.png", renderer),   //7
                               LoadTexture("assets/purpleblock.png", renderer), //8
                               LoadTexture("assets/redblock.png", renderer),    //9
                               LoadTexture("assets/yellowblock.png", renderer)};    //10

    Piezas p;
    CrearPieza(&p, texturas[4 + (rand() % 7)]);
    LimpiarTablero(&t, texturas);
    int lineasEliminadas = 0;
    SDL_Event event;
    int play = 1, x = 6, y = 0;
    while (play) {
        while (SDL_PollEvent(&event) != 0) {
            Piezas aux = p;
            if (event.type == SDL_QUIT)
                play = 0;
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        RotarPieza(&p);
                        break;
                    case SDLK_DOWN:
                        p.central.y++;
                        break;
                    case SDLK_LEFT:
                        p.central.x--;
                        break;
                    case SDLK_RIGHT:
                        p.central.x++;
                        break;
                    case SDLK_SPACE:
                        DetensionPieza(&t, &p);
                        lineasEliminadas += LineasELiminadas(&t);
                        CrearPieza(&p, texturas[4 + (rand() % 7)]);
                        break;

                }
            }
            if (HayColision(&t, &p))
                p = aux;
        }

        SDL_RenderClear(renderer);
        DrawPlayGround(renderer, &t); // ocupa x = 0 , x = WIDTH -1  y = HEIGHT -1
        DrawFigure(renderer, &p);
        SDL_RenderPresent(renderer);
        SDL_UpdateWindowSurface(screen);

    }
    Close(renderer, screen, texturas);
    return 0;
}

void Init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        printf("No se pudo Inicializar SDL2, ERROR: %s\n", SDL_GetError());
    int imgFlag = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlag)) & imgFlag)
        printf("Couldnt initialize Image, error: %s\n", IMG_GetError());
    if (TTF_Init() < 0)
        printf("No se pudo inicializar TTF, ERROR: %s\n", TTF_GetError());
    screen = SDL_CreateWindow("Tetris Game",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              TAM * WIDTH,
                              TAM * HEIGHT,
                              SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void DrawCubes(SDL_Renderer *renderer, int x, int y, SDL_Texture *image) {
    SDL_Rect cubepos;
    SDL_QueryTexture(image, NULL, NULL, &cubepos.w, &cubepos.h);
    cubepos.x = x * TAM;
    cubepos.y = y * TAM;
    SDL_RenderCopy(renderer, image, NULL, &cubepos);
}

void DrawFigure(SDL_Renderer *renderer, Piezas *pieza) {
    for (int i = 0; i < sizeof(pieza->laterales) / sizeof(Coor) + 1; ++i) {
        Coor pos = PiezaPos(i, pieza);
        DrawCubes(renderer, pos.x, pos.y, pieza->images);
    }
}

void CrearPieza(Piezas *p, SDL_Texture *image) {
    int tetramino = rand() % (sizeof(Tetraminos) / (sizeof(Coor) * 3));
    p->central.x = 6;
    p->central.y = 1;
    for (int i = 0; i < (sizeof(Tetraminos) / (sizeof(Coor) * 3)); ++i)
        p->laterales[i] = Tetraminos[tetramino][i];
    p->images = image;
}

Coor PiezaPos(int n, Piezas *p) {
    Coor pos;
    switch (n) {
        case 0:
            pos.y = p->central.y;
            pos.x = p->central.x;
            break;
        default:
            pos.x = p->central.x + p->laterales[n - 1].x;
            pos.y = p->central.y + p->laterales[n - 1].y;
            break;
    }
    return pos;
}

SDL_Texture *LoadTexture(char *path, SDL_Renderer *renderer) {
    SDL_Texture *textura = NULL;
    SDL_Surface *surface = NULL;
    surface = IMG_Load(path);
    if (surface == NULL)
        printf("No se pudo cargar la imagen: %s\n", IMG_GetError());
    else {
        textura = SDL_CreateTextureFromSurface(renderer, surface);
        if (textura == NULL)
            printf("Error al generar textura: %s\n", SDL_GetError());
    }

    SDL_FreeSurface(surface);
    surface = NULL;
    return textura;
}

void DrawPlayGround(SDL_Renderer *renderer, Tablero *t) {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (t->pos[y][x] != NULL)
                DrawCubes(renderer, x, y, t->pos[y][x]);
        }
    }
}

void CambioCoord(Coor *coordenadas) {
    int aux = coordenadas->y;
    coordenadas->y = coordenadas->x;
    coordenadas->x = -aux;
}

void RotarPieza(Piezas *pieza) {
    for (int i = 0; i < sizeof(pieza->laterales) / sizeof(Coor); ++i) {
        CambioCoord(&pieza->laterales[i]);
    }
}

void LimpiarTablero(Tablero *t, SDL_Texture *images[]) {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (x == 0 && y != HEIGHT - 1)
                t->pos[y][x] = images[0];
            else if (y == HEIGHT - 1 && x == 0)
                t->pos[y][x] = images[2];
            else if (y == 16 && x != WIDTH - 1)
                t->pos[y][x] = images[3];
            else if (x == WIDTH - 1 && y == HEIGHT - 1)
                t->pos[y][x] = images[1];
            else if (x == WIDTH - 1 && y != HEIGHT - 1)
                t->pos[y][x] = images[0];
            else
                t->pos[y][x] = NULL;
        }
    }
}

void DetensionPieza(Tablero *t, Piezas *p) {
    for (int i = 0; i < sizeof(p->laterales) / sizeof(Coor) + 1; ++i) {
        Coor pos = PiezaPos(i, p);
        t->pos[pos.y][pos.x] = p->images;
    }
}

int HayColision(Tablero *t, Piezas *p) {
    for (int i = 0; i < sizeof(p->laterales) / sizeof(Coor) + 1; ++i) {
        Coor pos = PiezaPos(i, p);
        if (pos.x < 1 || pos.x >= WIDTH - 1)
            return 1;
        if (pos.y < 0 || pos.y >= HEIGHT - 1)
            return 1;
        if (t->pos[pos.y][pos.x] != NULL)
            return 1;
    }
    return 0;
}

int FilaCompleta(Tablero *t, int fila) {
    for (int x = 1; x < WIDTH - 1; ++x) {
        if (t->pos[fila][x] == NULL) return 0;
    }
    return 1;
}

void EliminarFila(Tablero *t, int fila) {
    for (int y = fila; y > 0; --y) {
        for (int x = 1; x < WIDTH - 1; ++x) {
            t->pos[y][x] = NULL;
            t->pos[y][x] = t->pos[y - 1][x];

        }
    }
    for (int x = 1; x < WIDTH - 1; ++x) {
        t->pos[0][x] = NULL;
    }
}

int LineasELiminadas(Tablero *t) {
    int actRow = HEIGHT - 2, lineasElim = 0;
    while (actRow > 0) {
        if (FilaCompleta(t, actRow)) {
            EliminarFila(t, actRow);
            lineasElim++;
        }
       else
           actRow--;

    }
    return lineasElim;
}

void Close(SDL_Renderer *renderer, SDL_Window *window, SDL_Texture *textura[]) {

    for (int i = 0; i < 11; ++i) {
        SDL_DestroyTexture(textura[i]);
        textura[i] = NULL;
    }
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}
