#include "select_raca.h"

int select_raca()
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

    ALLEGRO_BITMAP *bitmap_humano = al_load_bitmap("data/images/humano.png");
    ALLEGRO_BITMAP *bitmap_sintozoide = al_load_bitmap("data/images/sintozoide.png");

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

    int opt;

    struct print_text text_humano;
    
    strcpy(text_humano.text1, "humano");
    strcpy(text_humano.text2, "ENCONTRE TODAS AS PISTAS QUE POSSAM");
    strcpy(text_humano.text3, "LEVAR A REBELIAO HUMANA A LOCALIZACAO");
    strcpy(text_humano.text4, "DO SERVIDOR CENTRAL E DESTRUA-O");

    struct print_text text_sintozoide;

    strcpy(text_sintozoide.text1, "sintozoide");
    strcpy(text_sintozoide.text2, "ENCONTRE E DESTRUA TODAS AS PISTAS");
    strcpy(text_sintozoide.text3, "QUE POSSAM LEVAR A REBELIAO HUMANA");
    strcpy(text_sintozoide.text4, "A LOCALIZACAO DO SERVIDOR CENTRAL");

    int hover_humano;
    int hover_sintozoide;
    
    while (1)
    {
        #if SHOW_VIDEO

            if (redraw && al_event_queue_is_empty(queue_video))
            {
                al_clear_to_color(al_map_rgb(0, 0, 0));

                video_display(video);

                al_draw_filled_rectangle(0, 0, DISPLAY_W, DISPLAY_H, al_map_rgba(0, 0, 0, 100));

                select_raca_draw_box_humano(font, roboto, bitmap_humano, text_humano, &hover_humano);
                select_raca_draw_box_sintozoide(font, roboto, bitmap_sintozoide, text_sintozoide, &hover_sintozoide);
                
                if (layout_menu_get_event_keyboard(queue_keyboard))
                {
                    opt = 0;
                    goto done;
                }
                
                if ((opt = layout_menu_get_event_mouse(queue_mouse, &hover_humano, &hover_sintozoide)) != 0)
                    goto done;

                redraw = 0;
                
                al_flip_display();
            }

            g_event_video();

        #else

            al_clear_to_color(al_map_rgb(0, 0, 0));

            al_draw_bitmap(bg, 0, 0, 0);

            al_draw_filled_rectangle(0, 0, DISPLAY_W, DISPLAY_H, al_map_rgba(0, 0, 0, 100));
            
            select_raca_draw_box_humano(font, roboto, bitmap_humano);
            select_raca_draw_box_sintozoide(font, roboto, bitmap_sintozoide);
            
            if (select_raca_get_event_keyboard(queue_keyboard))
                goto done;
            
            if ((opt = select_raca_get_event_mouse(queue_mouse, &hover_humano, &hover_sintozoide)) != 0)
                goto done;

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

    return opt;
}

void select_raca_draw_box_humano(ALLEGRO_FONT *gtek, ALLEGRO_FONT *roboto, ALLEGRO_BITMAP *bitmap_humano, struct print_text text, int *hover)
{
    ALLEGRO_COLOR color_bg;
    ALLEGRO_COLOR color_font;
    ALLEGRO_COLOR color_bitmap;

    if (!*hover)
    {
        color_bg     = al_map_rgba(0, 0, 0, 200);
        color_font   = al_map_rgb(153, 153, 153);
        color_bitmap = al_map_rgb(100, 100, 100);
    }

    else
    {
        color_bg     = al_map_rgba(50, 0, 0, 200);
        color_font   = al_map_rgb(255, 255, 255);
        color_bitmap = al_map_rgb(255, 255, 255);
    }

    layout_menu_draw_box(DISPLAY_W / 6, DISPLAY_H / 5, DISPLAY_W / 2.1, DISPLAY_H - (DISPLAY_H / 5), color_bg);
    layout_menu_draw_text(DISPLAY_W / 3.1, DISPLAY_H / 5, DISPLAY_H - (DISPLAY_H / 5), gtek, roboto, color_font, text);

    al_draw_tinted_bitmap(bitmap_humano, color_bitmap, (DISPLAY_W / 3.1) - (al_get_bitmap_width(bitmap_humano) / 2), ((DISPLAY_H - (DISPLAY_H / 5)) - 250) - (al_get_bitmap_height(bitmap_humano) / 2), 0);
}

void select_raca_draw_box_sintozoide(ALLEGRO_FONT *gtek, ALLEGRO_FONT *roboto, ALLEGRO_BITMAP *bitmap_sintozoide, struct print_text text, int *hover)
{
    ALLEGRO_COLOR color_bg;
    ALLEGRO_COLOR color_font;
    ALLEGRO_COLOR color_bitmap;

    if (!*hover)
    {
        color_bg     = al_map_rgba(0, 0, 0, 200);
        color_font   = al_map_rgb(153, 153, 153);
        color_bitmap = al_map_rgb(100, 100, 100);
    }

    else
    {
        color_bg     = al_map_rgba(0, 0, 50, 200);
        color_font   = al_map_rgb(255, 255, 255);
        color_bitmap = al_map_rgb(255, 255, 255);
    }

    layout_menu_draw_box(DISPLAY_W / 1.9, DISPLAY_H / 5, DISPLAY_W - (DISPLAY_W / 6), DISPLAY_H - (DISPLAY_H / 5), color_bg);
    layout_menu_draw_text(DISPLAY_W - (DISPLAY_W / 3.1), DISPLAY_H / 5, DISPLAY_H - (DISPLAY_H / 5), gtek, roboto, color_font, text);

    al_draw_tinted_bitmap(bitmap_sintozoide, color_bitmap, DISPLAY_W - (DISPLAY_W / 3.1) - (al_get_bitmap_width(bitmap_sintozoide) / 2), ((DISPLAY_H - (DISPLAY_H / 5)) - 250) - (al_get_bitmap_height(bitmap_sintozoide) / 2), 0);
}