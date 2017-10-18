int create_personagem(PERSONAGEM *personagem)
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
    
    if (!(roboto = al_load_ttf_font("data/fonts/Roboto-Regular.ttf", 14, 0)))
    {
        al_destroy_font(h1);
        al_destroy_font(font);
        fprintf(stderr, "Falha ao carregar a font gtek.\n");
        return -1;
    }

    strcpy(personagem->identidade.nome, "");

    int get_keyboard_personagem_nome = 0;
    ALLEGRO_EVENT_QUEUE *event_queue_personagens_keyboard_char = al_create_event_queue();
    al_register_event_source(event_queue_personagens_keyboard_char, al_get_keyboard_event_source());

    #if SHOW_VIDEO
        load_video();
    #else
        load_background();
    #endif

    int opt = 0;

    while (1)
    {
        #if SHOW_VIDEO
        
            if (redraw && al_event_queue_is_empty(queue_video))
            {
                al_clear_to_color(al_map_rgb(0, 0, 0));

                video_display(video);

                al_draw_filled_rectangle(0, 0, DISPLAY_W, DISPLAY_H, al_map_rgba(0, 0, 0, 100));
                al_draw_filled_rectangle(DISPLAY_W / 3, DISPLAY_H / 6, DISPLAY_W - (DISPLAY_W / 3), DISPLAY_H - (DISPLAY_H / 6), al_map_rgba(0, 0, 0, 200));

                al_draw_text(font, al_map_rgb(153, 153, 153), DISPLAY_W / 2, (DISPLAY_H / 6) + 50, ALLEGRO_ALIGN_CENTER, "alistamento");
                
                al_draw_text(roboto, al_map_rgb(255, 255, 255), DISPLAY_W / 2, (DISPLAY_H / 6) + 100, ALLEGRO_ALIGN_CENTER, "NOME: ");
                
                if (strlen(personagem->identidade.nome) > 0)
                    al_draw_text(roboto, al_map_rgb(255, 255, 255), DISPLAY_W / 2, (DISPLAY_H / 6) + 150, ALLEGRO_ALIGN_CENTER, personagem->identidade.nome);

                while (!al_is_event_queue_empty(event_queue_personagens_keyboard_char))
                {
                    ALLEGRO_EVENT ev;
            
                    al_wait_for_event(event_queue_personagens_keyboard_char, &ev);
            
                    if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                        goto done;

                    if (!get_keyboard_personagem_nome)
                    {
                        if (ev.type == ALLEGRO_EVENT_KEY_CHAR)
                        {
                            if (strlen(personagem->identidade.nome) <= 16)
                            {
                                char temp[] = {ev.keyboard.unichar, '\0'};
                                if (ev.keyboard.unichar == ' ')
                                {
                                    strcat(personagem->identidade.nome, temp);
                                }
                                else if (ev.keyboard.unichar >= 'A' &&
                                         ev.keyboard.unichar <= 'Z')
                                {
                                    strcat(personagem->identidade.nome, temp);
                                }
                                else if (ev.keyboard.unichar >= 'a' &&
                                         ev.keyboard.unichar <= 'z')
                                {
                                    strcat(personagem->identidade.nome, temp);
                                }
                            }
                    
                            if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(personagem->identidade.nome) != 0)
                            {
                                personagem->identidade.nome[strlen(personagem->identidade.nome) - 1] = '\0';
                            }
                        }
            
                        if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
                        {
                            opt = 1;
                            set_personagem(personagem);
                            get_keyboard_personagem_nome = 1;

                            goto done;
                        }
                    }
                }

                redraw = 0;
                
                al_flip_display();
            }

            g_event_video();

        #endif

        al_flip_display();
    }

    done:

    al_destroy_font(h1);
    al_destroy_font(font);
    al_destroy_font(roboto);

    return opt;
}
