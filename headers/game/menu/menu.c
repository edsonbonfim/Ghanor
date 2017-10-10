#include "menu.h"

int menu()
{
    int redraw = 1;

    int   opt;
    int * opt_hover = (int *) malloc(sizeof(int));
    int * opt_click = (int *) malloc(sizeof(int));
   
    EVENT event_video;

    COLOR color = al_map_rgb(153, 153, 153);
    COLOR hover = al_map_rgb(255, 255, 255);

    FONT  * h1    = load_ttf_font("fonts/gtek-technology.ttf", 30, 0);
    FONT  * font  = load_ttf_font("fonts/gtek-technology.ttf", 20, 0);
    VIDEO * video = al_open_video("videos/video.ogv");
    TIMER * timer = al_create_timer(1.0 / FPS);
    
    QUEUE * queue_video = create_event_queue();
    al_register_event_source(queue_video, al_get_video_event_source(video));
    al_register_event_source(queue_video, al_get_timer_event_source(timer));

    QUEUE * queue_hover = create_event_queue();
    al_register_event_source(queue_hover, al_get_mouse_event_source());

    QUEUE * queue_click = create_event_queue();
    al_register_event_source(queue_click, al_get_mouse_event_source());

    al_start_video(video, al_get_default_mixer());
    al_start_timer(timer);

    while (1)
    {
        if (redraw && al_event_queue_is_empty(queue_video))
        {
            video_display(video);
    
            draw_menu(h1, font, color);
            menu_mouse_hover(opt_hover, queue_hover, font, hover);

            if ((opt = menu_mouse_click(opt_click, queue_click)) != -2) goto done;
    
            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
            
            redraw = false;
        }
    
        al_wait_for_event(queue_video, &event_video);
    
        if (event_video.type == ALLEGRO_EVENT_TIMER)
            redraw = true;
    }

    done:

    set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);

    free(opt_hover);
    free(opt_click);

    al_destroy_event_queue(queue_hover);
    al_destroy_event_queue(queue_click);
    al_destroy_event_queue(queue_video);

    return opt;
}