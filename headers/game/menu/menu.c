#include "menu.h"

void g_menu(const int FPS, bool show_menu, bool exit_game)
{
    int init_timer;

    int redraw = 1;
    
    PERSONAGEM * personagem = (PERSONAGEM *) malloc(sizeof(PERSONAGEM));
    init_personagem(personagem);

    COLOR color = al_map_rgb(255, 255, 255);
    COLOR hover = al_map_rgb(0, 0, 0);

    MENU  * menu  = (MENU*) malloc(sizeof(MENU));
    FONT  * font  = load_ttf_font("fonts/Bethanie-Snake.ttf", 40, 0);
    VIDEO * video = al_open_video("video.ogv");
    TIMER * timer = al_create_timer(1.0 / FPS);
    
    EVENT event_video;
    
    QUEUE * queue_video = create_event_queue();
    al_register_event_source(queue_video, al_get_video_event_source(video));
    al_register_event_source(queue_video, al_get_timer_event_source(timer));

    QUEUE * queue_hover = create_event_queue();
    al_register_event_source(queue_hover, al_get_mouse_event_source());

    al_start_video(video, al_get_default_mixer());
    al_start_timer(timer);

    while (show_menu)
    {
        init_timer = al_get_time();

        if (redraw && al_event_queue_is_empty(queue_video))
        {
            video_display(video);
    
            if (true)
            {
                draw_menu(font, color);
                menu_mouse_hover(menu, queue_hover, font, hover);
                //menu_mouse_click();
            }
    
            /**if (show_personagens)
            {
                personagens();
                personagens_keyboard_char(personagem);
            }
    
            if (show_history)
                history_draw();
    
            frame++;
    
            if ((al_get_time() - init_timer) < 1.0 / FPS)
            {
                al_rest((1.0 / FPS) - (al_get_time() - init_timer));
            }*/
            
            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
            
            redraw = false;
        }
    
        al_wait_for_event(queue_video, &event_video);
    
        switch (event_video.type)
        {
            case ALLEGRO_EVENT_TIMER:
                if (!false)
                    redraw = true;
                break;
        }
    }

    free(menu);
}