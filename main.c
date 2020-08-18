#include "Functions.h"

int main(int argc, char *argv[]) {
    Init();
    Tetris game;
    InitRecords(game.top10);
    LeerRecords(game.DB, game.top10);
    InitFondos(&game);
    int tetris = 1, opcion = 1;
    while (tetris) {
        switch (opcion) {
        case 0:
            opcion = play(&game);
            break;

        case 1:
            opcion = Menu(&game);
            break;
        
        case 2:
            opcion = GameOver(&game);
            break;

        default:
            tetris = 0;
            break;
        }
    }

    CleanTextures(game.fondos, 6);
    Close();
    return 0;
}