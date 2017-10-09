void set_system_mouse_cursor(DISPLAY * display, int cursor)
{
    if (!al_set_system_mouse_cursor(display, cursor))
    {
        fprintf(stderr, "Falha ao atribuir ponteiro do mouse!\n");
        exit(-1);
    }
}