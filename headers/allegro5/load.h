FONT * load_ttf_font(char const * filename, int size, int flags)
{
    FONT * font;

    if (!(font = al_load_ttf_font(filename, size, flags)))
    {
        fprintf(stderr, "Nao foi possivel carregar a font %s", filename);
        exit(-1);
    }

    return font;
}