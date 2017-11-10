#include "layout.h"

/** ----- Desenhos ----- **/

void personagem_layout_draw_background(int x1, int y1, int x2, int y2, ALLEGRO_COLOR bgcolor)
{
    x1 = x1 - 20;
    x2 = x2 + 20;
    al_draw_filled_rectangle(x1, y1, x2, y2, bgcolor);
}

void personagem_layout_draw_text(int x, int y1, int y2, ALLEGRO_FONT **font, ALLEGRO_COLOR color, char **text)
{
    al_draw_text(font[FONT_GTEK],   color, x, y1 + 50,  ALLEGRO_ALIGN_CENTER, text[0]);
    al_draw_text(font[FONT_ROBOTO], color, x, y2 - 110, ALLEGRO_ALIGN_CENTER, text[1]);
    al_draw_text(font[FONT_ROBOTO], color, x, y2 - 80,  ALLEGRO_ALIGN_CENTER, text[2]);
    al_draw_text(font[FONT_ROBOTO], color, x, y2 - 50,  ALLEGRO_ALIGN_CENTER, text[3]);
}

/** ----- Eventos ----- **/

int personagem_layout_event_mouse_get_opt(ALLEGRO_EVENT event)
{
    int x1 = DISPLAY_W / 6;
    int y1 = DISPLAY_H / 5;
    int x2 = DISPLAY_W / 2.1;
    int y2 = DISPLAY_H - y1;

    if (get_mouse_position(event, x1, y1, x2, y2))
        return 1;

    x1 = DISPLAY_W / 1.9;
    x2 = DISPLAY_W - DISPLAY_W / 6;

    if (get_mouse_position(event, x1, y1, x2, y2))
        return 2;

    return 0;
}

void personagem_layout_event_mouse_hover(ALLEGRO_EVENT_QUEUE **queue, int *option)
{
    while (!al_is_event_queue_empty(queue[EVENT_MOUSE_HOVER]))
    {
        ALLEGRO_EVENT event;

        al_wait_for_event(queue[EVENT_MOUSE_HOVER], &event);

        if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
            *option = personagem_layout_event_mouse_get_opt(event);
    }
}

int personagem_layout_event_mouse_click(ALLEGRO_EVENT_QUEUE **queue, int *option)
{
    while (!al_is_event_queue_empty(queue[EVENT_MOUSE_CLICK]))
    {
        ALLEGRO_EVENT event;

        al_wait_for_event(queue[EVENT_MOUSE_CLICK], &event);

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
        {
            *option = personagem_layout_event_mouse_get_opt(event);
            return 1;
        }
    }
}
