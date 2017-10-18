int history()
{
    int opt;

    ALLEGRO_FONT *h1 = NULL;

    if (!(h1 = al_load_ttf_font("data/fonts/gtek-technology.ttf", 30, 0)))
    {
        fprintf(stderr, "Falha ao carregar a fonte gtek.\n");
        return -1;
    }

    ALLEGRO_FONT *font = NULL;

    if (!(font = al_load_ttf_font("data/fonts/Roboto-Regular.ttf", 20, 0)))
    {
        al_destroy_font(h1);
        fprintf(stderr, "Falha ao carregar a fonte roboto.\n");
        return -1;
    }

    ALLEGRO_COLOR color = al_map_rgb(255, 255, 255);

    ALLEGRO_EVENT_QUEUE *queue_keyboard;

    if (!(queue_keyboard = al_create_event_queue()))
    {
        al_destroy_font(h1);
        al_destroy_font(font);
        fprintf(stderr, "Falha ao criar a fila de eventos queue_keyboard");
        return -1;
    }

    al_register_event_source(queue_keyboard, al_get_keyboard_event_source());

    while (1)
    {
        al_clear_to_color(al_map_rgb(0, 0, 0));

        al_draw_text(h1, color, DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "historia");
        al_draw_text(font, color, DISPLAY_W / 2, DISPLAY_H / 2 - 15, ALLEGRO_ALIGN_CENTER, "Esta funcao ira contar a historia do RPG, mas por hora, pressione enter para continuar,");
        al_draw_text(font, color, DISPLAY_W / 2, DISPLAY_H / 2 + 15, ALLEGRO_ALIGN_CENTER, "ou pressione backspace para voltar.");

        while (!al_event_queue_is_empty(queue_keyboard))
        {
            ALLEGRO_EVENT ev;

            al_wait_for_event(queue_keyboard, &ev);

            if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
            {
                opt = 1;
                goto done;
            }

            if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
            {
                opt = 0;
                goto done;
            }
        }

        al_flip_display();
    }

    done:

    al_destroy_font(h1);
    al_destroy_font(font);
    al_destroy_event_queue(queue_keyboard);

    return opt;
}