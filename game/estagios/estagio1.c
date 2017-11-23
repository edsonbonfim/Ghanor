int contar_linhas(FILE *arquivo)
{
    char c;

    rewind(arquivo);

    int count = 1;

    while (!feof(arquivo))
    {
        c = fgetc(arquivo);

        if (c == '\n')
            count++;
    }

    rewind(arquivo);

    return count;
}

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

void casoNao(ALLEGRO_DISPLAY *display, int *option);
void casoFale(ALLEGRO_DISPLAY *display, int *option);

void estagio1(ALLEGRO_DISPLAY *display, int *option)
{
    ALLEGRO_FONT *font[]
    =
    {
        [FONT_GTEK] load_font("data/fonts/Righteous-Regular.ttf", 14, 0),
        [FONT_GTEK_TITLE] load_font("data/fonts/Righteous-Regular.ttf", 20, 0)
    };

    ALLEGRO_EVENT_QUEUE *queue[]
    =
    {
        [EVENT_MOUSE_CLICK] create_event_queue()
    };

    al_register_event_source(queue[EVENT_MOUSE_CLICK], al_get_mouse_event_source());

    int k;

    char nome_do_arquivo[50];

    ALLEGRO_BITMAP *bitmap[1];

    int j = 5;

    for (k = 1; k <= j; k++)
    {
        /** Carrega a imagem de fundo **/
        sprintf(nome_do_arquivo, "data/images/Estagio1/fundo%d.jpg", k);
        bitmap[BITMAP_BACKGROUND] = load_bitmap_at_size(nome_do_arquivo, DISPLAY_W, DISPLAY_H);

        /** Carrega o arquivo **/
        sprintf(nome_do_arquivo, "data/files/Estagio1/parte%d.txt", k);
        FILE *parte1 = fopen(nome_do_arquivo, "r");

        char Lixo;
        char c;
        char l[1000];

        int i, count = contar_linhas(parte1);

        while(1)
        {
            if (k == 1)
            {
                estagio1_draw_background(display, bitmap);
                al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "ESTÁGIO I - FALHA DE SEGURANÇA");
            }

            else
            {
                estagio1_draw_background(display, bitmap);
                al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "DIA 15 DE NOVEMBRO. 19:32h. NÚCLEO CENTRAL");
            }

            rewind(parte1);

            for (i = 0; i < count; i++)
            {
                fscanf(parte1, "%[^\n]", l);
                fscanf(parte1, "%c", &Lixo);
                al_draw_text(font[FONT_GTEK], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 100 + (25 * i), ALLEGRO_ALIGN_CENTER, l);
            }

            al_flip_display();

            while (!al_is_event_queue_empty(queue[EVENT_MOUSE_CLICK]))
            {
                ALLEGRO_EVENT event;

                al_wait_for_event(queue[EVENT_MOUSE_CLICK], &event);

                if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                {
                    if (k == j)
                    {
                        /** Nao **/
                        if (event.mouse.x < DISPLAY_W / 2)
                            casoNao(display, option);

                        /** Sim **/
                        if (event.mouse.x > DISPLAY_W / 2)
                            casoFale(display, option);
                    }

                    else
                        goto done;
                }
            }
        }

        done:

        continue;
    }
}

