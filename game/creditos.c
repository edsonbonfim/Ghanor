#include "creditos.h"

int creditos()
{
    ALLEGRO_FONT *h1 = NULL;
    
    if (!(h1 = al_load_ttf_font("data/fonts/gtek-technology.ttf", 30, 0)))
    {
        fprintf(stderr, "Falha ao carregar a font gtek.\n");
        return -1;
    }

    ALLEGRO_FONT *font = NULL;

    if (!(font = al_load_ttf_font("data/fonts/gtek-technology.ttf", 20, 0)))
    {
        al_destroy_font(h1);
        fprintf(stderr, "Falha ao carregar a font gtek.\n");
        return -1;
    }

    ALLEGRO_FONT *roboto = NULL;
    
    if (!(roboto = al_load_ttf_font("data/fonts/Roboto-Regular.ttf", 12, 0)))
    {
        al_destroy_font(h1);
        al_destroy_font(font);
        fprintf(stderr, "Falha ao carregar a font gtek.\n");
        return -1;
    }

    ALLEGRO_EVENT_QUEUE *queue_keyboard = NULL;
    
    if (!(queue_keyboard = al_create_event_queue()))
    {
        al_destroy_font(h1);
        al_destroy_font(font);
        al_destroy_font(roboto);
        fprintf(stderr, "Falha ao criar a fila de eventos queue_keyboard.\n");
        return -1;
    }
    
    al_register_event_source(queue_keyboard, al_get_keyboard_event_source());

    #if SHOW_VIDEO
        load_video();
    #else
        load_background();
    #endif

    while (1)
    {
        #if SHOW_VIDEO

            if (redraw && al_event_queue_is_empty(queue_video))
            {
                al_clear_to_color(al_map_rgb(0, 0, 0));

                video_display(video);

                draw_creditos(h1, font, roboto);

                while (!al_event_queue_is_empty(queue_keyboard))
                {
                    ALLEGRO_EVENT ev;

                    al_wait_for_event(queue_keyboard, &ev);

                    if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
                        goto done;
                }

                redraw = 0;

                al_flip_display();
            }

            g_event_video();

        #else

            al_clear_to_color(al_map_rgb(0, 0, 0));

            al_draw_bitmap(bg, 0, 0, 0);

            draw_creditos(h1, font, roboto);

            while (!al_event_queue_is_empty(queue_keyboard))
            {
                ALLEGRO_EVENT ev;

                al_wait_for_event(queue_keyboard, &ev);

                if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
                    goto done;
            }

            al_flip_display();

        #endif
    }

    done:

    al_destroy_font(h1);
    al_destroy_font(font);
    al_destroy_font(roboto);
    al_destroy_event_queue(queue_keyboard);

    #if SHOW_VIDEO
        close_video();
    #endif

    return 0;
}

void draw_creditos(ALLEGRO_FONT *h1, ALLEGRO_FONT *font, ALLEGRO_FONT *roboto)
{
    ALLEGRO_COLOR color = al_map_rgb(153, 153, 153);

    al_draw_filled_rectangle(0, 0, DISPLAY_W, DISPLAY_H, al_map_rgba(0, 0, 0, 100));

    al_draw_filled_rectangle(DISPLAY_W / 4, DISPLAY_H / 4, DISPLAY_W - (DISPLAY_W / 4), DISPLAY_H - (DISPLAY_H / 4), al_map_rgba(0, 0, 0, 200));
    al_draw_filled_rectangle((DISPLAY_W / 4) - 1, (DISPLAY_H / 4) - 1, (DISPLAY_W - (DISPLAY_W / 4)) + 1, (DISPLAY_H - (DISPLAY_H / 4)) + 1, al_map_rgba(0, 0, 0, 50));
    al_draw_filled_rectangle((DISPLAY_W / 4) - 2, (DISPLAY_H / 4) - 2, (DISPLAY_W - (DISPLAY_W / 4)) + 2, (DISPLAY_H - (DISPLAY_H / 4)) + 2, al_map_rgba(0, 0, 0, 30));
    al_draw_filled_rectangle((DISPLAY_W / 4) - 3, (DISPLAY_H / 4) - 3, (DISPLAY_W - (DISPLAY_W / 4)) + 3, (DISPLAY_H - (DISPLAY_H / 4)) + 3, al_map_rgba(0, 0, 0, 10));

    al_draw_text(h1, color, DISPLAY_W / 2, (DISPLAY_H / 2) - 110, ALLEGRO_ALIGN_CENTER, "creditos");
    al_draw_text(font, color, DISPLAY_W / 2, (DISPLAY_H / 2) - 30, ALLEGRO_ALIGN_CENTER, "alline ribeiro");
    al_draw_text(font, color, DISPLAY_W / 2, (DISPLAY_H / 2) + 20, ALLEGRO_ALIGN_CENTER, "beatriz rangel");
    al_draw_text(font, color, DISPLAY_W / 2, (DISPLAY_H / 2) + 70, ALLEGRO_ALIGN_CENTER, "edson onildo");
    al_draw_text(font, color, DISPLAY_W / 2, (DISPLAY_H / 2) + 120, ALLEGRO_ALIGN_CENTER, "isabela carvalho");
    al_draw_text(roboto, al_map_rgb(255, 255, 255), DISPLAY_W / 2, (DISPLAY_H - (DISPLAY_H / 4)) + 50, ALLEGRO_ALIGN_CENTER, "PRESSIONE BACKSPACE PARA VOLTAR");
}