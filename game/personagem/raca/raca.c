#include "raca.h"

int personagem_raca()
{
    /** ----- Fontes ----- **/

    ALLEGRO_FONT *font[]
    =
    {
        [FONT_GTEK]   load_font("data/fonts/gtek-technology.ttf", 20, 0),
        [FONT_ROBOTO] load_font("data/fonts/roboto-regular.ttf",  12, 0)
    };

    /** ----- Bitmaps ----- **/

    ALLEGRO_BITMAP *bitmap[]
    =
    {
        [BITMAP_HUMANO]     load_bitmap("data/images/humano.png"),
        [BITMAP_SINTOZOIDE] load_bitmap("data/images/sintozoide.png"),
        [BITMAP_BACKGROUND] load_bitmap_at_size("data/images/bg.png", DISPLAY_W, DISPLAY_H)
    };

    /** ----- Fila de Eventos ----- **/

    ALLEGRO_EVENT_QUEUE *queue[]
    =
    {
        [EVENT_MOUSE_HOVER] create_event_queue(),
        [EVENT_MOUSE_CLICK] create_event_queue()
    };

    al_register_event_source(queue[EVENT_MOUSE_HOVER], al_get_mouse_event_source());
    al_register_event_source(queue[EVENT_MOUSE_CLICK], al_get_mouse_event_source());

    /** Variaveis Auxiliares **/

    int option = 0;

    /** Loop **/

    while(1)
    {
        /** ----- Desenhos ----- **/

        personagem_raca_draw_background(bitmap);
        
        personagem_raca_humano_draw_background(bitmap, option);
        personagem_raca_sintozoide_draw_background(bitmap, option);

        personagem_raca_humano_draw_text(font, option);
        personagem_raca_sintozoide_draw_text(font, option);
        
        al_flip_display();

        /** Eventos **/

        personagem_layout_event_mouse_hover(queue, &option);
        
        if (personagem_layout_event_mouse_click(queue, &option))
            break;
    }

    al_destroy_font(font[FONT_GTEK]);
    al_destroy_font(font[FONT_ROBOTO]);

    al_destroy_bitmap(bitmap[BITMAP_HUMANO]);
    al_destroy_bitmap(bitmap[BITMAP_SINTOZOIDE]);
    al_destroy_bitmap(bitmap[BITMAP_BACKGROUND]);

    al_destroy_event_queue(queue[EVENT_MOUSE_HOVER]);

    return option;
}

void personagem_raca_draw_background(ALLEGRO_BITMAP **bitmap)
{
    al_draw_bitmap(bitmap[BITMAP_BACKGROUND], 0, 0, 0);
}

void personagem_raca_humano_draw_background(ALLEGRO_BITMAP **bitmap, int option)
{
    ALLEGRO_COLOR color_bg;
    ALLEGRO_COLOR color_font;
    ALLEGRO_COLOR color_bitmap;

    if (option == 1)
    {
        color_bg     = al_map_rgba(50, 0, 0, 200);
        color_bitmap = al_map_rgb(255, 255, 255);
    }

    else
    {
        color_bg     = al_map_rgba(0, 0, 0, 200);
        color_bitmap = al_map_rgb(100, 100, 100);
    }

    int x1 = DISPLAY_W / 6;
    int y1 = DISPLAY_H / 5;
    int x2 = DISPLAY_W / 2.1;
    int y2 = DISPLAY_H - y1;

    personagem_layout_draw_background(x1, y1, x2, y2, color_bg);

    int bitmap_width  = al_get_bitmap_width(bitmap[BITMAP_HUMANO]);
    int bitmap_height = al_get_bitmap_height(bitmap[BITMAP_HUMANO]); 

    int x = DISPLAY_W / 3.1 - bitmap_width / 2;
    int y = y2 - (bitmap_height / 2) - 250;

    al_draw_tinted_bitmap(bitmap[BITMAP_HUMANO], color_bitmap, x, y, 0);
}

void personagem_raca_humano_draw_text(ALLEGRO_FONT **font, int option)
{
    ALLEGRO_COLOR color = (option == 1) ? COLOR_WHITE : COLOR_GRAY;
    
    int x  = DISPLAY_W / 3.1;
    int y1 = DISPLAY_H / 5;
    int y2 = DISPLAY_H - y1;

    char *text[]
    =
    {
        "humano",
        "DESCRICAO HUMANO LINHA 1",
        "DESCRICAO HUMANO LINHA 2",
        "DESCRICAO HUMANO LINHA 3"
    };

    personagem_layout_draw_text(x, y1, y2, font, color, text);
}

void personagem_raca_sintozoide_draw_background(ALLEGRO_BITMAP **bitmap, int option)
{
    ALLEGRO_COLOR color_bg;
    ALLEGRO_COLOR color_font;
    ALLEGRO_COLOR color_bitmap;

    if (option == 2)
    {
        color_bg     = al_map_rgba(0, 0, 50, 200);
        color_bitmap = al_map_rgb(255, 255, 255);
    }

    else
    {
        color_bg     = al_map_rgba(0, 0, 0, 200);
        color_bitmap = al_map_rgb(100, 100, 100);
    }

    int x1 = DISPLAY_W / 1.9;
    int y1 = DISPLAY_H / 5;
    int x2 = DISPLAY_W - DISPLAY_W / 6;
    int y2 = DISPLAY_H - y1;

    personagem_layout_draw_background(x1, y1, x2, y2, color_bg);

    int bitmap_width  = al_get_bitmap_width(bitmap[BITMAP_SINTOZOIDE]);
    int bitmap_height = al_get_bitmap_height(bitmap[BITMAP_SINTOZOIDE]); 

    int x = DISPLAY_W - (DISPLAY_W / 3.1) - (bitmap_width / 2);
    int y = y2 - (bitmap_height / 2) - 250;

    al_draw_tinted_bitmap(bitmap[BITMAP_SINTOZOIDE], color_bitmap, x, y, 0);
}

void personagem_raca_sintozoide_draw_text(ALLEGRO_FONT **font, int option)
{
    ALLEGRO_COLOR color = (option == 2) ? COLOR_WHITE : COLOR_GRAY;
    
    int x  = DISPLAY_W - DISPLAY_W / 3.1;
    int y1 = DISPLAY_H / 5;
    int y2 = DISPLAY_H - y1;

    char *text[]
    =
    {
        "sintozoide",
        "DESCRICAO SINTOZOIDE LINHA 1",
        "DESCRICAO SINTOZOIDE LINHA 2",
        "DESCRICAO SINTOZOIDE LINHA 3"
    };

    personagem_layout_draw_text(x, y1, y2, font, color, text);
}
