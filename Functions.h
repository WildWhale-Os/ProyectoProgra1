#include "static.h"
#include <time.h>
#include <string.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

void Init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        printf("No se pudo Inicializar SDL2, ERROR: %s\n", SDL_GetError());
    int imgFlag = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlag)) & imgFlag)
        printf("Couldnt initialize Image, error: %s\n", IMG_GetError());
    if (TTF_Init() < 0)
        printf("No se pudo inicializar TTF, ERROR: %s\n", TTF_GetError());
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) < 0)
        //coment iniciamos el audio con parametros por defecto de frecuencia, formato de salida, cantidad de canales (1 para mono 2 para estereo), y 4096 de chunksize porque es lo estandar
        printf("No se pudo inicializar sonido, ERROR: %s\n", Mix_GetError());

    screen = SDL_CreateWindow("Tetris Game",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              TAM * WIDTH + TAM * 6,
                              TAM * HEIGHT,
                              SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    sfx = Mix_LoadWAV("assets/SEffects/Line.wav"); //cargamos las pistas;
    bot = Mix_LoadWAV("assets/SEffects/boton.wav");
    bot2 = Mix_LoadWAV("assets/SEffects/boton2.wav");
    bgm = Mix_LoadMUS("assets/Music/vitality.mp3");
    pausa = Mix_LoadWAV("assets/Music/bgm.wav");
    GO = Mix_LoadMUS("assets/Music/det.mp3");
    Mix_AllocateChannels(4); //definimos 3 canales;
    Mix_VolumeChunk(pausa, VolM);
    Mix_PlayChannel(3, pausa, -1);
}

void CleanTextures(SDL_Texture **texturas, int num)
{
    for (int i = 0; i < num; ++i)
    {
        SDL_DestroyTexture(texturas[i]);
        texturas[i] = NULL;
    }
}

SDL_Texture *LoadTexture(char *path)
{
    SDL_Texture *textura = NULL;
    SDL_Surface *surface = NULL;
    surface = IMG_Load(path);
    if (surface == NULL)
        printf("No se pudo cargar la imagen: %s\n", IMG_GetError());
    else
    {
        textura = SDL_CreateTextureFromSurface(renderer, surface);
        if (textura == NULL)
            printf("Error al generar textura: %s\n", SDL_GetError());
    }

    SDL_FreeSurface(surface);
    surface = NULL;
    return textura;
}

void CrearArregloPath(char **paths)
{
    paths[0] = "assets/Gradient/vg1.png";
    paths[1] = "assets/Gradient/lcg1.png";
    paths[2] = "assets/Gradient/hg1.png";
    paths[3] = "assets/Gradient/rcg1.png";
    paths[4] = "assets/Gradient/vg2.png";
    paths[5] = "assets/Gradient/lcg2.png";
    paths[6] = "assets/Gradient/hg2.png";
    paths[7] = "assets/Gradient/rcg2.png";
    paths[8] = "assets/Gradient/vg3.png";
    paths[9] = "assets/Gradient/lcg3.png";
    paths[10] = "assets/Gradient/hg3.png";
    paths[11] = "assets/Gradient/rcg3.png";
    paths[12] = "assets/Gradient/vg4.png";
    paths[13] = "assets/Gradient/lcg4.png";
    paths[14] = "assets/Gradient/hg4.png";
    paths[15] = "assets/Gradient/rcg4.png";
    paths[16] = "assets/Gradient/vg5.png";
    paths[17] = "assets/Gradient/lcg5.png";
    paths[18] = "assets/Gradient/hg5.png";
    paths[19] = "assets/Gradient/rcg5.png";
    paths[20] = "assets/Gradient/vg6.png";
    paths[21] = "assets/Gradient/lcg6.png";
    paths[22] = "assets/Gradient/hg6.png";
    paths[23] = "assets/Gradient/rcg6.png";
    paths[24] = "assets/Gradient/vg7.png";
    paths[25] = "assets/Gradient/lcg7.png";
    paths[26] = "assets/Gradient/hg7.png";
    paths[27] = "assets/Gradient/rcg7.png";
}

void CrearTableroEnMemoria(SDL_Texture *pos[][WIDTH])
{
    for (int j = 0; j < HEIGHT; ++j)
    {
        for (int i = 0; i < WIDTH; ++i)
        {
            pos[j][i] = NULL;
        }
    }
}

void CrearPieza(Piezas *p, SDL_Texture *image)
{
    int tetramino = rand() % 7;
    p->central.x = 15 - 0.5;
    p->central.y = 2;
    for (int i = 0; i < 3; ++i)
        p->laterales[i] = Tetraminos[tetramino][i];
    p->images = image;
}

SDL_Texture *createButton(SDL_Texture *menus, char *button, SDL_Rect *pos)
{
    SDL_Surface *textSurface = IMG_Load(button);                 //crea un puntero a superficie
    menus = SDL_CreateTextureFromSurface(renderer, textSurface); //crea una textura desde una superficie existente
    SDL_QueryTexture(menus, NULL, NULL, &pos->w, &pos->h);       //consulta los atributos de la textura pos
    SDL_FreeSurface(textSurface);                                //se libera la superficie
    textSurface = NULL;                                          //el puntero de superficie se redefine a nulo
    return menus;                                                //se devuelve el puntero menu con la textura creada
}

SDL_Texture *ImprimirTexto(SDL_Texture *aux, SDL_Rect *rect, char *string, SDL_Color *color, int size)
{
    TTF_Font *font = TTF_OpenFont("assets/fonts/StrickenBrush.ttf", size);
    SDL_Surface *textsurface = TTF_RenderText_Solid(font, string, *color);
    aux = SDL_CreateTextureFromSurface(renderer, textsurface);
    SDL_FreeSurface(textsurface);
    textsurface = NULL;
    SDL_QueryTexture(aux, NULL, NULL, &rect->w, &rect->h);
    TTF_CloseFont(font);
    return aux;
}

