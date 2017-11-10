#include "creditos.h"

void creditos(ALLEGRO_DISPLAY *display, int *option)
{
    /** ----- Fontes ----- **/
    
    ALLEGRO_FONT *font[]
    =
    {
        [FONT_ROBOTO]     load_font("data/fonts/roboto-regular.ttf",  12, 0),
        [FONT_GTEK]       load_font("data/fonts/gtek-technology.ttf", 20, 0),
        [FONT_GTEK_TITLE] load_font("data/fonts/gtek-technology.ttf", 30, 0)
    };

    /** ----- Bitmaps ----- **/
    
    ALLEGRO_BITMAP *bitmap[]
    =
    {
        [BITMAP_BACKGROUND] load_bitmap_at_size("data/images/bg.jpeg", DISPLAY_W, DISPLAY_H)
    };

    /** ----- Fila de Eventos ----- **/
    
    ALLEGRO_EVENT_QUEUE *queue[]
    =
    {
        [EVENT_KEYBOARD_KEYDOWN] create_event_queue()
    };

    al_register_event_source(queue[EVENT_KEYBOARD_KEYDOWN], al_get_keyboard_event_source());

    /** ----- Loop ----- **/

    while(1)
    {
        /** ----- Desenhos ----- **/
        
        creditos_draw_background(display, bitmap);
        creditos_draw_text(display, font);
        
        al_flip_display();

        /** ----- Eventos ----- ***/

        event_display_resize();

        if (creditos_event_keyboard(queue)
        ||  event_display_close(option))
            break;
    }

    al_destroy_font(font[FONT_ROBOTO]);
    al_destroy_font(font[FONT_GTEK]);
    al_destroy_font(font[FONT_GTEK_TITLE]);

    al_destroy_bitmap(BITMAP_BACKGROUND);

    al_destroy_event_queue(queue[EVENT_KEYBOARD_KEYDOWN]);
}

/** ----- Desenhos ----- **/

void creditos_draw_text(ALLEGRO_DISPLAY *display, ALLEGRO_FONT **font)
{
    int x = al_get_display_width(display)  / 2;
    int y = al_get_display_height(display) / 2;

    ALLEGRO_FONT *title   = font[FONT_GTEK_TITLE];
    ALLEGRO_FONT *options = font[FONT_GTEK];
    ALLEGRO_FONT *roboto  = font[FONT_ROBOTO];

    al_draw_text(title,   COLOR_GRAY, x, y - 120, ALLEGRO_ALIGN_CENTER, "creditos");
    al_draw_text(options, COLOR_GRAY, x, y - 40,  ALLEGRO_ALIGN_CENTER, "alline ribeiro");
    al_draw_text(options, COLOR_GRAY, x, y + 10,  ALLEGRO_ALIGN_CENTER, "beatriz rangel");
    al_draw_text(options, COLOR_GRAY, x, y + 60,  ALLEGRO_ALIGN_CENTER, "edson onildo");
    al_draw_text(options, COLOR_GRAY, x, y + 110, ALLEGRO_ALIGN_CENTER, "isabela carvalho");
    
    al_draw_text(roboto, COLOR_WHITE, x, (al_get_display_height(display) - (y / 2)) + 50, ALLEGRO_ALIGN_CENTER, "PRESSIONE ESC PARA VOLTAR");
}

void creditos_draw_background(ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP **bitmap)
{
    int display_width  = al_get_display_width(display);
    int display_height = al_get_display_height(display); 

    int x1 = display_width  / 4;
    int y1 = display_height / 4;
    int x2 = display_width  - x1;
    int y2 = display_height - y1;

    al_draw_tinted_bitmap(bitmap[BITMAP_BACKGROUND], al_map_rgb(127.5, 127.5, 127.5), 0, 0, 0);
    al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgba(0, 0, 0, 170));
}

/** ----- Eventos ----- **/

int creditos_event_keyboard(ALLEGRO_EVENT_QUEUE **queue)
{
    while (!al_event_queue_is_empty(queue[EVENT_KEYBOARD_KEYDOWN]))
    {
        ALLEGRO_EVENT event;

        al_wait_for_event(queue[EVENT_KEYBOARD_KEYDOWN], &event);

        if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            return 1;
    }

    return 0;
}
