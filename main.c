#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
    int x;
    int y;
    SDL_Texture *image;
} Cubos;

typedef struct {
    int x;
    int y;
    Cubos forma[4];
    SDL_Rect rect;
}Lfgure;

SDL_Texture *LoadTexture(char *path, SDL_Renderer *renderer);

void CubeMovement(Cubos *cubo, char direccion);

void Cubes(SDL_Renderer *renderer, SDL_Texture *image, int x, int y);

void Close(SDL_Window *screen, SDL_Renderer *renderer, SDL_Texture *texturas[], SDL_Texture *images[]);

int main() {
    SDL_Window *screen = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texturas[] = {NULL,NULL,NULL};
    int succes = 1;
    int sdl2 = SDL_Init(SDL_INIT_VIDEO);
    if (sdl2 < 0) {
        printf("Coulnt Init SDL2: Error code: %s", SDL_GetError());
        succes = 0;
    }
    int imgFlag = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlag) & imgFlag)) {
        printf("Couldnt initialize Image, error: %s\n", IMG_GetError());
        succes = 0;
    }
    if (TTF_Init() < 0)
        printf("Could not init TTF, Error: %s", TTF_GetError());
    screen = SDL_CreateWindow("Tetris Game",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              720,
                              680,
                              SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Texture *images[] = {LoadTexture("./assets/greenblcok.png", renderer),  //0
                             LoadTexture("./assets/blueblock.png", renderer),   //1
                             LoadTexture("./assets/orangeblock.png", renderer), //2
                             LoadTexture("./assets/pinkblock.png", renderer),   //3
                             LoadTexture("./assets/purpleblock.png", renderer), //4
                             LoadTexture("./assets/redblock.png", renderer),    //5
                             LoadTexture("./assets/yellowblock.png", renderer)};//6
    TTF_Font *font = TTF_OpenFont("assets/StrickenBrush.ttf", 24);
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface *textsurface = TTF_RenderText_Solid(font, "Next Cube", color);
    texturas[0] = SDL_CreateTextureFromSurface(renderer, textsurface);
    SDL_Rect textRect;
    textRect.x = 520;
    textRect.y = 0;
    SDL_QueryTexture(texturas[0], NULL, NULL, &textRect.w, &textRect.h);
    SDL_FreeSurface(textsurface);
    textsurface = NULL;
    textsurface = TTF_RenderText_Solid(font,"Puntaje", color);
    texturas[1] = SDL_CreateTextureFromSurface(renderer,textsurface);
    SDL_Rect puntosRect;
    puntosRect.x = 520;
    puntosRect.y = 160;
    SDL_QueryTexture(texturas[1], NULL, NULL, &puntosRect.w, &puntosRect.h);
    SDL_FreeSurface(textsurface);
    textsurface = NULL;
    textsurface = TTF_RenderText_Solid(font,"Lineas Eliminadas", color);
    texturas[2] = SDL_CreateTextureFromSurface(renderer,textsurface);
    SDL_Rect filasRect;
    filasRect.x = 520;
    filasRect.y = 320;
    SDL_QueryTexture(texturas[2], NULL, NULL, &filasRect.w, &filasRect.h);
    SDL_FreeSurface(textsurface);
    textsurface = NULL;
    Cubos cubes[7];
    for (int i = 0; i < 7; ++i) {
        cubes[i].x = 240;
        cubes[i].y = 0;
        cubes[i].image = images[i];
    }
    Lfgure primera;
    primera.x = 280;
    primera.y = 0;
    primera.forma[0].x = primera.x - 40;
    primera.forma[0].y = primera.y;
    primera.forma[1].x =  primera.forma[0].x;
    primera.forma[1].y = primera.forma[0].y + 40;
    primera.forma[2].x =  primera.forma[0].x;
    primera.forma[2].y = primera.forma[1].y + 40;
    primera.forma[3].x =  primera.x;
    primera.forma[3].y = primera.forma[2].y;
    primera.forma[0].image =  primera.forma[1].image =  primera.forma[2].image =  primera.forma[3].image = images[4];
    SDL_Event event;
    int cuboActual = 0;
    while (succes) {

        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT)
                succes = 0;
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        for(int i = 0; i < 4; ++i) CubeMovement(&primera.forma[i], 'u');
                        primera.y -= 40;
//                        CubeMovement(&cubes[cuboActual], 'u');
                        break;
                    case SDLK_DOWN:
                        for(int i = 0; i < 4; ++i) CubeMovement(&primera.forma[i], 'd');
                        primera.y += 40;
//                        CubeMovement(&cubes[cuboActual], 'd');
                        break;
                    case SDLK_LEFT:
                        for(int i = 0; i < 4; ++i) CubeMovement(&primera.forma[i], 'l');
                        primera.x -= 40;
//                        CubeMovement(&cubes[cuboActual], 'l');
                        break;
                    case SDLK_RIGHT:
                        for(int i = 0; i < 4; ++i) CubeMovement(&primera.forma[i], 'r');
                        primera.x += 40;
//                        CubeMovement(&cubes[cuboActual], 'r');
                        break;
                }
            }
        }

        SDL_RenderClear(renderer);
        // dibujar area de juego
        for (int y = 0; y <= 640; y += 40) {
            for (int x = 0; x <= 440; x += 40) {
                if (x == 0 || x == 440 || y == 640)
                    Cubes(renderer, images[3], x, y);
            }
        }