SDL_Texture *ImprimirNumeros(SDL_Texture *aux, SDL_Rect *rect, long long *num, SDL_Color *color, char *string, int size)
{
    sprintf(string, "%lld", *num);
    TTF_Font *font = TTF_OpenFont("assets/fonts/Montserrat-Regular.ttf", size);
    SDL_Surface *textsurface = TTF_RenderText_Solid(font, string, *color);
    aux = SDL_CreateTextureFromSurface(renderer, textsurface);
    SDL_FreeSurface(textsurface);
    textsurface = NULL;
    SDL_QueryTexture(aux, NULL, NULL, &rect->w, &rect->h);
    TTF_CloseFont(font);
    return aux;
}

void DefinirTexturasGame(Tetris *game, SDL_Texture **texturas, char **paths, SDL_Rect *rects)
{
    texturas[0] = LoadTexture(paths[0]);
    texturas[1] = LoadTexture(paths[3]);
    texturas[2] = LoadTexture(paths[1]);
    texturas[3] = LoadTexture(paths[2]);
    texturas[4] = LoadTexture(colors[rand() % 8]);
    CrearPieza(&game->actFigure, texturas[4]);
    texturas[4] = LoadTexture(colors[rand() % 8]);
    CrearPieza(&game->nextFigure, texturas[4]);
    game->actFigure.central.x = 6;
    game->actFigure.central.y = 1;
    texturas[5] = LoadTexture("assets/backrounds/info.png");
    texturas[8] = NULL;
    texturas[9] = NULL;
}

void DefinirTexturasMenu(SDL_Texture **texturas, char **paths, SDL_Rect *pos, char **buttons, int *frame)
{ //funcion que reestructura las texturas del main para formar los botones del main menu
    for (int i = 0; i < menuNum; i++)
    boton:
    {
        texturas[i] = createButton(texturas[i], buttons[i], &pos[i]); //se crean los botones
        pos[i].x = TAM * 9 - (pos[i].w / 2);                          //se definen las posiciones iniciales de las texturas
        pos[i].y = 250 + 50 * i;
    }
        texturas[4] = LoadTexture(paths[*frame]);                    //carga las texturas en el array texturas
    SDL_QueryTexture(texturas[4], NULL, NULL, &pos[4].w, &pos[4].h); //consulta los atributos de la textura pos
    pos[4].x = (TAM * 9) - (pos[4].w / 2);                           //se definen las posiciones iniciales de la textura del titulo
    pos[4].y = TAM;
}

void DefinirTexturasControl(SDL_Texture *button, SDL_Rect *pos, char **buttons)
{ //funcion que reestructura las texturas del main para formar los botones del menu controls
    button = createButton(button, buttons[0], &pos[0]);
    pos[0].x = 40; //cambia la posicion en x
    pos[0].y = 40; //cambia la posicion en y
}

void DefinirTexturasRecords(Tetris *game, SDL_Texture **texturas, SDL_Rect *pos, char **buttons)
{                                                                 //funcion que reestructura las texturas del main para formar los botones del menu records
    texturas[0] = createButton(texturas[0], buttons[0], &pos[0]); //boton: back
    pos[0].x = 40;                                                //cambia la posicion en x
    pos[0].y = 40;                                                //cambia la posicion en y
    int initPos = 210;
    for (int i = 1; i < 21; i += 2)
    {
        char numero[1000000] = {'\0'};
        texturas[i] = ImprimirTexto(texturas[i], &pos[i], game->top10[(i - 1) / 2].nombre, &blanco, 15);
        pos[i].x = 94 + 293 / 2 - pos[i].w / 2;
        pos[i].y = initPos + (41 * (i - 1) / 2) + 20 + pos[i].h / 2;
        texturas[i + 1] = ImprimirNumeros(texturas[i], &pos[i + 1], &game->top10[(i - 1) / 2].puntaje, &blanco, numero, 15);
        pos[i + 1].x = 387 + 293 / 2 - pos[i + 1].w / 2;
        pos[i + 1].y = initPos + (41 * (i - 1) / 2) + 20 + pos[i + 1].h / 2;
    }
}

void DefinirTexturasPause(SDL_Texture **texturas, SDL_Rect *pos, char **buttons, SDL_Color *color)
{                                     //funcion que reestructura las texturas del main para formar los botones del menu pause
    for (int i = 0; i < menuNum; i++) //botones: continue, records, controls, main menu
    {
        texturas[i] = createButton(texturas[i], buttons[i], &pos[i]);
        pos[i].x = TAM * 9 - pos[i].w / 2; //cambia la posicion en x
        pos[i].y = 250 + 50 * i;           //cambia la posicion en y
    }
}

void InitRecords(Records *top10)
{
    for (int k = 0; k < 10; ++k)
    {
        for (int i = 0; i < 30; ++i)
        {
            top10[k].nombre[i] = '\0';
        }
        top10[k].puntaje = 0;
    }
}

void InitPuntiacion(TableroPuntaje *tp)
{
    tp->sPuntos = (char *)malloc(10000000 * sizeof(char));
    tp->puntos = 0;
    tp->lineasEliminadas = 0;
    tp->slineas = (char *)malloc(10000000 * sizeof(char));
}

void DrawCubes(int x, int y, SDL_Texture *image)
{
    SDL_Rect cubepos;
    SDL_QueryTexture(image, NULL, NULL, &cubepos.w, &cubepos.h);
    cubepos.x = x * TAM;
    cubepos.y = y * TAM;
    SDL_RenderCopy(renderer, image, NULL, &cubepos);
}

