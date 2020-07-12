//
// Created by tomasb on 7/2/20.
//
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include <string.h>

#define TAM 40
#define WIDTH 12
#define HEIGHT 17
SDL_Window *screen = NULL;
SDL_Renderer *renderer = NULL;
SDL_Color blanco = {255, 255, 255, 255};
SDL_Color rojot = {255, 0, 0, 230};

typedef struct {
    SDL_Texture ***pos;
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

typedef struct {
    char nombre[30];
    long long puntaje;
} Records;


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

void DrawCubes(int x, int y, SDL_Texture *image);

void DrawPlayGround(Tablero *t);

void DrawFigure(Piezas *pieza);

void CrearPieza(Piezas *p, SDL_Texture *image);

Coor PiezaPos(int n, Piezas *p);

void CambioCoord(Coor *coordenadas);

void RotarPieza(Piezas *pieza);

void LimpiarTablero(Tablero *t, SDL_Texture *images[]);

void DetensionPieza(Tablero *t, Piezas *p);

int HayColision(Tablero *t, Piezas *p);

SDL_Texture *LoadTexture(char *path);

int FilaCompleta(Tablero *t, int fila);

void EliminarFila(Tablero *t, int fila);

int LineasELiminadas(Tablero *t);

SDL_Texture *ImprimirTexto(SDL_Texture *aux, SDL_Rect *rect, char *string, SDL_Color *color, int size);

SDL_Texture *ImprimirNumeros(SDL_Texture *aux, SDL_Rect *rect, long long *num, SDL_Color *color, char *string);

void
Actualizar(Tablero *t, Piezas *p, Piezas *next, SDL_Rect rects[], SDL_Texture *texturas[], char *sPunto, char *slineas,
           long long *puntos, long long *lineasEliminadas);

void LeerRecords(FILE *file, Records *top10);

void GameOver(SDL_Rect *rects, FILE *file, Records *top10, SDL_Event *event, SDL_Texture *texturas[], long long *puntos,
              char *sPunto);

void ActualizarRecords(FILE *file, Records *top10, Records *new);

void Close(SDL_Texture *textura[], char *spuntos, char *slineas);

int main() {
    FILE *records;
    srand(time(NULL));
    Init();
    Tablero t;
    t.pos = (SDL_Texture ***) malloc(HEIGHT * sizeof(SDL_Texture ***));
    for (int j = 0; j < HEIGHT; ++j) {
        t.pos[j] = (SDL_Texture **) malloc(WIDTH * sizeof(SDL_Texture **));
    }
    Records *top10 = (Records *) malloc(10 * sizeof(Records));
    for (int k = 0; k < 10 ; ++k) {
        for (int i = 0; i < 30 ; ++i) {
            top10[k].nombre[i] = '\0';
        }
        top10[k].puntaje = 0;
    }
    LeerRecords(records, top10);
    char *sPunto = (char *) malloc(10000000 * sizeof(char));
    long long puntos = 0;
    long long lineasEliminadas = 0;
    char *slineas = (char *) malloc(10000000 * sizeof(char));
    SDL_Rect rects[] = {{520, 0,   0, 0},    //0
                        {520, 200, 0, 0},    //1
                        {520, 400, 0, 0},    //2
                        {520, 240, 0, 0}, //3
                        {520, 440, 0, 0}, //4
                        {TAM * 9,  TAM, 0, 0}};    //5

    SDL_Texture *texturas[] = {LoadTexture("assets/VerticalWall.png"),    //0
                               LoadTexture("assets/RightCorner.png"), //1
                               LoadTexture("assets/LeftCorner.png"),  //2
                               LoadTexture("assets/HorizontaalWall.png"), //3
                               LoadTexture("assets/blueblock.png"),   //4 (4-10 cubeImages)
                               LoadTexture("assets/greenblcok.png"),  //5
                               LoadTexture("assets/orangeblock.png"), //6
                               LoadTexture("assets/pinkblock.png"),   //7
                               LoadTexture("assets/purpleblock.png"), //8
                               LoadTexture("assets/redblock.png"),    //9
                               LoadTexture("assets/yellowblock.png"), //10
                               ImprimirTexto(texturas[11], &rects[0], "Prixima Figura", &blanco, 28),    //11
                               ImprimirTexto(texturas[12], &rects[1], "Puntaje", &blanco, 28),    //12
                               ImprimirTexto(texturas[13], &rects[2], "Lineas Eliminadas", &blanco, 28),    //13
                               NULL,    //14
                               NULL,    //15
                               ImprimirTexto(texturas[16], &rects[5], "Game Over", &rojot, 62)};   //16


    rects[5].x -= rects[5].w / 2;
    Piezas p, next;
    CrearPieza(&p, texturas[4 + (rand() % 7)]);
    CrearPieza(&next, texturas[4 + (rand() % 7)]);
    p.central.x = 6;
    p.central.y = 1;
    LimpiarTablero(&t, texturas);
    SDL_Event event;
    int play = 1, tick = 0, down = 0, auxl;

    Actualizar(&t, &p, &next, rects, texturas, sPunto, slineas, &puntos, &lineasEliminadas);
    while (play) {
        Piezas aux = p;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT)
                play = 0;
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        RotarPieza(&p);
                        down = 0;
                        break;
                    case SDLK_DOWN:
                        p.central.y++;
                        down = 1;
                        break;
                    case SDLK_LEFT:
                        p.central.x--;
                        down = 0;
                        break;
                    case SDLK_RIGHT:
                        p.central.x++;
                        down = 0;
                        break;
                }
            }
        }
        if (tick % 20 == 0) {
            p.central.y++;
            tick = 0;
            down = 1;
        }

        if (HayColision(&t, &p)) {
            p = aux;
            for (int i = 0; i < 4; ++i) {
                if (PiezaPos(i, &p).y == 0 && HayColision(&t,&p) == 2) {
                    GameOver(rects, records, top10, &event, texturas, &puntos, sPunto);
                    play = 0;
                    break;
                }

            }
            if (down) {
                DetensionPieza(&t, &p);
                auxl = LineasELiminadas(&t);
                puntos += auxl == 1 ? auxl * 25 : auxl * auxl * 100;
                lineasEliminadas += auxl;
                p = next;
                CrearPieza(&next, texturas[4 + (rand() % 7)]);
                p.central.x = 6;
                p.central.y = 1;
                down = 0;
            }
        }
        if (play) {
            Actualizar(&t, &p, &next, rects, texturas, sPunto, slineas, &puntos, &lineasEliminadas);
            SDL_Delay(20);
            tick++;
        }

    }
    for (int j = 0; j < HEIGHT; ++j) {
        free(t.pos[j]);
    }
    free(t.pos);
    free(top10);
    Close(texturas, sPunto, slineas);
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
                              TAM * WIDTH + TAM * 6,
                              TAM * HEIGHT,
                              SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void DrawCubes(int x, int y, SDL_Texture *image) {
    SDL_Rect cubepos;
    SDL_QueryTexture(image, NULL, NULL, &cubepos.w, &cubepos.h);
    cubepos.x = x * TAM;
    cubepos.y = y * TAM;
    SDL_RenderCopy(renderer, image, NULL, &cubepos);
}

