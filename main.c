#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Texture *LoadTexture(char *path, SDL_Renderer *renderer);

void Cubes(SDL_Renderer *renderer, SDL_Texture *image, int x, int y);

void Draw(SDL_Renderer *renderer, SDL_Texture *image);


void Close(SDL_Window *screen, SDL_Renderer *renderer, SDL_Texture *image);

int main() {


    SDL_Window *screen = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *image = NULL;
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
    screen = SDL_CreateWindow("Tetris Game",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              640,
                              640,
                              SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    image = LoadTexture("./assets/greenblcok.png", renderer);
    SDL_Event event;
    while (succes) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT)
                succes = 0;
        }

        SDL_RenderClear(renderer);
        for (int y = 0; y <= 640; y += 40) {
            for (int x = 0; x <= 360; x += 40) {
                if (x == 0)
                    Cubes(renderer, image, x, y);
                if (y == 600)
                    Cubes(renderer, image, x, y);
                if (x == 360)
                    Cubes(renderer, image, x, y);
            }

        }
        //Draw(renderer, image);
        SDL_RenderPresent(renderer);
        SDL_UpdateWindowSurface(screen);

    }
    Close(screen, renderer, image);
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
    int tw, th;
    SDL_Rect basecube;
    SDL_Rect cubepos;
    SDL_QueryTexture(image, NULL, NULL, &tw, &th);
    basecube.x = basecube.y = 0;
    basecube.w = tw;
    basecube.h = th;
    cubepos.x = x;
    cubepos.y = y;
    cubepos.w = cubepos.h = 40;
    SDL_RenderCopy(renderer, image, &basecube, &cubepos);


}

//Funcion que dibuja nuestro escenario de juego;
void Draw(SDL_Renderer *renderer, SDL_Texture *image) {
    for (int y = 0; y <= 640; y += 40) {
        for (int x = 0; x <= 360; x += 40) {
            if (x == 0)
                Cubes(renderer, image, x, y);
            if (y == 600)
                Cubes(renderer, image, x, y);
            if (x == 360)
                Cubes(renderer, image, x, y);
        }

    }
}

void Close(SDL_Window *screen, SDL_Renderer *renderer, SDL_Texture *image) {
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyTexture(image);
    image = NULL;
    SDL_DestroyWindow(screen);
    screen = NULL;
    IMG_Quit();
    SDL_Quit();
}

