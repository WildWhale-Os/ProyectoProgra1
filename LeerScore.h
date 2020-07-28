void LeerScore(char* scores, SDL_Texture **texturas, SDL_Rect* rects)
{
    FILE* score;
    char linea[1024];
    char aux[9][100];
    int n = 0;
    score = fopen("assets/records.txt", "r");
    while (fgets(linea,100,(FILE*)score))
    {
        strcpy(aux[n], linea);
        ++n;
    }
    fclose(score);
    scores = aux;

    for (int i = 0; i < 10; i++)
    {
        texturas[i] = ImprimirTexto(texturas[i], &rects[i], scores[i], &blanco, 26);
        rects[i].x = 10;
        rects[i].y = 50 + 40*i;
    }
    return scores;
}