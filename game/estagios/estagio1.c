void estagio1_draw_background(ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP **bitmap)
{
    int display_width  = al_get_display_width(display);
    int display_height = al_get_display_height(display);

    int x1 = display_width  / 6;
    int y1 = display_height / 6 - 20;
    int x2 = display_width  - x1;
    int y2 = display_height - y1;

    al_draw_bitmap(bitmap[BITMAP_BACKGROUND], 0, 0, 0);
    al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgba(0, 0, 0, 170));
}

void estagio1(ALLEGRO_DISPLAY *display, int *option)
{
    ALLEGRO_BITMAP *bitmap[]
    =
    {
        [BITMAP_BACKGROUND] load_bitmap_at_size("data/images/Estagio1/emergencia.jpg", DISPLAY_W, DISPLAY_H)
    };

    ALLEGRO_FONT *font[]
    =
    {
        [FONT_GTEK] load_font("data/fonts/gtek-technology.ttf", 14, 0),
        [FONT_GTEK_TITLE] load_font("data/fonts/gtek-technology.ttf", 20, 0)
    };

    FILE *parte1 = fopen("data/files/Estagio1/parte1.txt", "r");

    char c;
    char l[1000];

    int i, count = 8;

    while(1)
    {
        estagio1_draw_background(display, bitmap);


        al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "estagio i - falha de seguranca");

        for (i = 0; i < count; i++)
        {
            fscanf(parte1, "%[^\n]", l);
            al_draw_text(font[FONT_GTEK], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 + (25 * i), ALLEGRO_ALIGN_CENTER, l);
        }

        al_flip_display();
    }
}