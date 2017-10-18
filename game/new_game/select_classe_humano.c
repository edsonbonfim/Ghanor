#include "select_classe_humano.h"

int select_classe_humano()
{
    ALLEGRO_FONT *font = NULL;

    if (!(font = al_load_ttf_font("data/fonts/gtek-technology.ttf", 20, 0)))
    {
        fprintf(stderr, "Falha ao carregar a font gtek.\n");
        return -1;
    }

    ALLEGRO_FONT *roboto = NULL;
    
    if (!(roboto = al_load_ttf_font("data/fonts/Roboto-Regular.ttf", 12, 0)))
    {
        al_destroy_font(font);
        fprintf(stderr, "Falha ao carregar a font gtek.\n");
        return -1;
    }

    ALLEGRO_BITMAP *bitmap_soldado = al_load_bitmap("data/images/humano.png");
    ALLEGRO_BITMAP *bitmap_cientista = al_load_bitmap("data/images/sintozoide.png");

    ALLEGRO_EVENT_QUEUE *queue_mouse = NULL;
    
    if (!(queue_mouse = al_create_event_queue()))
    {
        al_destroy_font(font);
        al_destroy_font(roboto);
        fprintf(stderr, "Falha ao criar a fila de eventos queue_mouse.\n");
        return -1;
    }

    al_register_event_source(queue_mouse, al_get_mouse_event_source());

    ALLEGRO_EVENT_QUEUE *queue_keyboard = NULL;
    
    if (!(queue_keyboard = al_create_event_queue()))
    {
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

    struct print_text text_soldado;
    
    strcpy(text_soldado.text1, "soldado");
    strcpy(text_soldado.text2, "DESCRICAO SOLDADO LINHA 1");
    strcpy(text_soldado.text3, "DESCRICAO SOLDADO LINHA 2");
    strcpy(text_soldado.text4, "DESCRICAO SOLDADO LINHA 3");

    struct print_text text_cientista;

    strcpy(text_cientista.text1, "cientista");
    strcpy(text_cientista.text2, "DESCRICAO CIENTISTA LINHA 1");
    strcpy(text_cientista.text3, "DESCRICAO CIENTISTA LINHA 2");
    strcpy(text_cientista.text4, "DESCRICAO CIENTISTA LINHA 3");

    int hover_soldado;
    int hover_cientista;
    
    while (1)
    {
        #if SHOW_VIDEO

            if (redraw && al_event_queue_is_empty(queue_video))
            {
                al_clear_to_color(al_map_rgb(0, 0, 0));

                video_display(video);

                al_draw_filled_rectangle(0, 0, DISPLAY_W, DISPLAY_H, al_map_rgba(0, 0, 0, 100));

                select_classe_humano_draw_box_soldado(font, roboto, bitmap_soldado, text_soldado, &hover_soldado);
                select_classe_humano_draw_box_cientista(font, roboto, bitmap_cientista, text_cientista, &hover_cientista);
                
                if (layout_menu_get_event_keyboard(queue_keyboard))
                {
                    opt = 0;
                    goto done;
                }
                
                if ((opt = layout_menu_get_event_mouse(queue_mouse, &hover_soldado, &hover_cientista)) != 0)
                    goto done;

                redraw = 0;
                
                al_flip_display();
            }

            g_event_video();

        #else

            al_clear_to_color(al_map_rgb(0, 0, 0));

            al_draw_bitmap(bg, 0, 0, 0);

            al_draw_filled_rectangle(0, 0, DISPLAY_W, DISPLAY_H, al_map_rgba(0, 0, 0, 100));
            
            select_classe_humano_draw_box_soldado(font, roboto, bitmap_soldado);
            select_classe_humano_draw_box_cientista(font, roboto, bitmap_cientista);
            
            if (select_classe_humano_get_event_keyboard(queue_keyboard))
                goto done;
            
            if ((opt = select_classe_humano_get_event_mouse(queue_mouse, &hover_soldado, &hover_cientista)) != 0)
                goto done;

            al_flip_display();

        #endif
    }

    done:

    al_destroy_font(font);
    al_destroy_font(roboto);
    al_destroy_event_queue(queue_mouse);
    al_destroy_event_queue(queue_keyboard);

    #if SHOW_VIDEO
        close_video();
    #endif

    return opt;
}

void select_classe_humano_draw_box_soldado(ALLEGRO_FONT *gtek, ALLEGRO_FONT *roboto, ALLEGRO_BITMAP *bitmap, struct print_text text, int *hover)
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

    //al_draw_tinted_bitmap(bitmap_soldado) / 2), ((DISPLAY_H - (DISPLAY_H / 5)) - 250) - (al_get_bitmap_height(bitmap) / 2), 0);
}

void select_classe_humano_draw_box_cientista(ALLEGRO_FONT *gtek, ALLEGRO_FONT *roboto, ALLEGRO_BITMAP *bitmap, struct print_text text, int *hover)
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

    layout_menu_draw_box(DISPLAY_W / 1.9, DISPLAY_H / 5, DISPLAY_W - (DISPLAY_W / 6), DISPLAY_H - (DISPLAY_H / 5), color_bg);
    layout_menu_draw_text(DISPLAY_W - (DISPLAY_W / 3.1), DISPLAY_H / 5, DISPLAY_H - (DISPLAY_H / 5), gtek, roboto, color_font, text);

    //al_draw_tinted_bitmap(bitmap) / 2), ((DISPLAY_H - (DISPLAY_H / 5)) - 250) - (al_get_bitmap_height(bitmap) / 2), 0);
}