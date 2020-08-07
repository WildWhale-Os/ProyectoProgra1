#include <stdio.h>
#include <SDL2/SDL.h>
#include "Functions.h"

int main() {
    Init();
    FILE * file = NULL;
    SDL_Texture *texturas[10];
    for (int i = 0; i < 10; ++i) {
        texturas[i] = NULL;
    }
    SDL_Color color[2] ={ { 47, 227, 197, 255 },
        { 233, 148, 58, 255 } };
    int tetris = 1, opcion = 1;
    while (tetris) {
        switch (opcion) {
        case 1:
            opcion = Menu(texturas, color);
            CleanTextures(texturas);
            break;
        case 0:
            opcion = play(texturas);
            CleanTextures(texturas);
            break;
        case 2:
            opcion = Puntajes(texturas, color);
            CleanTextures(texturas);
            break;
        case 3:
            opcion = controls(texturas, color);
            CleanTextures(texturas);
            break;
        default:
            tetris = 0;
            break;
        }
    }
    Close(texturas);
    return 0;
}