Coor PiezaPos(int n, Piezas *p)
{
    Coor pos;
    switch (n)
    {
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

void DrawFigure(Piezas *pieza)
{
    for (int i = 0; i < sizeof(pieza->laterales) / sizeof(Coor) + 1; ++i)
    {
        Coor pos = PiezaPos(i, pieza);
        DrawCubes(pos.x, pos.y, pieza->images);
    }
}

int Puntajes(Tetris *game)
{
    SDL_Texture *images[21];
    int x, y;                                          //variables de coordenadas del mouse
    char *button[] = {"assets/buttons/button15.png",   //arreglo de string a path de los botones en uso
                      "assets/buttons/button16.png"};  //2 = cantidad de botones en el menu
    SDL_Rect pos[21];                                  //uso de pos para definir la posicion de los rect mas adelante
    DefinirTexturasRecords(game, images, pos, button); //reestructuracion de texturas del main para su uso como boton  en un estado inicial especifico
    int cont = 0;                                      //contador de ticks para el cambio del backround
    SDL_Event event;                                   //definicion del evento
    int inPuntaje = 1, retorno = 0;                    //comodines de permanencia en una funcion: en el menu puntaje y uso en el main respectivamente
    while (inPuntaje)                                  //Loop que verifica que estemos en el menu puntaje
    {
        while (SDL_PollEvent(&event)) //mientas el evento "event" este activo
        {
            switch (event.type)
            {
            case SDL_QUIT: //en caso de apretar la x, se devuelve 5 al main (lo que hace que se cierre el juego)
                retorno = 5;
                inPuntaje = 0; //salir del menu puntaje
                break;
            case SDL_MOUSEMOTION: //en caso de que el mouse se mueva, se detecta la posicion y se establece como x e y.
                SDL_GetMouseState(&x, &y);

                if (x >= pos[0].x && x <= pos[0].x + pos[0].w && y >= pos[0].y && y <= pos[0].y + pos[0].h) //si la posicion del mouse coincide con la de una textura(boton)
                {
                    images[0] = createButton(images[0], button[1], &pos[0]); //cambia el estado inicial de la textura a un boton presionado
                    continue;
                }
                else
                {
                    images[0] = createButton(images[0], button[0], &pos[0]); //cambia de vuelta el estado inicial de la textura a un boton sin presionar
                    continue;
                }

                break;
            case SDL_MOUSEBUTTONDOWN: //si se hace click izquierdo con el mouse

                //x = event.button.x;
                // y = event.button.y;

                if (x >= pos[0].x && x <= pos[0].x + pos[0].w && y >= pos[0].y && y <= pos[0].y + pos[0].h) //si la posicion del mouse coincide con la de una textura(boton)
                {
                    Mix_VolumeChunk(bot2, VolM); // agregamos sonido a los botones mientras se esta en la configuracion
                    Mix_PlayChannel(2, bot2, 0);
                    retorno = 1; //se devuelve 1 al main lo que significa volver al menu principal
                    inPuntaje = 0;
                }

                break;
            case SDL_KEYDOWN:                            //si se pareta alguna tecla del teclado
                if (event.key.keysym.sym == SDLK_ESCAPE) //si esa tecla es esc
                {
                    retorno = 1; //se devuelve 1 al main lo que significa volver al menu principal
                    inPuntaje = 0;
                }
                break;
            }
        }
        if (cont == 30)
            cont = 0;
        else
            cont += 1;
        SDL_RenderClear(renderer);                             //se limpia el renderizador
        SDL_RenderCopy(renderer, game->fondos[5], NULL, NULL); //se presentan las texturas del fondo al renderizador
        for (int i = 0; i < 21; i++)
            SDL_RenderCopy(renderer, images[i], NULL, &pos[i]); //se presentan las texturas de los botones al renderizador

        SDL_RenderPresent(renderer);     //se presenta el renderizador
        SDL_UpdateWindowSurface(screen); //se recarga la superficie
    }
    CleanTextures(images, 20);
    return retorno; //se devuelve el valor dependiendo de lo seleccionado en el event loop
}

int controls()
{
    SDL_Texture *fondo = NULL;
    SDL_Texture *button = NULL;
    int x, y;                                    //coordenadas del mouse
    char *paths[] = {"assets/backrounds/c1.png", //arreglo de string a path de las imagenes de fondo en uso
                     "assets/backrounds/c2.png",
                     "assets/backrounds/c3.png",
                     "assets/backrounds/c4.png",
                     "assets/backrounds/c5.png",
                     "assets/backrounds/c6.png"};

    char *pathButtons[] = {"assets/buttons/button15.png", //arreglo de string a path de los botones en uso
                           "assets/buttons/button16.png"};
    SDL_Rect pos[1];                                  //uso de pos para definir la posicion de los rect mas adelante
    DefinirTexturasControl(button, pos, pathButtons); //reestructuracion de texturas del main para su uso como boton en un estado inicial especifico
    int cont = 0;                                     //contador de ticks para el cambio del backround
    SDL_Event event;                                  //definicion del evento
    int inControl = 1, retorno = 0;                   //comodines de permanencia en una funcion: en el menu controls y uso en el main respectivamente
    while (inControl)                                 //Loop que verifica que estemos en el menu controls
    {
        while (SDL_PollEvent(&event)) //mientas el evento "event" este activo
        {
            switch (event.type)
            {
            case SDL_QUIT: //en caso de apretar la x, se devuelve 5 al main (lo que hace que se cierre el juego)
                inControl = 0;
                retorno = 4; //salir del menu puntaje
                break;
            case SDL_MOUSEMOTION: //en caso de que el mouse se mueva, se detecta la posicion y se establece como x e y.
                SDL_GetMouseState(&x, &y);
                // x = event.motion.x;
                // y = event.motion.y;
                for (int i = 0; i < 1; i++)
                {
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h) //si la posicion del mouse coincide con la de una textura(boton)
                    {
                        button = createButton(button, pathButtons[1], &pos[i]); //cambia el estado inicial de la textura a un boton presionado
                        continue;
                    }
                    else
                    {
                        button = createButton(button, pathButtons[0], &pos[i]); //cambia de vuelta el estado inicial de la textura a un boton sin presionar
                        continue;
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN: //si se hace click izquierdo con el mouse
                SDL_GetMouseState(&x, &y);
                //x = event.button.x;
                //y = event.button.y;
                for (int i = 0; i < buttonNum; i++)
                {
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h) //si la posicion del mouse coincide con la de una textura(boton)
                    {
                        switch (i)
                        {
                        case 0:
                            Mix_VolumeChunk(bot2, VolM); // agregamos efecto de sonido a los botones mientras se esta en controles
                            Mix_PlayChannel(2, bot2, 0);
                            retorno = 1; //se devuelve 1 al main lo que significa volver al menu principal
                            inControl = 0;
                            break;
                        default:
                            break;
                        }
                    }
                }
                break;
            case SDL_KEYDOWN:                            //si se pareta alguna tecla del teclado
                if (event.key.keysym.sym == SDLK_ESCAPE) //si esa tecla es esc
                {
                    retorno = 1; //se devuelve 1 al main lo que significa volver al menu principal
                    inControl = 0;
                }
                break;
            }
        }
        fondo = LoadTexture(paths[cont / 10]); //cambio de textura del fondo
        if (cont == 25)                        //cada cuenta hasta 25 reinicia el loop de cambio detextura
            cont = 0;
        else
            cont += 1;
        SDL_RenderClear(renderer);                       //se limpia el renderer
        SDL_RenderCopy(renderer, fondo, NULL, NULL);     //se presentan las texturas del fondo al renderizador
        SDL_RenderCopy(renderer, button, NULL, &pos[0]); //se presentan las texturas de los botones al renderizador
        SDL_RenderPresent(renderer);                     //se presenta el renderizador
        SDL_UpdateWindowSurface(screen);                 //se recarga la superficie
    }
    SDL_DestroyTexture(fondo);
    SDL_DestroyTexture(button);
    return retorno; //se devuelve el valor dependiendo de lo seleccionado en el event loop
}

int Menu(Tetris *game)
{
    int frame = 0;
    SDL_Texture *images[menuNum + 1] = {NULL};
    Mix_PauseMusic();             //se pausas el tema de juego
    Mix_VolumeChunk(pausa, VolM); // se reproduce el tema del menu
    if (Mix_Paused(3))            // se reproduce si no esta sonando ya
        Mix_Resume(3);

    char *paths[] = {
        //arreglo de string a path de las imagenes del titulo  y el fondo en uso
        "assets/title/title1.png",  //0
        "assets/title/title2.png",  //1
        "assets/title/title3.png",  //2
        "assets/title/title4.png",  //3
        "assets/title/title5.png",  //4
        "assets/title/title6.png",  //5
        "assets/title/title7.png",  //6
        "assets/title/title8.png",  //7
        "assets/title/title9.png",  //8
        "assets/title/title10.png", //9
        "assets/title/title12.png", //10
        "assets/title/title11.png", //11
        "assets/title/title13.png", //12
        "assets/title/title14.png", //13
        "assets/title/title15.png", //14
        "assets/title/title16.png", //15
        "assets/title/title17.png", //16
        "assets/title/title18.png", //17
        "assets/title/title19.png", //18
        "assets/title/title20.png", //19
        "assets/title/title21.png", //20
        "assets/title/title22.png", //21
        "assets/title/title23.png", //22
        "assets/title/title24.png", //23
        "assets/title/title25.png", //24
        "assets/title/title26.png", //25
        "assets/title/title27.png", //26
        "assets/backrounds/m1.png", //27
    };
    char *pathsButtons[] = {"assets/buttons/button1.png", //arreglo de string a path de las imagenes de los botones en uso
                            "assets/buttons/button4.png",
                            "assets/buttons/button3.png",
                            "assets/buttons/button5.png",
                            "assets/buttons/button8.png",
                            "assets/buttons/button11.png",
                            "assets/buttons/button10.png",
                            "assets/buttons/button12.png"};
    int x, y;                                                      //coordenadas del mouse
    int cont = 0;                                                  //contador de ticks para el cambio del backround
    SDL_Rect pos[menuNum + 1];                                     //uso de pos para definir la posicion de los rect mas adelante
    DefinirTexturasMenu(images, paths, pos, pathsButtons, &frame); //reestructuracion de texturas del main para su uso como boton en un estado inicial especifico
    SDL_Event event;                                               //definicion del evento
    int inMenu = 1, retorno = 0;                                   //comodines de en menu y el valor de retorno al main
    while (inMenu)                                                 //mientras se este en el menu
    {
        // if (Mix_Playing(3) == 0)
        //     Mix_PlayChannel(3, pausa, -1); //reproducimos el tema de pausa/menu si no esta sonando ya

        while (SDL_PollEvent(&event) != 0) //mientas el evento "event" este activo
        {
            switch (event.type)
            {
            case SDL_QUIT:   //en caso de apretar la x, se devuelve 5 al main (lo que hace que se cierre el juego)
                retorno = 5; //salir del menu puntaje
                inMenu = 0;
                break;
            case SDL_MOUSEMOTION: //en caso de que el mouse se mueva, se detecta la posicion y se establece como x e y.
                SDL_GetMouseState(&x, &y);
                // x = event.motion.x;
                // y = event.motion.y;
                for (int i = 0; i < menuNum; i++)
                {
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h) //si la posicion del mouse coincide con la de una textura(boton)
                    {
                        images[i] = createButton(images[i], pathsButtons[i + 4], &pos[i]); //cambia el estado inicial de la textura a un boton presionado
                        continue;
                    }
                    else
                    {
                        images[i] = createButton(images[i], pathsButtons[i], &pos[i]); //cambia de vuelta el estado inicial de la textura a un boton sin presionar
                        continue;
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN: //si se hace click izquierdo con el mouse
                SDL_GetMouseState(&x, &y);
                //x = event.button.x;
                //y = event.button.y;
                for (int i = 0; i < menuNum; i++)
                {
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h) //si la posicion del mouse coincide con la de una textura(boton)
                    {
                        switch (i)
                        {
                        case 0:
                            Mix_VolumeChunk(bot, VolM); //se agrega efectos de sonido a los botones del menu
                            Mix_PlayChannel(2, bot, 0);
                            Mix_Pause(3);
                            retorno = 0;
                            inMenu = 0;
                            break;
                        case 1:
                            Mix_VolumeChunk(bot2, VolM); //se agrega  efectos de sonido a los botones del menu
                            Mix_PlayChannel(2, bot2, 0);
                            inMenu = 0;
                            retorno = Puntajes(game);
                            break;
                        case 2:
                            Mix_VolumeChunk(bot2, VolM); //se agrega efectos de sonido a los botones del menu
                            Mix_PlayChannel(2, bot2, 0);
                            inMenu = 0;
                            retorno = controls();
                            break;
                        case 3:
                            Mix_VolumeChunk(bot2, VolM); //se agrega efectos de sonido a los botones del menu
                            Mix_PlayChannel(2, bot2, 0);
                            retorno = 4;
                            inMenu = 0;
                            break;
                        }
                    }
                }
                break;
            case SDL_KEYDOWN: //si se pareta alguna tecla del teclado
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    retorno = 5; //Exit
                    inMenu = 0;
                }
                break;
            }
        }
        if (cont % 5 == 0) //cambio de textura del fondo
        {
            frame++; //segun el contador frame
            if (frame >= 26)
                frame = 0;
            images[4] = LoadTexture(paths[frame]); //animacion del titulo con  cambio de textura
        }
        if (cont == 35)
            cont = 0;
        else
            cont += 1;

        SDL_RenderClear(renderer);                             //se limpia el renderer
        SDL_RenderCopy(renderer, game->fondos[0], NULL, NULL); //se presenta la textura actual del titulo
        for (int i = 0; i < menuNum + 1; i++)                  //se copian las texturas de los botones en el renderer
        {
            SDL_RenderCopy(renderer, images[i], NULL, &pos[i]);
        }
        SDL_RenderPresent(renderer);     //se presenta el tenderer
        SDL_UpdateWindowSurface(screen); //se actualiza la pantalla
    }

    CleanTextures(images, menuNum + 1);
    return retorno; //valor de retorno al main
}

int pause(SDL_Texture **texturas, Tetris *game)
{
    int x, y;                                       //coordenadas del mouse
    Mix_PlayChannel(1, pausa, -1);                  //se reproduce la musica de pausa
    char *button[] = {"assets/buttons/button6.png", //arreglo de string a path de las imagenes del fondo y los botnes
                      "assets/buttons/button4.png",
                      "assets/buttons/button3.png",
                      "assets/buttons/button7.png",
                      "assets/buttons/button13.png",
                      "assets/buttons/button11.png",
                      "assets/buttons/button10.png",
                      "assets/buttons/button14.png",
                      "assets/backrounds/pause.png"};

    SDL_Color color[2] = {{47, 227, 197, 255}, //color de la fuente
                          {233, 148, 58, 255}};
    SDL_Rect pos[menuNum];                              //array de rects que se usara mas adelante
    DefinirTexturasPause(texturas, pos, button, color); //reestructuracion de texturas del main para su uso como boton en un estado inicial especifico
    SDL_Event event;                                    //definicion del evento
    int inPause = 1, retorno;                           //comodines de en el menu de pausa y valor de retorno al main
    while (inPause)                                     //mientras este en el menu de pausa
    {
        // Mix_PauseMusic();             //coment dejamos de reproducir el tema de juego
        Mix_VolumeChunk(pausa, VolM); //definimos el volumen del chunk "pausa"
        if (Mix_Paused(3))
            Mix_Resume(3); //reproducimos el tema de pausa/menu si no esta sonando ya

        while (SDL_PollEvent(&event)) //mientas el evento "event" este activo
        {
            switch (event.type)
            {
            case SDL_QUIT: //en caso de apretar la x, se devuelve 5 al main (lo que hace que se cierre el juego)
                retorno = 5;
                inPause = 0; //salir del menu de pausa
                break;
            case SDL_MOUSEMOTION: //en caso de que el mouse se mueva, se detecta la posicion y se establece como x e y.
                SDL_GetMouseState(&x, &y);
                //x = event.motion.x;
                // y = event.motion.y;
                for (int i = 0; i < menuNum; i++)
                {
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h) //si la posicion del mouse coincide con la de una textura(boton)
                    {
                        texturas[i] = createButton(texturas[i], button[i + 4], &pos[i]);
                        continue;
                    }
                    else
                    {
                        texturas[i] = createButton(texturas[i], button[i], &pos[i]); //cambia de vuelta el estado inicial de la textura a un boton sin presionar
                        continue;
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN: //si se hace click izquierdo con el mouse
                SDL_GetMouseState(&x, &y);
                //x = event.button.x;
                //y = event.button.y;
                for (int i = 0; i < menuNum; i++)
                {
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h) //si la posicion del mouse coincide con la de una textura(boton)
                    {
                        switch (i)
                        {
                        case 0:
                            Mix_PlayChannel(2, bot, 0); // le damos efectos de sonido a los botones del menu de pausa
                            Mix_Pause(3);
                            inPause = 0;
                            retorno = 1;
                            break;
                        case 1:
                            Mix_PlayChannel(2, bot2, 0);
                            inPause = Puntajes(game);
                            break;
                        case 2:
                            Mix_PlayChannel(2, bot2, 0);
                            inPause = controls();
                            break;
                        case 3:
                            Mix_PlayChannel(2, bot2, 0);
                            retorno = 0;
                            inPause = 0;
                            break;
                        }
                    }
                }
                break;
            case SDL_KEYDOWN: //si se pareta alguna tecla del teclado
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    retorno = 1; //Exit
                    inPause = 0;
                }
                break;
            }
        }
        SDL_RenderClear(renderer);                             //se limpia el renderer
        SDL_RenderCopy(renderer, game->fondos[3], NULL, NULL); //se copian en el renderer las texturas del backround
        for (int i = 0; i < menuNum; i++)                      //se copian en el renderer las texturas de los botones
        {
            SDL_RenderCopy(renderer, texturas[i], NULL, &pos[i]);
        }
        SDL_RenderPresent(renderer);     //se presenta el renderer
        SDL_UpdateWindowSurface(screen); //se actualiza la pantalla
    }
    return retorno; //valor de retorno al main
}

