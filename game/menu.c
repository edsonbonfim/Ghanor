#include "menu.h"

int menu()
{
    int opt, qnt;

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

    ALLEGRO_EVENT_QUEUE *queue_hover = NULL;
    
    if (!(queue_hover = al_create_event_queue()))
    {
        al_destroy_font(h1);
        al_destroy_font(font);
        fprintf(stderr, "Falha ao criar a fila de eventos queue_hover.\n");
        return -1;
    }
    
    al_register_event_source(queue_hover, al_get_mouse_event_source());
    al_register_event_source(queue_hover, al_get_keyboard_event_source());

    ALLEGRO_EVENT_QUEUE *queue_click = NULL;

    if (!(queue_click = al_create_event_queue()))
    {
        al_destroy_font(h1);
        al_destroy_font(font);
        al_destroy_event_queue(queue_hover);
        fprintf(stderr, "Falha ao criar a fila de eventos queue_click.\n");
        return -1;
    }

    al_register_event_source(queue_click, al_get_mouse_event_source());
    al_register_event_source(queue_click, al_get_keyboard_event_source());

    #if SHOW_VIDEO
        load_video();
    #else
        load_background();
    #endif

    if (!count())
        qnt = 2;
    else
        qnt = 1;

    while (1)
    {
        #if SHOW_VIDEO

            if (redraw && al_event_queue_is_empty(queue_video))
            {
                al_clear_to_color(al_map_rgb(0, 0, 0));

                video_display(video);
                
                draw_menu(h1, font);
                
                menu_mouse_hover(queue_hover, font, &qnt);
                
                if ((opt = menu_mouse_click(queue_click, &qnt)))
                    goto done;
                
                redraw = 0;

                al_flip_display();
            }

            g_event_video();
        
        #else

            al_clear_to_color(al_map_rgb(0, 0, 0));
        
            al_draw_bitmap(bg, 0, 0, 0);

            draw_menu(h1, font);

            menu_mouse_hover(queue_hover, font, &qnt);

            if ((opt = menu_mouse_click(queue_click, &qnt)))
                goto done;

            al_flip_display();

        #endif
    }

    done:

    if (!al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
    {
        fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
        return -1;
    }

    al_destroy_font(h1);
    al_destroy_font(font);
    al_destroy_event_queue(queue_hover);
    al_destroy_event_queue(queue_click);

    #if SHOW_VIDEO
        close_video();
    #endif

    return opt;
}

void draw_menu(ALLEGRO_FONT *h1, ALLEGRO_FONT *font)
{
    int qnt = count();

    ALLEGRO_COLOR color = al_map_rgb(153, 153, 153);

    al_draw_filled_rectangle(0, 0, DISPLAY_W, DISPLAY_H, al_map_rgba(0, 0, 0, 100));

    al_draw_filled_rectangle(DISPLAY_W / 4, DISPLAY_H / 4, DISPLAY_W - (DISPLAY_W / 4), DISPLAY_H - (DISPLAY_H / 4), al_map_rgba(0, 0, 0, 200));
    al_draw_filled_rectangle((DISPLAY_W / 4) - 1, (DISPLAY_H / 4) - 1, (DISPLAY_W - (DISPLAY_W / 4)) + 1, (DISPLAY_H - (DISPLAY_H / 4)) + 1, al_map_rgba(0, 0, 0, 50));
    al_draw_filled_rectangle((DISPLAY_W / 4) - 2, (DISPLAY_H / 4) - 2, (DISPLAY_W - (DISPLAY_W / 4)) + 2, (DISPLAY_H - (DISPLAY_H / 4)) + 2, al_map_rgba(0, 0, 0, 30));
    al_draw_filled_rectangle((DISPLAY_W / 4) - 3, (DISPLAY_H / 4) - 3, (DISPLAY_W - (DISPLAY_W / 4)) + 3, (DISPLAY_H - (DISPLAY_H / 4)) + 3, al_map_rgba(0, 0, 0, 10));

    al_draw_text(h1, color, DISPLAY_W / 2, (DISPLAY_H / 2) - 110, ALLEGRO_ALIGN_CENTER, "without name");

    if (!qnt)
        al_draw_text(font, al_map_rgb(50, 50, 50), DISPLAY_W / 2, (DISPLAY_H / 2) - 30, ALLEGRO_ALIGN_CENTER, "continuar");
    else
        al_draw_text(font, color, DISPLAY_W / 2, (DISPLAY_H / 2) - 30, ALLEGRO_ALIGN_CENTER, "continuar");

    al_draw_text(font, color, DISPLAY_W / 2, (DISPLAY_H / 2) + 20, ALLEGRO_ALIGN_CENTER, "novo jogo");
    al_draw_text(font, color, DISPLAY_W / 2, (DISPLAY_H / 2) + 70, ALLEGRO_ALIGN_CENTER, "creditos");
    al_draw_text(font, color, DISPLAY_W / 2, (DISPLAY_H / 2) + 120, ALLEGRO_ALIGN_CENTER, "sair");
}

int get_menu_mouse_opt(ALLEGRO_EVENT ev)
{
    if ((ev.mouse.x > (DISPLAY_W / 2) - 70 && ev.mouse.x < (DISPLAY_W / 2) + 70) && ev.mouse.y > (DISPLAY_H / 2) - 30 && ev.mouse.y < (DISPLAY_H / 2) - 10)
    {
        if (!count())
            return 0;

        return 1;
    }

    else if ((ev.mouse.x > (DISPLAY_W / 2) - 75 && ev.mouse.x < (DISPLAY_W / 2) + 75) && ev.mouse.y > (DISPLAY_H / 2) + 20 && ev.mouse.y < (DISPLAY_H / 2) + 40)
        return 2;

    else if ((ev.mouse.x > (DISPLAY_W / 2) - 65 && ev.mouse.x < (DISPLAY_W / 2) + 65) && ev.mouse.y > (DISPLAY_H / 2) + 70 && ev.mouse.y < (DISPLAY_H / 2) + 90)
        return 3;

    else if ((ev.mouse.x > (DISPLAY_W / 2) - 30 && ev.mouse.x < (DISPLAY_W / 2) + 30) && ev.mouse.y > (DISPLAY_H / 2) + 120 && ev.mouse.y < (DISPLAY_H / 2) + 140)
        return 4;

    else
        return 0;
}

int get_menu_keyboard_opt(ALLEGRO_EVENT ev, int *qnt)
{
    if (*qnt == 5)
    {
        if (!count())
        {
            *qnt = 2;
            return 2;
        }

        else
        {
            *qnt = 1;
            return 1;
        }
    }

    else if (*qnt == 0 || (*qnt == 1 && !count()))
    {
        *qnt = 4;
        return 4;
    }

    switch (ev.keyboard.keycode)
    {
    case ALLEGRO_KEY_S:
    case ALLEGRO_KEY_D:
    case ALLEGRO_KEY_DOWN:
    case ALLEGRO_KEY_RIGHT:
        (*qnt)++;
        break;

    case ALLEGRO_KEY_W:
    case ALLEGRO_KEY_A:
    case ALLEGRO_KEY_UP:
    case ALLEGRO_KEY_LEFT:
        (*qnt)--;
        break;
    }

    return *qnt;
}

int menu_mouse_hover(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_FONT *font, int *qnt)
{
    while (!al_is_event_queue_empty(queue))
    {
        ALLEGRO_EVENT ev;

        al_wait_for_event(queue, &ev);

        if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
            *qnt = get_menu_mouse_opt(ev);

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
            *qnt = get_menu_keyboard_opt(ev, qnt);
    }

    ALLEGRO_COLOR color = al_map_rgb(255, 255, 255);

    switch (*qnt)
    {
        case 1:
            if (!count())
                break;

            al_draw_text(font, color, (DISPLAY_W / 2) + 1, (DISPLAY_H / 2) - 29, ALLEGRO_ALIGN_CENTER, "continuar");
            goto done;

        case 2:
            al_draw_text(font, color, (DISPLAY_W / 2) + 1, (DISPLAY_H / 2) + 21, ALLEGRO_ALIGN_CENTER, "novo jogo");
            goto done;

        case 3:
            al_draw_text(font, color, (DISPLAY_W / 2) + 1, (DISPLAY_H / 2) + 71, ALLEGRO_ALIGN_CENTER, "creditos");
            goto done;

        case 4:
            al_draw_text(font, color, (DISPLAY_W / 2) + 1, (DISPLAY_H / 2) + 121, ALLEGRO_ALIGN_CENTER, "sair");
            goto done;

        default:
            if (!al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
            {
                fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
                return -1;
            }
    }

    return 0;

    done:
    
    if (!al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT))
    {
        fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
        return -1;
    }

    return 0;
}

int menu_mouse_click(ALLEGRO_EVENT_QUEUE *queue, int *qnt)
{
    while (!al_is_event_queue_empty(queue))
    {
        ALLEGRO_EVENT ev;

        al_wait_for_event(queue, &ev);

        if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
            return get_menu_mouse_opt(ev);

        else if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER || ev.keyboard.keycode == ALLEGRO_KEY_SPACE)
            return get_menu_keyboard_opt(ev, qnt);
    }

    return 0;
}