void DrawFigure(Piezas *pieza) {
    for (int i = 0; i < sizeof(pieza->laterales) / sizeof(Coor) + 1; ++i) {
        Coor pos = PiezaPos(i, pieza);
        DrawCubes(pos.x, pos.y, pieza->images);
    }
}

void CrearPieza(Piezas *p, SDL_Texture *image) {
    int tetramino = rand() % (sizeof(Tetraminos) / (sizeof(Coor) * 3));
    p->central.x = 14;
    p->central.y = 2;
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

SDL_Texture *LoadTexture(char *path) {
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

void DrawPlayGround(Tablero *t) {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (t->pos[y][x] != NULL)
                DrawCubes(x, y, t->pos[y][x]);
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
        if (t->pos[pos.y][pos.x] != NULL)
            return 2;
        if (pos.y < 0 || pos.y > HEIGHT - 2)
            return 1;
        if (pos.x < 1 || pos.x >= WIDTH - 1)
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
        } else
            actRow--;

    }
    return lineasElim;
}

SDL_Texture *ImprimirTexto(SDL_Texture *aux, SDL_Rect *rect, char *string, SDL_Color *color, int size) {
    TTF_Font *font = TTF_OpenFont("assets/StrickenBrush.ttf", size);
    SDL_Surface *textsurface = TTF_RenderText_Solid(font, string, *color);
    aux = SDL_CreateTextureFromSurface(renderer, textsurface);
    SDL_FreeSurface(textsurface);
    textsurface = NULL;
    SDL_QueryTexture(aux, NULL, NULL, &rect->w, &rect->h);
    return aux;
}

SDL_Texture *ImprimirNumeros(SDL_Texture *aux, SDL_Rect *rect, long long *num, SDL_Color *color, char *string) {
    sprintf(string, "%lld", *num);
    TTF_Font *font = TTF_OpenFont("assets/Montserrat-Regular.ttf", 28);
    SDL_Surface *textsurface = TTF_RenderText_Solid(font, string, *color);
    aux = SDL_CreateTextureFromSurface(renderer, textsurface);
    SDL_FreeSurface(textsurface);
    textsurface = NULL;
    SDL_QueryTexture(aux, NULL, NULL, &rect->w, &rect->h);
    return aux;
}

