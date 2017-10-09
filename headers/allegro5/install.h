void install_mouse()
{
    if (!al_install_mouse())
    {
        fprintf(stderr, "Falha ao inicializar o mouse!\n");
        exit(-1);
    }
}

void install_keyboard()
{
    if (!al_install_keyboard())
    {
        fprintf(stderr, "Falha ao inicializar o teclado!\n");
        exit(-1);
    }
}