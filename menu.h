#define menuNum  4
#define buttonNum 4



SDL_Texture* LoadTexture(char* path, SDL_Renderer* screen) {
    SDL_Texture* textura = NULL;
    SDL_Surface* surface = NULL;
    surface = IMG_Load(path);
    if (surface == NULL)
        printf("No se pudo cargar la imagen: %s\n", IMG_GetError());
    else {
        textura = SDL_CreateTextureFromSurface(screen, surface);
        if (textura == NULL)
            printf("Error al generar textura: %s\n", SDL_GetError());
    }

    SDL_FreeSurface(surface);
    surface = NULL;
    return textura;
}

void createButton(SDL_Renderer* screen, TTF_Font* font, SDL_Texture* menus[], const char* button, SDL_Color* color, SDL_Rect* pos, int cont) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, button, color[0]);
    menus[cont] = SDL_CreateTextureFromSurface(screen, textSurface);
    SDL_QueryTexture(menus[cont], NULL, NULL, &pos[cont].w, &pos[cont].h);
    SDL_FreeSurface(textSurface);
    textSurface = NULL;
}

int menu(SDL_Renderer* screen, TTF_Font* font) {
    Uint32 time;
    int x, y;//coordenadas del mouse

    SDL_Texture* currentImage[] = { LoadTexture("assets/m1.png",screen),
                                    LoadTexture("assets/m2.png",screen),
                                    LoadTexture("assets/m3.png",screen),
                                    LoadTexture("assets/m4.png",screen),
                                    LoadTexture("assets/m5.png",screen),
                                    LoadTexture("assets/m6.png",screen),
                                    LoadTexture("assets/m7.png",screen),
    };
    int cont = 0;

    const char* button[menuNum] = { "Play","Settings","Controls","Exit" };//2 = cantidad de botones en el menu
    SDL_Texture* menus[menuNum];

    SDL_Color color[2] = { {47,227,197,255},{233,148,58,255} };
    SDL_Rect pos[menuNum];

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, button[0], color[0]);
    for (int cont = 0; cont < menuNum; cont++)
    {
        createButton(screen, font, menus, button[cont], &color, pos, cont);
        pos[cont].x = 300;
        pos[cont].y = 250 + 50 * cont;
    }

    SDL_Event event;
    while (1)
    {

        time = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                return 1;
            case SDL_MOUSEMOTION:
                x = event.motion.x;
                y = event.motion.y;
                for (int i = 0; i < menuNum; i++)
                {
                    int selected = 0;
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h)
                    {
                        selected = 1;
                        textSurface = TTF_RenderText_Solid(font, button[i], color[1]);
                        menus[i] = SDL_CreateTextureFromSurface(screen, textSurface);
                        SDL_QueryTexture(menus[i], NULL, NULL, &pos[i].w, &pos[i].h);
                        SDL_FreeSurface(textSurface);
                        textSurface = NULL;
                        continue;
                    }
                    else
                    {
                        selected = 0;
                        textSurface = TTF_RenderText_Solid(font, button[i], color[0]);
                        menus[i] = SDL_CreateTextureFromSurface(screen, textSurface);
                        SDL_QueryTexture(menus[i], NULL, NULL, &pos[i].w, &pos[i].h);
                        SDL_FreeSurface(textSurface);
                        textSurface = NULL;
                        continue;
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                x = event.button.x;
                y = event.button.y;
                for (int i = 0; i < menuNum; i++)
                {
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h)
                    {
                        switch (i)
                        {
                        case 0:
                            return 1;
                        case 1:
                            settings(screen, font);
                            break;
                        case 2:
                            controls(screen, font);
                            break;
                        case 3:
                            return 0;
                        }
                    }
                }
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    return 0;//Exit
                }
                break;

            }
        }

        SDL_RenderClear(screen);
        SDL_RenderCopy(screen, currentImage[cont / 5], NULL, NULL);
        if (cont == 30)cont = 0;
        else cont += 1;

        for (int i = 0; i < menuNum; i++)
        {
            SDL_RenderCopy(screen, menus[i], NULL, &pos[i]);
        }
        SDL_RenderPresent(screen);
        SDL_UpdateWindowSurface(screen);
    }
    SDL_DestroyTexture(currentImage);
    *currentImage = NULL;
}