void
Actualizar(Tablero *t, Piezas *p, Piezas *next, SDL_Rect rects[], SDL_Texture *texturas[], char *sPunto, char *slineas,
           long long *puntos, long long *lineasEliminadas) {
    texturas[14] = ImprimirNumeros(texturas[14], &rects[3], puntos, &blanco, sPunto);
    texturas[15] = ImprimirNumeros(texturas[15], &rects[4], lineasEliminadas, &blanco, slineas);
    SDL_RenderClear(renderer);
    DrawPlayGround(t); // ocupa x = 0 , x = WIDTH -1  y = HEIGHT -1
    DrawFigure(p);
    DrawFigure(next);
    for (int i = 0; i < 5; ++i) {
        SDL_RenderCopy(renderer, texturas[i + 11], NULL, &rects[i]);
    }
    SDL_RenderPresent(renderer);
    SDL_UpdateWindowSurface(screen);
}

void LeerRecords(FILE *file, Records *top10) {
    file = fopen("assets/record.txt", "r");
    if (file == NULL)
        printf("no se puedo abrir los records");
    int recordAct = 0;
    while (!feof(file)) {
        fscanf(file, "%s %lld", top10[recordAct].nombre, &top10[recordAct].puntaje);
        recordAct++;
    }
    fclose(file);
}

void GameOver(SDL_Rect *rects, FILE *file, Records *top10, SDL_Event *event, SDL_Texture *texturas[], long long *puntos,
              char *sPunto) {
    SDL_StartTextInput();
    Records new;
    for (int i = 0; i < 30; ++i) {
        new.nombre[i] = '\0';
    }
    texturas[13] = NULL;
    file = fopen("assets/record.txt", "w");
    texturas[14] = ImprimirNumeros(texturas[14], &rects[3], puntos, &blanco, sPunto);
    rects[3].x = 12 * TAM;
    rects[3].y = 6 * TAM - rects[3].h / 2;
    texturas[12] = ImprimirTexto(texturas[12], &rects[1], "Puntaje Obtenido", &blanco, 30);
    rects[1].x = 2 * TAM;
    rects[1].y = 6 * TAM - rects[1].h / 2;
    texturas[11] = ImprimirTexto(texturas[11], &rects[0], "Ingrese Numbre:", &blanco, 26);
    rects[0].x = 2 * TAM;
    rects[0].y = 10 * TAM - rects[0].h / 2;
    new.puntaje = *puntos;
    int inputName = 1;
    while (inputName) {
        while (SDL_PollEvent(event) != 0) {
            if (event->type == SDL_QUIT) {
                inputName = 0;
                break;
            } else if (event->type == SDL_TEXTINPUT) {
                strcat(new.nombre, event->text.text);
                texturas[13] = ImprimirTexto(texturas[13], &rects[2], new.nombre, &blanco, 26);
                rects[2].x = 12 * TAM;
                rects[2].y = 10 * TAM - rects[2].h / 2;
            } else if (event->type == SDL_KEYDOWN)
                switch (event->key.keysym.sym) {
                    case SDLK_RSHIFT:
                        inputName = 0;
                        break;
                }
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texturas[16], NULL, &rects[5]);
        SDL_RenderCopy(renderer, texturas[12], NULL, &rects[1]);
        SDL_RenderCopy(renderer, texturas[14], NULL, &rects[3]);
        SDL_RenderCopy(renderer, texturas[11], NULL, &rects[0]);
        SDL_RenderCopy(renderer, texturas[13], NULL, &rects[2]);
        SDL_RenderPresent(renderer);
        SDL_UpdateWindowSurface(screen);
    }
    ActualizarRecords(file, top10, &new);
    SDL_StopTextInput();
    SDL_StopTextInput();
    fclose(file);
}

void ActualizarRecords(FILE *file, Records *top10, Records *new) {

    if (new->puntaje > top10[0].puntaje) {
        for (int k = 1; k < 10; ++k) {
            top10[k] = top10[k - 1];
        }
        top10[0] = *new;
    } else if (top10[9].puntaje < new->puntaje) {
        top10[9] = *new;
        for (int i = 0; i < 10; i++) {
            for (int j = i + 1; j < 10; j++) {
                if (top10[i].puntaje < top10[j].puntaje) {
                    Records aux = top10[i];
                    top10[i] = top10[j];
                    top10[j] = aux;
                }

            }

        }
    }


    for (int i = 0; i < 10; i++)
        fprintf(file, "%s %lld\n", top10[i].nombre, top10[i].puntaje);
}

void Close(SDL_Texture *textura[], char *spuntos, char *slineas) {

    for (int i = 0; i < 17; ++i) {
        SDL_DestroyTexture(textura[i]);
        textura[i] = NULL;
    }
    free(spuntos);
    free(slineas);
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(screen);
    screen = NULL;
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}
