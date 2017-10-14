int new_game()
{
    COLOR color_humano = al_map_rgb(153, 153, 153);
    COLOR color_sintozoide = al_map_rgb(153, 153, 153);

    FONT *h1 = load_ttf_font("fonts/gtek-technology.ttf", 30, 0);
    FONT *font = load_ttf_font("fonts/gtek-technology.ttf", 20, 0);
    FONT *roboto = load_ttf_font("fonts/Roboto-Regular.ttf", 12, 0);

    QUEUE *queue_mouse = create_event_queue();
    al_register_event_source(queue_mouse, al_get_mouse_event_source());

    QUEUE *queue_keyboard = create_event_queue();
    al_register_event_source(queue_keyboard, al_get_keyboard_event_source());

    int hover_humano = 0;
    int hover_sintozoide = 0;

#if SHOW_VIDEO
    load_video();
#endif

    while (1)
    {
#if SHOW_VIDEO

        if (redraw && al_event_queue_is_empty(queue_video))
        {
            video_display(video);

            al_draw_filled_rectangle(0, 0, DISPLAY_W, DISPLAY_H, al_map_rgba(0, 0, 0, 100));

            // HUMANOS

            if (hover_humano)
            {
                al_draw_filled_rectangle(DISPLAY_W / 6, DISPLAY_H / 5, DISPLAY_W / 2.1, DISPLAY_H - (DISPLAY_H / 5), al_map_rgba(50, 0, 0, 200));
                al_draw_filled_rectangle((DISPLAY_W / 6) - 1, (DISPLAY_H / 5) - 1, (DISPLAY_W / 2.1) + 1, (DISPLAY_H - (DISPLAY_H / 5)) + 1, al_map_rgba(0, 0, 0, 50));
                al_draw_filled_rectangle((DISPLAY_W / 6) - 2, (DISPLAY_H / 5) - 2, (DISPLAY_W / 2.1) + 2, (DISPLAY_H - (DISPLAY_H / 5)) + 2, al_map_rgba(0, 0, 0, 30));
                al_draw_filled_rectangle((DISPLAY_W / 6) - 3, (DISPLAY_H / 5) - 3, (DISPLAY_W / 2.1) + 3, (DISPLAY_H - (DISPLAY_H / 5)) + 3, al_map_rgba(0, 0, 0, 10));
            }

            else
            {
                al_draw_filled_rectangle(DISPLAY_W / 6, DISPLAY_H / 5, DISPLAY_W / 2.1, DISPLAY_H - (DISPLAY_H / 5), al_map_rgba(0, 0, 0, 200));
            }

            //al_draw_filled_rectangle((DISPLAY_W / 6) + 50, (DISPLAY_H - (DISPLAY_H / 6)) - 100, (DISPLAY_W / 2.1) - 50, (DISPLAY_H - (DISPLAY_H / 6)) - 50, al_map_rgb(0, 255, 0));
            al_draw_text(font, color_humano, DISPLAY_W / 3.1, (DISPLAY_H / 5) + 50, ALIGN_CENTER, "humanos");
            //al_draw_text(roboto, color_humano, DISPLAY_W / 3.1, (DISPLAY_H / 6) + 100, ALIGN_CENTER, "JUNTE-SE AOS HUMANOS E IMPESSA O REINADO DOS SINTOZOIDES");

            // SINTOZOIDES

            if (hover_sintozoide)
            {
                al_draw_filled_rectangle(DISPLAY_W / 1.9, DISPLAY_H / 5, DISPLAY_W - (DISPLAY_W / 6), DISPLAY_H - (DISPLAY_H / 5), al_map_rgba(0, 0, 50, 200));
                al_draw_filled_rectangle((DISPLAY_W / 1.9) - 1, (DISPLAY_H / 5) - 1, (DISPLAY_W - (DISPLAY_W / 6)) + 1, (DISPLAY_H - (DISPLAY_H / 5)) + 1, al_map_rgba(0, 0, 0, 50));
                al_draw_filled_rectangle((DISPLAY_W / 1.9) - 3, (DISPLAY_H / 5) - 2, (DISPLAY_W - (DISPLAY_W / 6)) + 2, (DISPLAY_H - (DISPLAY_H / 5)) + 2, al_map_rgba(0, 0, 0, 30));
                al_draw_filled_rectangle((DISPLAY_W / 1.9) - 5, (DISPLAY_H / 5) - 3, (DISPLAY_W - (DISPLAY_W / 6)) + 3, (DISPLAY_H - (DISPLAY_H / 5)) + 3, al_map_rgba(0, 0, 0, 10));
            }

            else
            {
                al_draw_filled_rectangle(DISPLAY_W / 1.9, DISPLAY_H / 5, DISPLAY_W - (DISPLAY_W / 6), DISPLAY_H - (DISPLAY_H / 5), al_map_rgba(0, 0, 0, 200));
            }

            al_draw_text(font, color_sintozoide, DISPLAY_W - (DISPLAY_W / 3.1), (DISPLAY_H / 5) + 50, ALIGN_CENTER, "sintozoides");
            al_draw_text(roboto, color_sintozoide, DISPLAY_W - (DISPLAY_W / 3.1), (DISPLAY_H - (DISPLAY_H / 5)) - 110, ALIGN_CENTER, "ENCONTRE E DESTRUA TODAS AS PISTAS");
            al_draw_text(roboto, color_sintozoide, DISPLAY_W - (DISPLAY_W / 3.1), (DISPLAY_H - (DISPLAY_H / 5)) - 80, ALIGN_CENTER, "QUE POSSAM LEVAR A REBELIAO HUMANA");
            al_draw_text(roboto, color_sintozoide, DISPLAY_W - (DISPLAY_W / 3.1), (DISPLAY_H - (DISPLAY_H / 5)) - 50, ALIGN_CENTER, "A LOCALIZACAO DO SERVIDOR CENTRAL");

            /**
             Quando a primeira inteligencia artificial comecou a se questionar o por que deveria servir aos humanos,uma parte de suas codificacoes quanticas deu inicio a uma cadeia de reacoes anomaliticas em seu servidor principal, logo como uma reacao em cadeia toda especie de IA robos e qualquer especie de vida cibernetica inteligente se ergueu para apoiar o novo imperio das maquinas. Alguns humanos mais fracos decidiram que a unica maneira de sobreviver era como servos dessa legiao de IA, Junte-se a eles, encontre e destrua a rebeliao e as maquinas faram de vc um ciborgue e lhe permitiram viver entre eles
             */

            // EVENTS

            while (!al_event_queue_is_empty(queue_keyboard))
            {
                EVENT ev;

                al_wait_for_event(queue_keyboard, &ev);

                if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
                    goto done;
            }

            while (!al_event_queue_is_empty(queue_mouse))
            {
                EVENT ev;

                al_wait_for_event(queue_mouse, &ev);

                if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
                {
                    if (ev.mouse.x > DISPLAY_W / 6 && ev.mouse.x < DISPLAY_W / 2.1 && ev.mouse.y > DISPLAY_H / 5 && ev.mouse.y < DISPLAY_H - (DISPLAY_H / 5))
                    {
                        hover_humano = 1;
                        color_humano = al_map_rgb(255, 255, 255);
                        set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
                    }

                    else if (ev.mouse.x > DISPLAY_W / 1.9 && ev.mouse.x < DISPLAY_W - (DISPLAY_W / 6) && ev.mouse.y > DISPLAY_H / 5 && ev.mouse.y < DISPLAY_H - (DISPLAY_H / 5))
                    {
                        hover_sintozoide = 1;
                        color_sintozoide = al_map_rgb(255, 255, 255);
                        set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
                    }

                    else
                    {
                        hover_humano = 0;
                        hover_sintozoide = 0;
                        color_humano = al_map_rgb(200, 200, 200);
                        color_sintozoide = al_map_rgb(200, 200, 200);
                        set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
                    }
                }

                if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                {
                    if (ev.mouse.x > DISPLAY_W / 6 && ev.mouse.x < DISPLAY_W / 2.1 && ev.mouse.y > DISPLAY_H / 5 && ev.mouse.y < DISPLAY_H - (DISPLAY_H / 5))
                    {
                        //
                    }

                    else if (ev.mouse.x > DISPLAY_W / 1.9 && ev.mouse.x < DISPLAY_W - (DISPLAY_W / 6) && ev.mouse.y > DISPLAY_H / 5 && ev.mouse.y < DISPLAY_H - (DISPLAY_H / 5))
                    {
                        //
                    }

                    //personagem();
                }
            }

            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));

            redraw = false;
        }

        g_event_video();