int settings(SDL_Renderer* screen, TTF_Font* font) {
    Uint32 time;
    int x, y;//coordenadas del mouse

    SDL_Texture* currentImage[] = { LoadTexture("assets/c1.png",screen),
                                LoadTexture("assets/c2.png",screen),
                                LoadTexture("assets/c3.png",screen),
                                LoadTexture("assets/c4.png",screen),
                                LoadTexture("assets/c5.png",screen),
                                LoadTexture("assets/c6.png",screen),
    };

    char* button[] = { "BACK","Volume","more","less" };//2 = cantidad de botones en el menu
    SDL_Texture* menus[buttonNum];

    SDL_Color color[2] = { {47,227,197,255},{233,148,58,255} };
    SDL_Rect pos[buttonNum];

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, button[0], color[0]);

    for (int cont = 0; cont < menuNum; cont++)
    {
        createButton(screen, font, menus, button[cont], &color, pos, cont);
        pos[cont].x = 300;
        pos[cont].y = 250 + 50 * cont;
    }
    pos[0].x = 40;
    pos[0].y = 40;

    SDL_Event event;
    while (1)
    {
        time = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                return 1;
            case SDL_MOUSEMOTION:
                x = event.motion.x;
                y = event.motion.y;
                for (int i = 0; i < buttonNum; i++)
                {
                    int selected = 0;
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h)
                    {
                        selected = 1;
                        textSurface = TTF_RenderText_Solid(font, button[i], color[1]);
                        menus[i] = SDL_CreateTextureFromSurface(screen, textSurface);
                        SDL_QueryTexture(menus[i], NULL, NULL, &pos[i].w, &pos[i].h);
                        SDL_FreeSurface(textSurface);
                        textSurface = NULL;
                        continue;
                    }
                    else
                    {
                        selected = 0;
                        textSurface = TTF_RenderText_Solid(font, button[i], color[0]);
                        menus[i] = SDL_CreateTextureFromSurface(screen, textSurface);
                        SDL_QueryTexture(menus[i], NULL, NULL, &pos[i].w, &pos[i].h);
                        SDL_FreeSurface(textSurface);
                        textSurface = NULL;
                        continue;
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                x = event.button.x;
                y = event.button.y;
                for (int i = 0; i < buttonNum; i++)
                {
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h)
                    {
                        return 0;
                    }
                }
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    return 0;//Exit
                }
                break;

            }
        }
        SDL_RenderClear(screen);
        SDL_RenderCopy(screen, currentImage[0], NULL, NULL);
        for (int i = 0; i < buttonNum; i++)
        {
            SDL_RenderCopy(screen, menus[i], NULL, &pos[i]);
        }
        SDL_RenderPresent(screen);
        SDL_UpdateWindowSurface(screen);
    }
}

