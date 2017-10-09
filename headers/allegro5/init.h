void init()
{
    if (!al_init())
    {
        fprintf(stderr, "Falha ao inicializar o allegro!\n");
        exit(-1);
    }
}