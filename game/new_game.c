#include "new_game.h"

int hover_humano;
int hover_sintozoide;

ALLEGRO_COLOR color_humano;
ALLEGRO_COLOR color_sintozoide;

int new_game()
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

    ALLEGRO_EVENT_QUEUE *queue_mouse = NULL;
    
    if (!(queue_mouse = al_create_event_queue()))
    {
        al_destroy_font(h1);
        al_destroy_font(font);
        al_destroy_font(roboto);
        fprintf(stderr, "Falha ao criar a fila de eventos queue_mouse.\n");
        return -1;
    }
    
    al_register_event_source(queue_mouse, al_get_mouse_event_source());

    ALLEGRO_EVENT_QUEUE *queue_keyboard = NULL;
    
    if (!(queue_keyboard = al_create_event_queue()))
    {
        al_destroy_font(h1);
        al_destroy_font(font);
        al_destroy_font(roboto);
        al_destroy_event_queue(queue_mouse);
        fprintf(stderr, "Falha ao criar a fila de eventos queue_keyboard.\n");
        return -1;
    }
    
    al_register_event_source(queue_keyboard, al_get_keyboard_event_source());

    #if SHOW_VIDEO
        load_video();
    #else
        load_background();
    #endif

    color_humano = al_map_rgb(255, 255, 255);
    color_sintozoide = al_map_rgb(255, 255, 255);

    while (1)
    {
        #if SHOW_VIDEO

            if (redraw && al_event_queue_is_empty(queue_video))
            {
                al_clear_to_color(al_map_rgb(0, 0, 0));

                video_display(video);

                al_draw_filled_rectangle(0, 0, DISPLAY_W, DISPLAY_H, al_map_rgba(0, 0, 0, 100));

                new_game_draw_box_humano(font);
                new_game_draw_box_sintozoide(font, roboto);
                
                if (new_game_get_event_keyboard(queue_keyboard))
                    goto done;
                
                new_game_get_event_mouse(queue_mouse);

                redraw = 0;
                
                al_flip_display();
            }

            g_event_video();

        #else

            al_clear_to_color(al_map_rgb(0, 0, 0));

            al_draw_bitmap(bg, 0, 0, 0);

            al_draw_filled_rectangle(0, 0, DISPLAY_W, DISPLAY_H, al_map_rgba(0, 0, 0, 100));
            
            new_game_draw_box_humano(font);
            new_game_draw_box_sintozoide(font, roboto);
            
            if (new_game_get_event_keyboard(queue_keyboard))
                goto done;
            
            new_game_get_event_mouse(queue_mouse);

            al_flip_display();

        #endif
    }

    done:

    al_destroy_font(h1);
    al_destroy_font(font);
    al_destroy_font(roboto);
    al_destroy_event_queue(queue_mouse);
    al_destroy_event_queue(queue_keyboard);

    #if SHOW_VIDEO
        close_video();
    #endif

    return 0;
}

void new_game_draw_box_humano(ALLEGRO_FONT *font)
{
    ALLEGRO_COLOR color = al_map_rgb(153, 153, 153);

    if (hover_humano)
    {
        al_draw_filled_rectangle(DISPLAY_W / 6, DISPLAY_H / 5, DISPLAY_W / 2.1, DISPLAY_H - (DISPLAY_H / 5), al_map_rgba(50, 0, 0, 200));
        al_draw_filled_rectangle((DISPLAY_W / 6) - 1, (DISPLAY_H / 5) - 1, (DISPLAY_W / 2.1) + 1, (DISPLAY_H - (DISPLAY_H / 5)) + 1, al_map_rgba(0, 0, 0, 50));
        al_draw_filled_rectangle((DISPLAY_W / 6) - 2, (DISPLAY_H / 5) - 2, (DISPLAY_W / 2.1) + 2, (DISPLAY_H - (DISPLAY_H / 5)) + 2, al_map_rgba(0, 0, 0, 30));
        al_draw_filled_rectangle((DISPLAY_W / 6) - 3, (DISPLAY_H / 5) - 3, (DISPLAY_W / 2.1) + 3, (DISPLAY_H - (DISPLAY_H / 5)) + 3, al_map_rgba(0, 0, 0, 10));
    }

    else
    {
        al_draw_filled_rectangle(DISPLAY_W / 6, DISPLAY_H / 5, DISPLAY_W / 2.1, DISPLAY_H - (DISPLAY_H / 5), al_map_rgba(0, 0, 0, 200));
    }

    al_draw_text(font, color, DISPLAY_W / 3.1, (DISPLAY_H / 5) + 50, ALLEGRO_ALIGN_CENTER, "humanos");
}