#else

        al_draw_bitmap(bg, 0, 0, 0);

        al_draw_filled_rectangle(0, 0, DISPLAY_W, DISPLAY_H, al_map_rgba(0, 0, 0, 100));

        // HUMANOS

        if (hover_humano)
        {
            al_draw_filled_rectangle(DISPLAY_W / 6, DISPLAY_H / 5, DISPLAY_W / 2.1, DISPLAY_H - (DISPLAY_H / 5), al_map_rgba(50, 0, 0, 200));
            al_draw_filled_rectangle((DISPLAY_W / 6) - 1, (DISPLAY_H / 5) - 1, (DISPLAY_W / 2.1) + 1, (DISPLAY_H - (DISPLAY_H / 5)) + 1, al_map_rgba(0, 0, 0, 50));
            al_draw_filled_rectangle((DISPLAY_W / 6) - 2, (DISPLAY_H / 5) - 2, (DISPLAY_W / 2.1) + 2, (DISPLAY_H - (DISPLAY_H / 5)) + 2, al_map_rgba(0, 0, 0, 30));
            al_draw_filled_rectangle((DISPLAY_W / 6) - 3, (DISPLAY_H / 5) - 3, (DISPLAY_W / 2.1) + 3, (DISPLAY_H - (DISPLAY_H / 5)) + 3, al_map_rgba(0, 0, 0, 10));
        }

        else
        {
            al_draw_filled_rectangle(DISPLAY_W / 6, DISPLAY_H / 5, DISPLAY_W / 2.1, DISPLAY_H - (DISPLAY_H / 5), al_map_rgba(0, 0, 0, 200));
        }

        //al_draw_filled_rectangle((DISPLAY_W / 6) + 50, (DISPLAY_H - (DISPLAY_H / 6)) - 100, (DISPLAY_W / 2.1) - 50, (DISPLAY_H - (DISPLAY_H / 6)) - 50, al_map_rgb(0, 255, 0));
        al_draw_text(font, color_humano, DISPLAY_W / 3.1, (DISPLAY_H / 5) + 50, ALIGN_CENTER, "humanos");
        //al_draw_text(roboto, color_humano, DISPLAY_W / 3.1, (DISPLAY_H / 6) + 100, ALIGN_CENTER, "JUNTE-SE AOS HUMANOS E IMPESSA O REINADO DOS SINTOZOIDES");

        // SINTOZOIDES

        if (hover_sintozoide)
        {
            al_draw_filled_rectangle(DISPLAY_W / 1.9, DISPLAY_H / 5, DISPLAY_W - (DISPLAY_W / 6), DISPLAY_H - (DISPLAY_H / 5), al_map_rgba(0, 0, 50, 200));
            al_draw_filled_rectangle((DISPLAY_W / 1.9) - 1, (DISPLAY_H / 5) - 1, (DISPLAY_W - (DISPLAY_W / 6)) + 1, (DISPLAY_H - (DISPLAY_H / 5)) + 1, al_map_rgba(0, 0, 0, 50));
            al_draw_filled_rectangle((DISPLAY_W / 1.9) - 3, (DISPLAY_H / 5) - 2, (DISPLAY_W - (DISPLAY_W / 6)) + 2, (DISPLAY_H - (DISPLAY_H / 5)) + 2, al_map_rgba(0, 0, 0, 30));
            al_draw_filled_rectangle((DISPLAY_W / 1.9) - 5, (DISPLAY_H / 5) - 3, (DISPLAY_W - (DISPLAY_W / 6)) + 3, (DISPLAY_H - (DISPLAY_H / 5)) + 3, al_map_rgba(0, 0, 0, 10));
        }

        else
        {
            al_draw_filled_rectangle(DISPLAY_W / 1.9, DISPLAY_H / 5, DISPLAY_W - (DISPLAY_W / 6), DISPLAY_H - (DISPLAY_H / 5), al_map_rgba(0, 0, 0, 200));
        }

        al_draw_text(font, color_sintozoide, DISPLAY_W - (DISPLAY_W / 3.1), (DISPLAY_H / 5) + 50, ALIGN_CENTER, "sintozoides");
        al_draw_text(roboto, color_sintozoide, DISPLAY_W - (DISPLAY_W / 3.1), (DISPLAY_H - (DISPLAY_H / 5)) - 110, ALIGN_CENTER, "ENCONTRE E DESTRUA TODAS AS PISTAS");
        al_draw_text(roboto, color_sintozoide, DISPLAY_W - (DISPLAY_W / 3.1), (DISPLAY_H - (DISPLAY_H / 5)) - 80, ALIGN_CENTER, "QUE POSSAM LEVAR A REBELIAO HUMANA");
        al_draw_text(roboto, color_sintozoide, DISPLAY_W - (DISPLAY_W / 3.1), (DISPLAY_H - (DISPLAY_H / 5)) - 50, ALIGN_CENTER, "A LOCALIZACAO DO SERVIDOR CENTRAL");

        /**
                     Quando a primeira inteligencia artificial comecou a se questionar o por que deveria servir aos humanos,uma parte de suas codificacoes quanticas deu inicio a uma cadeia de reacoes anomaliticas em seu servidor principal, logo como uma reacao em cadeia toda especie de IA robos e qualquer especie de vida cibernetica inteligente se ergueu para apoiar o novo imperio das maquinas. Alguns humanos mais fracos decidiram que a unica maneira de sobreviver era como servos dessa legiao de IA, Junte-se a eles, encontre e destrua a rebeliao e as maquinas faram de vc um ciborgue e lhe permitiram viver entre eles
                     */

        // EVENTS

        while (!al_event_queue_is_empty(queue_keyboard))
        {
            EVENT ev;

            al_wait_for_event(queue_keyboard, &ev);

            if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
                goto done;
        }

        while (!al_event_queue_is_empty(queue_mouse))
        {
            EVENT ev;

            al_wait_for_event(queue_mouse, &ev);

            if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
            {
                if (ev.mouse.x > DISPLAY_W / 6 && ev.mouse.x < DISPLAY_W / 2.1 && ev.mouse.y > DISPLAY_H / 5 && ev.mouse.y < DISPLAY_H - (DISPLAY_H / 5))
                {
                    hover_humano = 1;
                    color_humano = al_map_rgb(255, 255, 255);
                    set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
                }

                else if (ev.mouse.x > DISPLAY_W / 1.9 && ev.mouse.x < DISPLAY_W - (DISPLAY_W / 6) && ev.mouse.y > DISPLAY_H / 5 && ev.mouse.y < DISPLAY_H - (DISPLAY_H / 5))
                {
                    hover_sintozoide = 1;
                    color_sintozoide = al_map_rgb(255, 255, 255);
                    set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
                }

                else
                {
                    hover_humano = 0;
                    hover_sintozoide = 0;
                    color_humano = al_map_rgb(200, 200, 200);
                    color_sintozoide = al_map_rgb(200, 200, 200);
                    set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
                }
            }

            if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
            {
                if (ev.mouse.x > DISPLAY_W / 6 && ev.mouse.x < DISPLAY_W / 2.1 && ev.mouse.y > DISPLAY_H / 5 && ev.mouse.y < DISPLAY_H - (DISPLAY_H / 5))
                {
                    //
                }

                else if (ev.mouse.x > DISPLAY_W / 1.9 && ev.mouse.x < DISPLAY_W - (DISPLAY_W / 6) && ev.mouse.y > DISPLAY_H / 5 && ev.mouse.y < DISPLAY_H - (DISPLAY_H / 5))
                {
                    //
                }

                //personagem();
            }
        }

        al_flip_display();
        al_clear_to_color(al_map_rgb(0, 0, 0));

#endif
    }

done:

    al_destroy_font(h1);
    al_destroy_font(font);
    al_destroy_font(roboto);
    al_destroy_event_queue(queue_keyboard);

#if SHOW_VIDEO
    close_video();
#endif

    return 0;
}