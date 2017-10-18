#include "layout.h"

void layout_menu_draw_box(int x1, int y1, int x2, int y2, ALLEGRO_COLOR bgcolor)
{
    al_draw_filled_rectangle(x1,     y1,     x2,     y2,     bgcolor);
    al_draw_filled_rectangle(x1 - 1, y1 - 1, x2 + 1, y2 + 1, al_map_rgba(0, 0, 0, 50));
    al_draw_filled_rectangle(x1 - 2, y1 - 2, x2 + 2, y2 + 2, al_map_rgba(0, 0, 0, 30));
    al_draw_filled_rectangle(x1 - 3, y1 - 3, x2 + 3, y2 + 3, al_map_rgba(0, 0, 0, 10));
}

void layout_menu_draw_text(int x, int y1, int y2, ALLEGRO_FONT *gtek, ALLEGRO_FONT *roboto, ALLEGRO_COLOR color, struct print_text text)
{
    al_draw_text(gtek,   color, x, y1 + 50,  ALLEGRO_ALIGN_CENTER, text.text1);
    al_draw_text(roboto, color, x, y2 - 110, ALLEGRO_ALIGN_CENTER, text.text2);
    al_draw_text(roboto, color, x, y2 - 80,  ALLEGRO_ALIGN_CENTER, text.text3);
    al_draw_text(roboto, color, x, y2 - 50,  ALLEGRO_ALIGN_CENTER, text.text4);
}

int layout_menu_get_event_keyboard(ALLEGRO_EVENT_QUEUE *queue_keyboard)
{
    while (!al_event_queue_is_empty(queue_keyboard))
    {
        ALLEGRO_EVENT ev;

        al_wait_for_event(queue_keyboard, &ev);

        if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            return 1;
    }

    return 0;
}

int layout_menu_get_event_mouse(ALLEGRO_EVENT_QUEUE *queue_mouse, int *hover_humano, int *hover_sintozoide)
{
    int opt = 0;

    while (!al_event_queue_is_empty(queue_mouse))
    {
        ALLEGRO_EVENT ev;

        al_wait_for_event(queue_mouse, &ev);

        layout_menu_event_mouse_axes(ev, hover_humano, hover_sintozoide);
        
        switch(layout_menu_event_mouse_button_up(ev))
        {
            case 1:
                opt = 1;
                goto done;

            case 2:
                opt = 2;
                goto done;
        }
    }

    done:

    return opt;
}

void layout_menu_event_mouse_axes(ALLEGRO_EVENT ev, int *hover_humano, int *hover_sintozoide)
{
    if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
    {
        if (ev.mouse.x > DISPLAY_W / 6 && ev.mouse.x < DISPLAY_W / 2.1 && ev.mouse.y > DISPLAY_H / 5 && ev.mouse.y < DISPLAY_H - (DISPLAY_H / 5))
        {
            *hover_humano = 1;
            al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
        }

        else if (ev.mouse.x > DISPLAY_W / 1.9 && ev.mouse.x < DISPLAY_W - (DISPLAY_W / 6) && ev.mouse.y > DISPLAY_H / 5 && ev.mouse.y < DISPLAY_H - (DISPLAY_H / 5))
        {
            *hover_sintozoide = 1;
            al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
        }

        else
        {
            *hover_humano = 0;
            *hover_sintozoide = 0;
            al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
        }
    }
}

int layout_menu_event_mouse_button_up(ALLEGRO_EVENT ev)
{
    if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
    {
        /* Raca dos humanos foi selecionada */
        if (ev.mouse.x > DISPLAY_W / 6 && ev.mouse.x < DISPLAY_W / 2.1 && ev.mouse.y > DISPLAY_H / 5 && ev.mouse.y < DISPLAY_H - (DISPLAY_H / 5))
            return 1;

        /* Raca dos sintozoides foi selecionada */
        else if (ev.mouse.x > DISPLAY_W / 1.9 && ev.mouse.x < DISPLAY_W - (DISPLAY_W / 6) && ev.mouse.y > DISPLAY_H / 5 && ev.mouse.y < DISPLAY_H - (DISPLAY_H / 5))
            return 2;
    }

    return 0;
}