void new_game_draw_box_sintozoide(ALLEGRO_FONT *font, ALLEGRO_FONT *roboto)
{
    ALLEGRO_COLOR color = al_map_rgb(153, 153, 153);
    
    if (hover_sintozoide)
    {
        al_draw_filled_rectangle(DISPLAY_W / 1.9, DISPLAY_H / 5, DISPLAY_W - (DISPLAY_W / 6), DISPLAY_H - (DISPLAY_H / 5), al_map_rgba(0, 0, 50, 200));
        al_draw_filled_rectangle((DISPLAY_W / 1.9) - 1, (DISPLAY_H / 5) - 1, (DISPLAY_W - (DISPLAY_W / 6)) + 1, (DISPLAY_H - (DISPLAY_H / 5)) + 1, al_map_rgba(0, 0, 0, 50));
        al_draw_filled_rectangle((DISPLAY_W / 1.9) - 3, (DISPLAY_H / 5) - 2, (DISPLAY_W - (DISPLAY_W / 6)) + 2, (DISPLAY_H - (DISPLAY_H / 5)) + 2, al_map_rgba(0, 0, 0, 30));
        al_draw_filled_rectangle((DISPLAY_W / 1.9) - 5, (DISPLAY_H / 5) - 3, (DISPLAY_W - (DISPLAY_W / 6)) + 3, (DISPLAY_H - (DISPLAY_H / 5)) + 3, al_map_rgba(0, 0, 0, 10));
    }

    else
    {
        al_draw_filled_rectangle(DISPLAY_W / 1.9, DISPLAY_H / 5, DISPLAY_W - (DISPLAY_W / 6), DISPLAY_H - (DISPLAY_H / 5), al_map_rgba(0, 0, 0, 200));
    }

    al_draw_text(font, color, DISPLAY_W - (DISPLAY_W / 3.1), (DISPLAY_H / 5) + 50, ALLEGRO_ALIGN_CENTER, "sintozoides");
    al_draw_text(roboto, color, DISPLAY_W - (DISPLAY_W / 3.1), (DISPLAY_H - (DISPLAY_H / 5)) - 110, ALLEGRO_ALIGN_CENTER, "ENCONTRE E DESTRUA TODAS AS PISTAS");
    al_draw_text(roboto, color, DISPLAY_W - (DISPLAY_W / 3.1), (DISPLAY_H - (DISPLAY_H / 5)) - 80, ALLEGRO_ALIGN_CENTER, "QUE POSSAM LEVAR A REBELIAO HUMANA");
    al_draw_text(roboto, color, DISPLAY_W - (DISPLAY_W / 3.1), (DISPLAY_H - (DISPLAY_H / 5)) - 50, ALLEGRO_ALIGN_CENTER, "A LOCALIZACAO DO SERVIDOR CENTRAL");
}

int new_game_get_event_keyboard(ALLEGRO_EVENT_QUEUE *queue_keyboard)
{
    while (!al_event_queue_is_empty(queue_keyboard))
    {
        ALLEGRO_EVENT ev;

        al_wait_for_event(queue_keyboard, &ev);

        if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
            return 1;
    }

    return 0;
}

void new_game_event_mouse_axes(ALLEGRO_EVENT ev)
{
    if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
    {
        if (ev.mouse.x > DISPLAY_W / 6 && ev.mouse.x < DISPLAY_W / 2.1 && ev.mouse.y > DISPLAY_H / 5 && ev.mouse.y < DISPLAY_H - (DISPLAY_H / 5))
        {
            hover_humano = 1;
            color_humano = al_map_rgb(255, 255, 255);
            al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
        }

        else if (ev.mouse.x > DISPLAY_W / 1.9 && ev.mouse.x < DISPLAY_W - (DISPLAY_W / 6) && ev.mouse.y > DISPLAY_H / 5 && ev.mouse.y < DISPLAY_H - (DISPLAY_H / 5))
        {
            hover_sintozoide = 1;
            color_sintozoide = al_map_rgb(255, 255, 255);
            al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
        }

        else
        {
            hover_humano = 0;
            hover_sintozoide = 0;
            color_humano = al_map_rgb(200, 200, 200);
            color_sintozoide = al_map_rgb(200, 200, 200);
            al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
        }
    }
}

void new_game_event_mouse_button_up(ALLEGRO_EVENT ev)
{
    if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
    {
        if (ev.mouse.x > DISPLAY_W / 6 && ev.mouse.x < DISPLAY_W / 2.1 && ev.mouse.y > DISPLAY_H / 5 && ev.mouse.y < DISPLAY_H - (DISPLAY_H / 5))
        {
            //
        }

        else if (ev.mouse.x > DISPLAY_W / 1.9 && ev.mouse.x < DISPLAY_W - (DISPLAY_W / 6) && ev.mouse.y > DISPLAY_H / 5 && ev.mouse.y < DISPLAY_H - (DISPLAY_H / 5))
        {
            //
        }
    }
}

void new_game_get_event_mouse(ALLEGRO_EVENT_QUEUE *queue_mouse)
{
    while (!al_event_queue_is_empty(queue_mouse))
    {
        ALLEGRO_EVENT ev;

        al_wait_for_event(queue_mouse, &ev);

        new_game_event_mouse_axes(ev);
        new_game_event_mouse_button_up(ev);
    }
}