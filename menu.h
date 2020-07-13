//falta modularizar TODO

#define menuCant  4
int menu(SDL_Renderer* screen, TTF_Font* font) {
    Uint32 time;
    int x, y;//coordenadas del mouse

    const char* button[menuCant] = { "Continue","Settings","Controls","Exit" };//2 = cantidad de botones en el menu
    SDL_Texture* menus[menuCant];

    SDL_Color color[2] = { {47,227,197,255},{233,148,58,255} };
    SDL_Rect pos[menuCant];

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, button[0], color[0]);

    menus[0] = SDL_CreateTextureFromSurface(screen, textSurface);
    SDL_QueryTexture(menus[0], NULL, NULL, &pos[0].w, &pos[0].h);
    pos[0].x = 300;
    pos[0].y = 200;
    SDL_FreeSurface(textSurface);
    textSurface = NULL;

    textSurface = TTF_RenderText_Solid(font, button[1], color[0]);
    menus[1] = SDL_CreateTextureFromSurface(screen, textSurface);
    SDL_QueryTexture(menus[1], NULL, NULL, &pos[1].w, &pos[1].h);
    pos[1].x = 300;
    pos[1].y = 250;
    SDL_FreeSurface(textSurface);
    textSurface = NULL;

    textSurface = TTF_RenderText_Solid(font, button[2], color[0]);
    menus[2] = SDL_CreateTextureFromSurface(screen, textSurface);
    SDL_QueryTexture(menus[2], NULL, NULL, &pos[2].w, &pos[2].h);
    pos[2].x = 300;
    pos[2].y = 300;
    SDL_FreeSurface(textSurface);
    textSurface = NULL;

    textSurface = TTF_RenderText_Solid(font, button[3], color[0]);
    menus[3] = SDL_CreateTextureFromSurface(screen, textSurface);
    SDL_QueryTexture(menus[3], NULL, NULL, &pos[3].w, &pos[3].h);
    pos[3].x = 300;
    pos[3].y = 350;
    SDL_FreeSurface(textSurface);
    textSurface = NULL;

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
                for (int i = 0; i < menuCant; i++)
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
                for (int i = 0; i < menuCant; i++)
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
        SDL_RenderCopy(screen, menus[0], NULL, &pos[0]);
        SDL_RenderCopy(screen, menus[1], NULL, &pos[1]);
        SDL_RenderCopy(screen, menus[2], NULL, &pos[2]);
        SDL_RenderCopy(screen, menus[3], NULL, &pos[3]);
        SDL_RenderPresent(screen);
        SDL_UpdateWindowSurface(screen);
    }
}

int settings(SDL_Renderer* screen, TTF_Font* font) {
    Uint32 time;
    int x, y;//coordenadas del mouse

    const char* button[menuCant] = { "BACK","Volume","more","less" };//2 = cantidad de botones en el menu
    SDL_Texture* menus[menuCant];

    SDL_Color color[2] = { {47,227,197,255},{233,148,58,255} };
    SDL_Rect pos[menuCant];

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, button[0], color[0]);

    menus[0] = SDL_CreateTextureFromSurface(screen, textSurface);
    SDL_QueryTexture(menus[0], NULL, NULL, &pos[0].w, &pos[0].h);
    pos[0].x = 40;
    pos[0].y = 40;
    SDL_FreeSurface(textSurface);
    textSurface = NULL;

    menus[1] = SDL_CreateTextureFromSurface(screen, textSurface);
    SDL_QueryTexture(menus[1], NULL, NULL, &pos[1].w, &pos[1].h);
    pos[1].x = 300;
    pos[1].y = 300;
    SDL_FreeSurface(textSurface);
    textSurface = NULL;

    menus[2] = SDL_CreateTextureFromSurface(screen, textSurface);
    SDL_QueryTexture(menus[2], NULL, NULL, &pos[2].w, &pos[2].h);
    pos[2].x = 375;
    pos[2].y = 350;
    SDL_FreeSurface(textSurface);
    textSurface = NULL;

    menus[3] = SDL_CreateTextureFromSurface(screen, textSurface);
    SDL_QueryTexture(menus[3], NULL, NULL, &pos[3].w, &pos[3].h);
    pos[3].x = 275;
    pos[3].y = 350;
    SDL_FreeSurface(textSurface);
    textSurface = NULL;

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
                for (int i = 0; i < menuCant; i++)
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
                for (int i = 0; i < menuCant; i++)
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
        SDL_RenderCopy(screen, menus[0], NULL, &pos[0]);
        SDL_RenderCopy(screen, menus[1], NULL, &pos[1]);
        SDL_RenderCopy(screen, menus[2], NULL, &pos[2]);
        SDL_RenderCopy(screen, menus[3], NULL, &pos[3]);
        SDL_RenderPresent(screen);
        SDL_UpdateWindowSurface(screen);
    }
}

int controls(SDL_Renderer* screen, TTF_Font* font) {
    Uint32 time;
    int x, y;//coordenadas del mouse

    const char* button[1] = { "BACK" };//2 = cantidad de botones en el menu
    SDL_Texture* menus[1];

    SDL_Color color[2] = { {47,227,197,255},{233,148,58,255} };
    SDL_Rect pos[1];

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, button[0], color[0]);
    menus[0] = SDL_CreateTextureFromSurface(screen, textSurface);
    SDL_QueryTexture(menus[0], NULL, NULL, &pos[0].w, &pos[0].h);
    pos[0].x = 40;
    pos[0].y = 40;
    SDL_FreeSurface(textSurface);
    textSurface = NULL;


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
                for (int i = 0; i < menuCant; i++)
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
        SDL_RenderCopy(screen, menus[0], NULL, &pos[0]);
        SDL_RenderPresent(screen);
        SDL_UpdateWindowSurface(screen);
    }
}

//no funciona
void createButton(SDL_Renderer* screen, TTF_Font* font, SDL_Texture** menus, SDL_Surface* textSurface, const char* button, SDL_Color* color, SDL_Rect* pos, int cont) {
    if (cont != 0)textSurface = TTF_RenderText_Solid(font, button[cont], color[0]);
    menus[cont] = SDL_CreateTextureFromSurface(screen, textSurface);
    SDL_QueryTexture(menus[cont], NULL, NULL, &pos[cont].w, &pos[cont].h);
    pos[cont].x = 300;
    pos[cont].y = 250;
    SDL_FreeSurface(textSurface);
    textSurface = NULL;
}