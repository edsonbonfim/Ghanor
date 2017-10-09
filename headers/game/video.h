bool show_video;
bool redraw;



void run_video(VIDEO *video, TIMER *timer_video, EVENT_QUEUE *event_queue_video, EVENT ev_video, PERSONAGEM * personagem)
{
    if (redraw && al_event_queue_is_empty(event_queue_video))
    {
        video_display(video);

        if (show_menu)
        {
            menu_draw();
            menu_mouse_hover();
            menu_mouse_click();
        }

        if (show_personagens)
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
        }
        
        al_flip_display();
        al_clear_to_color(al_map_rgb(0, 0, 0));
        
        redraw = false;
    }

    al_wait_for_event(event_queue_video, &ev_video);

    switch (ev_video.type)
    {
    case ALLEGRO_EVENT_TIMER:
        if (!false)
        {
            redraw = true;
        }
        break;
    }
}
