#include "select_classe_sintozoide.h"

int select_classe_sintozoide()
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

    ALLEGRO_BITMAP *bitmap_atirador = al_load_bitmap("data/images/humano.png");
    ALLEGRO_BITMAP *bitmap_operario = al_load_bitmap("data/images/sintozoide.png");

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

    struct print_text text_atirador;
    
    strcpy(text_atirador.text1, "atirador");
    strcpy(text_atirador.text2, "DESCRICAO ATIRADOR LINHA 1");
    strcpy(text_atirador.text3, "DESCRICAO ATIRADOR LINHA 2");
    strcpy(text_atirador.text4, "DESCRICAO ATIRADOR LINHA 3");

    struct print_text text_operario;

    strcpy(text_operario.text1, "operario");
    strcpy(text_operario.text2, "DESCRICAO OPERARIO LINHA 1");
    strcpy(text_operario.text3, "DESCRICAO OPERARIO LINHA 2");
    strcpy(text_operario.text4, "DESCRICAO OPERARIO LINHA 3");

    int hover_atirador;
    int hover_operario;
    
    while (1)
    {
        #if SHOW_VIDEO

            if (redraw && al_event_queue_is_empty(queue_video))
            {
                al_clear_to_color(al_map_rgb(0, 0, 0));

                video_display(video);

                al_draw_filled_rectangle(0, 0, DISPLAY_W, DISPLAY_H, al_map_rgba(0, 0, 0, 100));

                select_classe_sintozoide_draw_box_atirador(font, roboto, bitmap_atirador, text_atirador, &hover_atirador);
                select_classe_sintozoide_draw_box_operario(font, roboto, bitmap_operario, text_operario, &hover_operario);
                
                if (layout_menu_get_event_keyboard(queue_keyboard))
                {
                    opt = 0;
                    goto done;
                }
                
                if ((opt = layout_menu_get_event_mouse(queue_mouse, &hover_atirador, &hover_operario)) != 0)
                    goto done;

                redraw = 0;
                
                al_flip_display();
            }

            g_event_video();

        #else

            al_clear_to_color(al_map_rgb(0, 0, 0));

            al_draw_bitmap(bg, 0, 0, 0);

            al_draw_filled_rectangle(0, 0, DISPLAY_W, DISPLAY_H, al_map_rgba(0, 0, 0, 100));
            
            select_classe_sintozoide_draw_box_atirador(font, roboto, bitmap_atirador);
            select_classe_sintozoide_draw_box_operario(font, roboto, bitmap_operario);
            
            if (select_classe_sintozoide_get_event_keyboard(queue_keyboard))
                goto done;
            
            if ((opt = select_classe_sintozoide_get_event_mouse(queue_mouse, &hover_atirador, &hover_operario)) != 0)
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

void select_classe_sintozoide_draw_box_atirador(ALLEGRO_FONT *gtek, ALLEGRO_FONT *roboto, ALLEGRO_BITMAP *bitmap_atirador, struct print_text text, int *hover)
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

    //al_draw_tinted_bitmap(bitmap_atirador, color_bitmap, (DISPLAY_W / 3.1) - (al_get_bitmap_width(bitmap_atirador) / 2), ((DISPLAY_H - (DISPLAY_H / 5)) - 250) - (al_get_bitmap_height(bitmap_atirador) / 2), 0);
}

void select_classe_sintozoide_draw_box_operario(ALLEGRO_FONT *gtek, ALLEGRO_FONT *roboto, ALLEGRO_BITMAP *bitmap_operario, struct print_text text, int *hover)
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

    //al_draw_tinted_bitmap(bitmap_operario, color_bitmap, DISPLAY_W - (DISPLAY_W / 3.1) - (al_get_bitmap_width(bitmap_operario) / 2), ((DISPLAY_H - (DISPLAY_H / 5)) - 250) - (al_get_bitmap_height(bitmap_operario) / 2), 0);
}