//        for (int i = 0; i <= cuboActual; ++i) {
//            Cubes(renderer, cubes[i].image, cubes[i].x, cubes[i].y);
//        }
//        if (cubes[cuboActual].y == 600) {
//            cuboActual++;
//            if (cuboActual > 7)
//                cuboActual = 0;
//        }
        for (int i = 0; i < 4; ++i) {
            Cubes(renderer,primera.forma[i].image,primera.forma[i].x, primera.forma[i].y);
        }

        //Draw(renderer, image);
        SDL_RenderCopy(renderer, texturas[0], NULL, &textRect);
        SDL_RenderCopy(renderer, texturas[1], NULL, &puntosRect);
        SDL_RenderCopy(renderer, texturas[2], NULL, &filasRect);
        SDL_RenderPresent(renderer);
        SDL_UpdateWindowSurface(screen);

    }
    Close(screen, renderer, texturas,images);
    return 0;
}

SDL_Texture *LoadTexture(char *path, SDL_Renderer *renderer) {
    SDL_Texture *texture = NULL;
    SDL_Surface *surface = NULL;
    surface = IMG_Load(path);
    if (surface == NULL)
        printf("No se pudo cargar la imagen: %s\n", IMG_GetError());
    else {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture == NULL)
            printf("Error al generar textura: %s\n", SDL_GetError());
    }
    SDL_FreeSurface(surface);
    surface = NULL;

    return texture;
}

void Cubes(SDL_Renderer *renderer, SDL_Texture *image, int x, int y) {
    SDL_Rect cubepos;
    SDL_QueryTexture(image, NULL, NULL, &cubepos.w, &cubepos.h);
    cubepos.x = x;
    cubepos.y = y;

    SDL_RenderCopy(renderer, image, NULL, &cubepos);
}

void CubeMovement(Cubos *cubo, char direccion) {
    switch (direccion) {
        case 'u':
            if (cubo->y > 0)
                cubo->y -= 40;
            break;
        case 'd':
            if (cubo->y + 120 < 600)
                cubo->y += 40;
            break;
        case 'l':
            if (cubo->x > 40)
                cubo->x -= 40;
            break;
        case 'r':
            if (cubo->x + 40 < 400)
                cubo->x += 40;
            break;
    }
}

void Close(SDL_Window *screen, SDL_Renderer *renderer, SDL_Texture *texturas[], SDL_Texture *images[]) {
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    for (int i = 0; i < sizeof(texturas) / sizeof(SDL_Texture*); ++i) {
        SDL_DestroyTexture(texturas[i]);
        texturas[i] = NULL;
    }
    for (int i = 0; i < sizeof(images) / sizeof(SDL_Texture*); ++i) {
        SDL_DestroyTexture(images[i]);
        images[i] = NULL;
    }
    SDL_DestroyWindow(screen);
    screen = NULL;
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