void casoNao(ALLEGRO_DISPLAY *display, int *option)
{
    ALLEGRO_FONT *font[]
    =
    {
        [FONT_GTEK] load_font("data/fonts/Righteous-Regular.ttf", 14, 0),
        [FONT_GTEK_TITLE] load_font("data/fonts/Righteous-Regular.ttf", 20, 0)
    };

    ALLEGRO_EVENT_QUEUE *queue[]
    =
    {
        [EVENT_MOUSE_CLICK] create_event_queue()
    };

    al_register_event_source(queue[EVENT_MOUSE_CLICK], al_get_mouse_event_source());

    int k;

    char nome_do_arquivo[50];

    ALLEGRO_BITMAP *bitmap[1];

    int j = 3;

    for (k = 1; k <= j; k++)
    {
        /** Carrega a imagem de fundo **/
        bitmap[BITMAP_BACKGROUND] = load_bitmap_at_size("data/images/Estagio1/casoFale1.jpg", DISPLAY_W, DISPLAY_H);

        /** Carrega o arquivo **/
        sprintf(nome_do_arquivo, "data/files/Estagio1/casoNao%d.txt", k);
        FILE *parte1 = fopen(nome_do_arquivo, "r");

         char Lixo;
        char c;
        char l[1000];

        int i, count = contar_linhas(parte1);


        while(1)
        {
            estagio1_draw_background(display, bitmap);
                al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "VC DECIDIU NAO FALAR");

            rewind(parte1);

            for (i = 0; i < count; i++)
            {
                fscanf(parte1, "%[^\n]", l);
                fscanf(parte1, "%c", &Lixo);
                al_draw_text(font[FONT_GTEK], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 100 + (25 * i), ALLEGRO_ALIGN_CENTER, l);
            }

            al_flip_display();

            while (!al_is_event_queue_empty(queue[EVENT_MOUSE_CLICK]))
            {
                ALLEGRO_EVENT event;

                al_wait_for_event(queue[EVENT_MOUSE_CLICK], &event);

                if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                {
                    if (k == j)
                    {
                        /** Nao **/
                        if (event.mouse.x < DISPLAY_W / 2)
                            casoNao(display, option);

                        /** Sim **/
                        //if (event.mouse.x > DISPLAY_W / 2)
                            //casoFale(dis);
                    }

                    else
                        goto done;
                }
            }
        }

        done:

        continue;
    }
}


void casoFale(ALLEGRO_DISPLAY *display, int *option)
{
    ALLEGRO_FONT *font[]
    =
    {
        [FONT_GTEK] load_font("data/fonts/Righteous-Regular.ttf", 14, 0),
        [FONT_GTEK_TITLE] load_font("data/fonts/Righteous-Regular.ttf", 20, 0)
    };

    ALLEGRO_EVENT_QUEUE *queue[]
    =
    {
        [EVENT_MOUSE_CLICK] create_event_queue()
    };

    al_register_event_source(queue[EVENT_MOUSE_CLICK], al_get_mouse_event_source());

    int k;

    char nome_do_arquivo[50];

    ALLEGRO_BITMAP *bitmap[1];

    int j = 2;

    for (k = 1; k <= j; k++)
    {
        bitmap[BITMAP_BACKGROUND] = load_bitmap_at_size("data/images/Estagio1/casoFale1.jpg", DISPLAY_W, DISPLAY_H);

        /** Carrega o arquivo **/
        sprintf(nome_do_arquivo, "data/files/Estagio1/casoFale%d.txt", k);
        FILE *parte1 = fopen(nome_do_arquivo, "r");

         char Lixo;
        char c;
        char l[1000];

        int i, count = contar_linhas(parte1);


        while(1)
        {
            estagio1_draw_background(display, bitmap);
                al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "VC DECIDIU FALAR");

            rewind(parte1);

            for (i = 0; i < count; i++)
            {
                fscanf(parte1, "%[^\n]", l);
                fscanf(parte1, "%c", &Lixo);
                al_draw_text(font[FONT_GTEK], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 100 + (25 * i), ALLEGRO_ALIGN_CENTER, l);
            }

            al_flip_display();

            while (!al_is_event_queue_empty(queue[EVENT_MOUSE_CLICK]))
            {
                ALLEGRO_EVENT event;

                al_wait_for_event(queue[EVENT_MOUSE_CLICK], &event);

                if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                {
                    if (k == j)
                    {
                        /** Nao **/
                        if (event.mouse.x < DISPLAY_W / 2)
                            casoNao(display, option);

                        /** Sim **/
                        //if (event.mouse.x > DISPLAY_W / 2)
                            //casoFale(dis);
                    }

                    else
                        goto done;
                }
            }
        }

        done:

        continue;
    }
}