void DrawPlayGround(Tablero *t)
{
    for (int y = 0; y < HEIGHT; ++y)
    {
        for (int x = 0; x < WIDTH; ++x)
        {
            if (t->pos[y][x] != NULL)
                DrawCubes(x, y, t->pos[y][x]);
        }
    }
}

void CambioCoord(Coor *coordenadas)
{
    int aux = coordenadas->y;
    coordenadas->y = coordenadas->x;
    coordenadas->x = -aux;
}

void RotarPieza(Piezas *pieza)
{
    for (int i = 0; i < 3; ++i)
    {
        CambioCoord(&pieza->laterales[i]);
    }
}

void LimpiarTablero(Tablero *t, SDL_Texture *images[])
{
    for (int y = 0; y < HEIGHT; ++y)
    {
        for (int x = 0; x < WIDTH; ++x)
        {
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

void UpdateTablero(Tablero *t, SDL_Texture *images[])
{
    for (int y = 0; y < HEIGHT; ++y)
    {
        for (int x = 0; x < WIDTH; ++x)
        {
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
        }
    }
}

void UpdateBorde(Tablero *t, SDL_Texture **texturas, char *paths[], int *tick, int *control, int *direccion)
{
    if (*tick % 2 == 0)
    {
        switch (*direccion)
        {
        case 1:
            *control += 4;
            break;
        case -1:
            *control -= 4;
            break;
        }
        texturas[0] = LoadTexture(paths[0 + *control]);
        texturas[1] = LoadTexture(paths[3 + *control]);
        texturas[2] = LoadTexture(paths[1 + *control]);
        texturas[3] = LoadTexture(paths[2 + *control]);
        UpdateTablero(t, texturas);
        if (*control >= 24 || *control <= 0)
            *direccion *= -1;
    }
}

void DetensionPieza(Tablero *t, Piezas *p)
{
    for (int i = 0; i < 4; ++i)
    {
        Coor pos = PiezaPos(i, p);
        t->pos[(int)pos.y][(int)pos.x] = p->images;
    }
}

int HayColision(Tablero *t, Piezas *p, SDL_Texture **texturas)
{
    for (int i = 0; i < 4; ++i)
    {
        Coor pos = PiezaPos(i, p);
        if (t->pos[(int)pos.y][(int)pos.x] == texturas[0])
            return 2;
        if (t->pos[(int)pos.y][(int)pos.x] != NULL)
            return 1;
    }
    return 0;
}

int FilaCompleta(Tablero *t, int fila)
{
    for (int x = 1; x < WIDTH - 1; ++x)
    {
        if (t->pos[fila][x] == NULL)
            return 0;
    }
    return 1;
}

void EliminarFila(Tablero *t, int fila)
{
    for (int y = fila; y > 0; --y)
    {
        for (int x = 1; x < WIDTH - 1; ++x)
        {
            t->pos[y][x] = t->pos[y - 1][x];
        }
    }
    for (int x = 1; x < WIDTH - 1; ++x)
    {
        t->pos[0][x] = NULL;
    }
}

int LineasELiminadas(Tablero *t)
{
    int actRow = HEIGHT - 2, lineasElim = 0;
    while (actRow > 0)
    {
        if (FilaCompleta(t, actRow))
        {
            EliminarFila(t, actRow);
            lineasElim++;
        }
        else
            actRow--;
    }
    return lineasElim;
}

void Update(Tetris *game, SDL_Rect *rects, SDL_Texture **texturas)
{
    texturas[8] = ImprimirNumeros(texturas[8], &rects[3], &game->score.puntos, &blanco, game->score.sPuntos, 28);
    texturas[9] = ImprimirNumeros(texturas[9], &rects[4], &game->score.lineasEliminadas, &blanco, game->score.slineas, 28);
    rects[3].x = TAM * 15 - (rects[3].w / 2);
    rects[3].y = 330 - (rects[3].h / 2);
    rects[4].x = TAM * 15 - (rects[4].w / 2);
    rects[4].y = 505 - (rects[4].h / 2);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, game->fondos[1], NULL, &rects[5]);
    SDL_RenderCopy(renderer, game->fondos[2], NULL, &rects[6]);
    DrawPlayGround(&game->tablero); // ocupa x = 0 , x = WIDTH -1  y = HEIGHT -1
    DrawFigure(&game->actFigure);
    DrawFigure(&game->nextFigure);
    for (int i = 0; i < 2; ++i)
    {
        SDL_RenderCopy(renderer, texturas[i + 8], NULL, &rects[i + 3]);
    }
    SDL_RenderPresent(renderer);
    SDL_UpdateWindowSurface(screen);
}

void LeerRecords(FILE *file, Records *top10)
{
    file = fopen("assets/record.txt", "r");
    if (file == NULL)
        printf("no se puedo abrir los records");
    int recordAct = 0;
    while (!feof(file))
    {
        fscanf(file, "%s %lld", top10[recordAct].nombre, &top10[recordAct].puntaje);
        printf("lugar %d jugador %s, puntaje %lld\n", recordAct + 1, top10[recordAct].nombre, top10[recordAct].puntaje);
        recordAct++;
    }
}

void UpdateRecords(FILE *file, Records *top10, Records *newPlayer)
{
    file = fopen("assets/record.txt", "w");
    if (newPlayer->puntaje > top10[0].puntaje)
    {
        for (int k = 9; k > 0; --k)
        {
            top10[k] = top10[k - 1];
        }
        top10[0] = *newPlayer;
    }
    else if (top10[9].puntaje < newPlayer->puntaje)
    {
        top10[9] = *newPlayer;
        for (int i = 0; i < 10; i++)
        {
            for (int j = i + 1; j < 10; j++)
            {
                if (top10[i].puntaje < top10[j].puntaje)
                {
                    Records aux = top10[i];
                    top10[i] = top10[j];
                    top10[j] = aux;
                }
            }
        }
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 0)
            fprintf(file, "%s %lld", top10[i].nombre, top10[i].puntaje);
        else
        {
            fprintf(file, "%c", '\n');
            fprintf(file, "%s %lld", top10[i].nombre, top10[i].puntaje);
        }
    }
    fclose(file);
}

int GameOver(Tetris *game)
{
    Mix_VolumeMusic(VolM); //coment le otorgamos valor al volumen de la musica de game over
    Mix_PlayMusic(GO, -1); //coment reproducimos la musica de game over

    SDL_Texture *texturas[4];
    SDL_Rect rects[4];
    SDL_Rect pos;
    SDL_Texture *textoS = NULL;
    textoS = ImprimirTexto(textoS, &pos, "Presione Shift derecho para salir", &blancoC, 20);
    pos.x = TAM * 9 - pos.w / 2;
    pos.y = 660 - pos.h;
    Records newPlayer;
    for (int i = 0; i < 30; ++i)
    {
        newPlayer.nombre[i] = '\0';
    }
    texturas[0] = ImprimirNumeros(texturas[0], &rects[0], &game->score.puntos, &blanco, game->score.sPuntos, 28);
    rects[0].x = 12 * TAM;
    rects[0].y = 7 * TAM - rects[0].h / 2;
    texturas[1] = ImprimirTexto(texturas[1], &rects[1], "Puntaje Obtenido", &blanco, 30);
    rects[1].x = 2 * TAM;
    rects[1].y = 7 * TAM - rects[1].h / 2;
    texturas[2] = ImprimirTexto(texturas[2], &rects[2], "Ingrese Nombre:", &blanco, 26);
    rects[2].x = 2 * TAM;
    rects[2].y = 10 * TAM - rects[2].h / 2;
    newPlayer.puntaje = game->score.puntos;
    texturas[3] = ImprimirTexto(texturas[3], &rects[3], newPlayer.nombre, &blanco, 26);
    rects[3].x = 12 * TAM;
    rects[3].y = 10 * TAM - rects[3].h / 2;
    int inputName = 1, retorno = -1, cont = 0, lessOne = -1;
    SDL_Event event;
    while (inputName)
    {
        SDL_StartTextInput();
        while (SDL_PollEvent(&event) != 0)
        {
            switch (event.type)
            {
            case SDL_QUIT:
                inputName = 0;
                break;
            case SDL_TEXTINPUT:
                strcat(newPlayer.nombre, event.text.text);
                texturas[3] = ImprimirTexto(texturas[3], &rects[3], newPlayer.nombre, &blanco, 26);
                rects[3].x = 12 * TAM;
                rects[3].y = 10 * TAM - rects[3].h / 2;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_BACKSPACE:
                    if (strlen(newPlayer.nombre) > 0)
                    {
                        newPlayer.nombre[strlen(newPlayer.nombre) - 1] = '\0';
                        texturas[3] = ImprimirTexto(texturas[3], &rects[3], newPlayer.nombre, &blanco, 26);
                        rects[3].x = 12 * TAM;
                        rects[3].y = 10 * TAM - rects[3].h / 2;
                    }
                    break;
                case SDLK_RSHIFT:
                    inputName = 0;
                    retorno = 1;
                    SDL_StopTextInput();
                    break;
                }
                break;
            }
        }

        if (cont % 5 == 0)
        {
            if (blancoC.a >= 255)
                lessOne = -1;
            if (blancoC.a <= 10)
                lessOne = 1;
            textoS = ImprimirTexto(textoS, &pos, "Presione Shift derecho para salir", &blancoC, 20);
            blancoC.a += lessOne * 5;
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, game->fondos[4], NULL, NULL);
        SDL_RenderCopy(renderer, texturas[0], NULL, &rects[0]);
        SDL_RenderCopy(renderer, texturas[1], NULL, &rects[1]);
        SDL_RenderCopy(renderer, texturas[2], NULL, &rects[2]);
        SDL_RenderCopy(renderer, texturas[3], NULL, &rects[3]);
        SDL_RenderCopy(renderer, textoS, NULL, &pos);
        SDL_RenderPresent(renderer);
        SDL_UpdateWindowSurface(screen);
    }
    UpdateRecords(game->DB, game->top10, &newPlayer);
    blancoC.a = 255;
    CleanTextures(texturas, 4);
    free(game->score.sPuntos);
    free(game->score.slineas);
    return retorno;
}

