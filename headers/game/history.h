bool show_history;

FILE * history;
FONT * history_font;

void load_history()
{
    history = fopen("data/history/history.txt", "r");
    
    if (history == NULL)
    {
        fprintf(stderr, "Nao foi possivel abrir o arquivo 'history.txt'!\n");
        exit(-1);
    }
}

void history_draw()
{
    al_draw_text(history_font, al_map_rgb(255, 255, 255), DISPLAY_W / 2,
    ((DISPLAY_H + al_get_font_ascent(history_font) * 2)
     / FPS) * (frame % FPS)
    - al_get_font_ascent(history_font), ALLEGRO_ALIGN_CENTRE,
    "Testando Taxa de FPS");
}