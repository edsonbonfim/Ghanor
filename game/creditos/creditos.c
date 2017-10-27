#include "creditos.h"

void creditos()
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
        [BITMAP_BACKGROUND] load_bitmap_at_size("data/images/bg.png", DISPLAY_W, DISPLAY_H)
    };

    /** ----- Fila de Eventos ----- **/
    
    ALLEGRO_EVENT_QUEUE *queue[]
    =
    {
        [EVENT_KEYBOARD_KEYDOWN] create_event_queue()
    };

    al_register_event_source(queue[EVENT_KEYBOARD_KEYDOWN], al_get_keyboard_event_source());

    /** ----- Variaveis Auxiliares ----- **/

    int escape = 0;

    /** ----- Loop ----- **/

    while(!escape)
    {
        /** ----- Desenhos ----- **/
        
        creditos_draw_background(bitmap);
        creditos_draw_text(font);
        
        al_flip_display();

        /** ----- Eventos ----- ***/

        if (creditos_event_keyboard(queue))
            escape = 1;
    }

    al_destroy_font(font[FONT_ROBOTO]);
    al_destroy_font(font[FONT_GTEK]);
    al_destroy_font(font[FONT_GTEK_TITLE]);

    al_destroy_bitmap(BITMAP_BACKGROUND);

    al_destroy_event_queue(queue[EVENT_KEYBOARD_KEYDOWN]);
}

/** ----- Desenhos ----- **/

void creditos_draw_text(ALLEGRO_FONT **font)
{
    int x = DISPLAY_W / 2;
    int y = DISPLAY_H / 2;

    ALLEGRO_FONT *title   = font[FONT_GTEK_TITLE];
    ALLEGRO_FONT *options = font[FONT_GTEK];
    ALLEGRO_FONT *roboto  = font[FONT_ROBOTO];

    al_draw_text(title, COLOR_GRAY, x, y - 110, ALLEGRO_ALIGN_CENTER, "creditos");
    
    al_draw_text(options, COLOR_GRAY, x, y - 30,  ALLEGRO_ALIGN_CENTER, "alline ribeiro");
    al_draw_text(options, COLOR_GRAY, x, y + 20,  ALLEGRO_ALIGN_CENTER, "beatriz rangel");
    al_draw_text(options, COLOR_GRAY, x, y + 70,  ALLEGRO_ALIGN_CENTER, "edson onildo");
    al_draw_text(options, COLOR_GRAY, x, y + 120, ALLEGRO_ALIGN_CENTER, "isabela carvalho");
    
    al_draw_text(roboto, COLOR_WHITE, x, (DISPLAY_H - (y / 2)) + 50, ALLEGRO_ALIGN_CENTER, "PRESSIONE ESC PARA VOLTAR");
}

void creditos_draw_background(ALLEGRO_BITMAP **bitmap)
{
    int x1 = DISPLAY_W / 4;
    int y1 = DISPLAY_H / 4;
    int x2 = DISPLAY_W - x1;
    int y2 = DISPLAY_H - y1;

    al_draw_bitmap(bitmap[BITMAP_BACKGROUND], 0, 0, 0);
    
    al_draw_filled_rectangle(0, 0, x1 * 4, y1 * 4, al_map_rgba(0, 0, 0, 100));    
    al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgba(0, 0, 0, 200));
    al_draw_filled_rectangle(x1 - 1, y1 - 1, x2 + 1, y2 + 1, al_map_rgba(0, 0, 0, 50));
    al_draw_filled_rectangle(x1 - 2, y1 - 2, x2 + 2, y2 + 2, al_map_rgba(0, 0, 0, 30));
    al_draw_filled_rectangle(x1 - 3, y1 - 3, x2 + 3, y2 + 3, al_map_rgba(0, 0, 0, 10));
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
