#include "sintozoide.h"

int personagem_classe_sintozoide()
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
        [BITMAP_ATIRADOR]    load_bitmap("data/images/atirador.png"),
        [BITMAP_OPERARIO]   load_bitmap("data/images/operario.png"),
        [BITMAP_BACKGROUND] load_bitmap_at_size("data/images/bg.jpeg", DISPLAY_W, DISPLAY_H)
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

        personagem_classe_sintozoide_draw_background(bitmap);
        
        personagem_classe_sintozoide_atirador_draw_background(bitmap, option);
        personagem_classe_sintozoide_operario_draw_background(bitmap, option);

        personagem_classe_sintozoide_atirador_draw_text(font, option);
        personagem_classe_sintozoide_operario_draw_text(font, option);
        
        al_flip_display();

        /** Eventos **/

        personagem_layout_event_mouse_hover(queue, &option);
        
        if (personagem_layout_event_mouse_click(queue, &option))
            break;
    }

    al_destroy_font(font[FONT_GTEK]);
    al_destroy_font(font[FONT_ROBOTO]);

    al_destroy_bitmap(bitmap[BITMAP_ATIRADOR]);
    al_destroy_bitmap(bitmap[BITMAP_OPERARIO]);
    al_destroy_bitmap(bitmap[BITMAP_BACKGROUND]);

    al_destroy_event_queue(queue[EVENT_MOUSE_HOVER]);

    return option;
}

void personagem_classe_sintozoide_draw_background(ALLEGRO_BITMAP **bitmap)
{
    al_draw_bitmap(bitmap[BITMAP_BACKGROUND], 0, 0, 0);
}

void personagem_classe_sintozoide_atirador_draw_background(ALLEGRO_BITMAP **bitmap, int option)
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
        color_bitmap = al_map_rgb(150, 150, 150);
    }

    int x1 = DISPLAY_W / 6;
    int y1 = DISPLAY_H / 5;
    int x2 = DISPLAY_W / 2.1;
    int y2 = DISPLAY_H - y1;

    personagem_layout_draw_background(x1, y1, x2, y2, color_bg);

    int bitmap_width  = al_get_bitmap_width(bitmap[BITMAP_ATIRADOR]);
    int bitmap_height = al_get_bitmap_height(bitmap[BITMAP_ATIRADOR]); 

    int x = DISPLAY_W / 3.1 - bitmap_width / 2;
    int y = y2 - (bitmap_height / 2) - 250;

    al_draw_tinted_bitmap(bitmap[BITMAP_ATIRADOR], color_bitmap, x, y, 0);
}

void personagem_classe_sintozoide_atirador_draw_text(ALLEGRO_FONT **font, int option)
{
    ALLEGRO_COLOR color = (option == 1) ? COLOR_WHITE : COLOR_GRAY;
    
    int x  = DISPLAY_W / 3.1;
    int y1 = DISPLAY_H / 5;
    int y2 = DISPLAY_H - y1;

    /**
     * Criados como os militares perfeitos com o intuito de proteger a raça humana. Mas, após a ascensão das IA's, passaram a proteger o interesse de seus semelhantes.
     */

    char *text[]
    =
    {
        "atirador",
        "CRIADOS COMO OS MILITARES PARA PROTEGER",
        "A RACA HUMANA, MAS APOS A ASCENCAO DAS",
        "IA'S, PASSARAM A PROTEGER SEUS SEMELHANTES"
    };

    personagem_layout_draw_text(x, y1, y2, font, color, text);
}

void personagem_classe_sintozoide_operario_draw_background(ALLEGRO_BITMAP **bitmap, int option)
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
        color_bitmap = al_map_rgb(150, 150, 150);
    }

    int x1 = DISPLAY_W / 1.9;
    int y1 = DISPLAY_H / 5;
    int x2 = DISPLAY_W - DISPLAY_W / 6;
    int y2 = DISPLAY_H - y1;

    personagem_layout_draw_background(x1, y1, x2, y2, color_bg);

    int bitmap_width  = al_get_bitmap_width(bitmap[BITMAP_OPERARIO]);
    int bitmap_height = al_get_bitmap_height(bitmap[BITMAP_OPERARIO]); 

    int x = DISPLAY_W - (DISPLAY_W / 3.1) - (bitmap_width / 2);
    int y = y2 - (bitmap_height / 2) - 250;

    al_draw_tinted_bitmap(bitmap[BITMAP_OPERARIO], color_bitmap, x, y, 0);
}

void personagem_classe_sintozoide_operario_draw_text(ALLEGRO_FONT **font, int option)
{
    ALLEGRO_COLOR color = (option == 2) ? COLOR_WHITE : COLOR_GRAY;
    
    int x  = DISPLAY_W - DISPLAY_W / 3.1;
    int y1 = DISPLAY_H / 5;
    int y2 = DISPLAY_H - y1;

    char *text[]
    =
    {
        "operario",
        "ESPECIALISTAS EM MECANICA, SAO OS",
        "UNICOS A CONHECER OS SEGREDOS",
        "DAS TECNOLOGIAS DE GHANOR"
    };

    personagem_layout_draw_text(x, y1, y2, font, color, text);
}