int CountDown(Tetris *game, SDL_Texture **texturas, SDL_Rect *rects)
{
    SDL_Rect pos;
    long long time = 5;
    char sNum[2] = {'\0'};
    SDL_Texture *texto = NULL;
    texto = ImprimirNumeros(texto, &pos, &time, &blanco, sNum, 100);
    pos.x = TAM * 9 - pos.w / 2;
    pos.y = (TAM * HEIGHT) / 2 - pos.h / 2;
    int preGame = 1, retorno = 1;
    SDL_Event ev;
    while (preGame)
    {
        while (SDL_PollEvent(&ev) != 0)
        {
            if (ev.type == SDL_QUIT)
            {
                preGame = 0;
                retorno = 0;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, game->fondos[1], NULL, &rects[5]);
        SDL_RenderCopy(renderer, game->fondos[2], NULL, &rects[6]);
        DrawPlayGround(&game->tablero); // ocupa x = 0 , x = WIDTH -1  y = HEIGHT -1
        SDL_RenderCopy(renderer, texto, NULL, &pos);
        SDL_RenderPresent(renderer);
        SDL_UpdateWindowSurface(screen);
        SDL_Delay(1000);
        time--;
        texto = ImprimirNumeros(texto, &pos, &time, &blanco, sNum, 100);
        pos.x = TAM * 9 - pos.w / 2;
        pos.y = (TAM * HEIGHT) / 2 - pos.h / 2;
        if (time == 0)
            preGame = 0;
    }
    SDL_DestroyTexture(texto);
    return retorno;
}

int BeforeGame(Tetris *game, SDL_Texture **texturas, SDL_Rect *rects)
{
    SDL_Texture *images[2] = {NULL};
    SDL_Rect pos;
    images[0] = LoadTexture("assets/backrounds/c1.png");
    images[1] = ImprimirTexto(images[1], &pos, "Presione Espacio cuando este listo", &blancoC, 20);
    pos.x = TAM * 9 - pos.w / 2;
    pos.y = TAM * (HEIGHT - 1) - pos.h / 2;
    SDL_Event ev;
    int preGame = 1, retorno = 0, cont = 0, lessOne = -1;
    while (preGame)
    {
        while (SDL_PollEvent(&ev) != 0)
        {
            switch (ev.type)
            {
            case SDL_QUIT:
                preGame = 0;
                break;

            case SDL_KEYDOWN:
                switch (ev.key.keysym.sym)
                {
                case SDLK_SPACE:
                    retorno = 1;
                    preGame = 0;
                    break;
                default:
                    break;
                }
            default:
                break;
            }
        }
        if (cont % 5 == 0)
        {
            if (blancoC.a >= 255)
                lessOne = -1;
            if (blancoC.a <= 10)
                lessOne = 1;
            images[1] = ImprimirTexto(images[1], &pos, "Presione Espacio cuando este listo", &blancoC, 20);
            blancoC.a += lessOne * 5;
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, images[0], NULL, NULL);
        SDL_RenderCopy(renderer, images[1], NULL, &pos);
        SDL_RenderPresent(renderer);
        SDL_UpdateWindowSurface(screen);
    }
    blancoC.a = 255;
    CleanTextures(images, 2);
    if (retorno == 1)
        return CountDown(game, texturas, rects);
    return retorno;
}

int onColision(Tetris *game, Piezas *aux,
               SDL_Texture **texturas, SDL_Rect *rects, SDL_Event *event, int *down)
{
    int paux;
    if (HayColision(&game->tablero, &game->actFigure, texturas))
    {
        game->actFigure = *aux;
        for (int i = 0; i < 4; ++i)
        {
            if ((PiezaPos(i, &game->actFigure).y <= 0 || game->actFigure.central.y == 1) && HayColision(&game->tablero, &game->actFigure, texturas) == 1)
            {
                return 0;
            }
        }
        if (HayColision(&game->tablero, &game->actFigure, texturas) != 2 && *down)
        {
            DetensionPieza(&game->tablero, &game->actFigure);
            paux = LineasELiminadas(&game->tablero);
            game->score.puntos += paux == 1 ? paux * 25 : paux * paux * 100;
            game->score.lineasEliminadas += paux;
            game->actFigure = game->nextFigure;
            texturas[4] = LoadTexture(colors[rand() % 8]);
            CrearPieza(&game->nextFigure, texturas[4]);
            game->actFigure.central.x = 6;
            game->actFigure.central.y = 1;
            down = 0;
        }
    }
    return 1;
}

int play(Tetris *game)
{
    srand(time(NULL));
    SDL_Texture *images[10] = {NULL};
    CrearTableroEnMemoria(game->tablero.pos);
    char *paths[28];
    CrearArregloPath(paths);
    InitPuntiacion(&game->score);
    SDL_Rect rects[] = {{520, 0, 0, 0},                    //0
                        {520, 200, 0, 0},                  //1
                        {520, 400, 0, 0},                  //2
                        {15 * TAM, 310, 0, 0},             //3
                        {15 * TAM, 490, 0, 0},             //4
                        {0, 0, TAM * WIDTH, TAM * HEIGHT}, //5
                        {WIDTH * TAM, 0, TAM * 6, TAM * HEIGHT}};

    DefinirTexturasGame(game, images, paths, rects);
    LimpiarTablero(&game->tablero, images);
    SDL_Event event;
    Update(game, rects, images);
    int play = BeforeGame(game, images, rects), tick = 0, down = 0, control = 0, direccion = 1, retorno = -1;
    while (play)
    {
        Mix_Pause(1);          //coment pausamos el tema del menu
        Mix_VolumeMusic(VolM); //coment reproducimos el tema de juego
        if (Mix_PlayingMusic() == 0)
        {                           //hacemos que se reproduzca si no esta sonando
            Mix_PlayMusic(bgm, -1); //coment el -1 es para que se reproduzca infinitamente
        }
        Piezas aux = game->actFigure;
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                retorno = 5;
                play = 0;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    RotarPieza(&game->actFigure);
                    down = 0;
                    break;
                case SDLK_DOWN:
                    game->actFigure.central.y++;
                    down = 1;
                    break;
                case SDLK_LEFT:
                    game->actFigure.central.x--;
                    down = 0;
                    break;
                case SDLK_RIGHT:
                    game->actFigure.central.x++;
                    down = 0;
                    break;
                case SDLK_ESCAPE:
                    Mix_PauseMusic();
                    play = pause(images, game);
                    if (!play)
                    {
                        retorno = 1;
                    }
                    else
                        Mix_ResumeMusic();
                }
            }
        }
        if (play)
        {
            if (tick % 10 == 0)
                UpdateBorde(&game->tablero, images, paths, &tick, &control, &direccion);
            if (tick % 20 == 0)
            {
                game->actFigure.central.y++;
                tick = 0;
                down = 1;
            }
            play = onColision(game, &aux, images, rects, &event, &down);
            if (!play)
                retorno = 2;
            if (play)
            {
                Update(game, rects, images);
                //SDL_Delay(20);
                tick++;
            }
        }
    }
    CleanTextures(images, 10);
    return retorno;
}

void InitFondos(Tetris *game)
{
    game->fondos[0] = LoadTexture("assets/backrounds/m1.png");
    game->fondos[1] = LoadTexture("assets/backrounds/Board.png");
    game->fondos[2] = LoadTexture("assets/backrounds/info.png");
    game->fondos[3] = LoadTexture("assets/backrounds/pause.png");
    game->fondos[4] = LoadTexture("assets/backrounds/gameOver.png");
    game->fondos[5] = LoadTexture("assets/backrounds/scoreboard.png");
}

void Close()
{

    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(screen);
    screen = NULL;
    IMG_Quit();
    TTF_Quit();
    Mix_FreeMusic(bgm); //coment liberamos todos los recursos de musica que cargamos
    Mix_FreeMusic(GO);
    Mix_FreeChunk(pausa);
    Mix_FreeChunk(sfx);
    Mix_FreeChunk(bot);
    Mix_FreeChunk(bot2);
    Mix_Quit(); //cerramos sdl_mixer
    SDL_Quit();
}