int controls(SDL_Renderer* screen, TTF_Font* font) {
    Uint32 time;
    int x, y;//coordenadas del mouse

    SDL_Texture* currentImage[] = { LoadTexture("assets/s1.png",screen),
                            LoadTexture("assets/s2.png",screen),
                            LoadTexture("assets/s3.png",screen),
                            LoadTexture("assets/s4.png",screen),
                            LoadTexture("assets/s5.png",screen),
                            LoadTexture("assets/s6.png",screen),
                            LoadTexture("assets/s7.png",screen),
    };

    const char* button[1] = { "BACK" };//2 = cantidad de botones en el menu
    SDL_Texture* menus[1];

    SDL_Color color[2] = { {47,227,197,255},{233,148,58,255} };
    SDL_Rect pos[1];

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, button[0], color[0]);
    createButton(screen, font, menus, button[0], &color, pos, 0);
    pos[0].x = 40;
    pos[0].y = 40;

    SDL_Event event;
    while (1)
    {
        time = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                return 1;
            case SDL_MOUSEMOTION:
                x = event.motion.x;
                y = event.motion.y;
                for (int i = 0; i < 1; i++)
                {
                    int selected = 0;
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h)
                    {
                        selected = 1;
                        textSurface = TTF_RenderText_Solid(font, button[i], color[1]);
                        menus[i] = SDL_CreateTextureFromSurface(screen, textSurface);
                        SDL_QueryTexture(menus[i], NULL, NULL, &pos[i].w, &pos[i].h);
                        SDL_FreeSurface(textSurface);
                        textSurface = NULL;
                        continue;
                    }
                    else
                    {
                        selected = 0;
                        textSurface = TTF_RenderText_Solid(font, button[i], color[0]);
                        menus[i] = SDL_CreateTextureFromSurface(screen, textSurface);
                        SDL_QueryTexture(menus[i], NULL, NULL, &pos[i].w, &pos[i].h);
                        SDL_FreeSurface(textSurface);
                        textSurface = NULL;
                        continue;
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                x = event.button.x;
                y = event.button.y;
                for (int i = 0; i < buttonNum; i++)
                {
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h)
                    {
                        return 0;
                    }
                }
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    return 0;//Exit
                }
                break;
            }
        }
        SDL_RenderClear(screen);
        SDL_RenderCopy(screen, currentImage[0], NULL, NULL);
        SDL_RenderCopy(screen, menus[0], NULL, &pos[0]);
        SDL_RenderPresent(screen);
        SDL_UpdateWindowSurface(screen);
    }
}

int pause(SDL_Renderer* screen, TTF_Font* font) {
    Uint32 time;
    int x, y;//coordenadas del mouse

    const char* button[menuNum] = { "Continue","Settings","Controls","Main Menu" };//2 = cantidad de botones en el menu
    SDL_Texture* menus[menuNum];

    SDL_Color color[2] = { {47,227,197,255},{233,148,58,255} };
    SDL_Rect pos[menuNum];

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, button[0], color[0]);
    for (int cont = 0; cont < menuNum; cont++)
    {
        createButton(screen, font, menus, button[cont], &color, pos, cont);
        pos[cont].x = 300;
        pos[cont].y = 250 + 50 * cont;
    }

    SDL_Event event;
    while (1)
    {
        time = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                return 1;
            case SDL_MOUSEMOTION:
                x = event.motion.x;
                y = event.motion.y;
                for (int i = 0; i < menuNum; i++)
                {
                    int selected = 0;
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h)
                    {
                        selected = 1;
                        textSurface = TTF_RenderText_Solid(font, button[i], color[1]);
                        menus[i] = SDL_CreateTextureFromSurface(screen, textSurface);
                        SDL_QueryTexture(menus[i], NULL, NULL, &pos[i].w, &pos[i].h);
                        SDL_FreeSurface(textSurface);
                        textSurface = NULL;
                        continue;
                    }
                    else
                    {
                        selected = 0;
                        textSurface = TTF_RenderText_Solid(font, button[i], color[0]);
                        menus[i] = SDL_CreateTextureFromSurface(screen, textSurface);
                        SDL_QueryTexture(menus[i], NULL, NULL, &pos[i].w, &pos[i].h);
                        SDL_FreeSurface(textSurface);
                        textSurface = NULL;
                        continue;
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                x = event.button.x;
                y = event.button.y;
                for (int i = 0; i < menuNum; i++)
                {
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h)
                    {
                        switch (i)
                        {
                        case 0:
                            return 1;
                        case 1:
                            settings(screen, font);
                            break;
                        case 2:
                            controls(screen, font);
                            break;
                        case 3:
                            return 0;
                        }
                    }
                }
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    return 0;//Exit
                }
                break;

            }
        }
        SDL_RenderClear(screen);
        for (int i = 0; i < menuNum; i++)
        {
            SDL_RenderCopy(screen, menus[i], NULL, &pos[i]);
        }
        SDL_RenderPresent(screen);
        SDL_UpdateWindowSurface(screen);
    }
}
