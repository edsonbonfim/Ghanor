typedef struct menu
{
    int continuar;
    int creditos;
    int personagens;
    int sair;
} MENU;

void draw_menu(FONT * font, COLOR color)
{
    al_draw_text(font, color, DISPLAY_W - 150, DISPLAY_H - 100-150, ALIGN_CENTER, "Continuar");
    al_draw_text(font, color, DISPLAY_W - 150, DISPLAY_H - 100-100, ALIGN_CENTER, "Personagens");
    al_draw_text(font, color, DISPLAY_W - 150, DISPLAY_H - 100-50,  ALIGN_CENTER, "Creditos");
    al_draw_text(font, color, DISPLAY_W - 150, DISPLAY_H - 100,     ALIGN_CENTER, "Sair");
}

void set_state_menu_mouse_hover(MENU * menu, EVENT ev)
{
    if ((ev.mouse.x > (DISPLAY_W - 200) && ev.mouse.x < (DISPLAY_W - 100))
    &&   ev.mouse.y > (DISPLAY_H - 250) && ev.mouse.y < (DISPLAY_H - 210))
    {
        menu->continuar = true;
    }

    else if ((ev.mouse.x > (DISPLAY_W - 200) && ev.mouse.x < (DISPLAY_W - 100))
            &&   ev.mouse.y > (DISPLAY_H - 200) && ev.mouse.y < (DISPLAY_H - 160))
    {
        menu->personagens = true;
    }

    else if ((ev.mouse.x > (DISPLAY_W - 200) && ev.mouse.x < (DISPLAY_W - 100))
            &&   ev.mouse.y > (DISPLAY_H - 150) && ev.mouse.y < (DISPLAY_H - 110))
    {
        menu->creditos = true;
    }

    else if ((ev.mouse.x > (DISPLAY_W - 200) && ev.mouse.x < (DISPLAY_W - 100))
            &&   ev.mouse.y > (DISPLAY_H - 100) && ev.mouse.y < (DISPLAY_H - 60))
    {
        menu->sair = true;
    }

    else
    {
        menu->continuar   = false;
        menu->personagens = false;
        menu->creditos    = false;
        menu->sair        = false;
    }
}

void get_state_menu_mouse_hover(MENU * menu, QUEUE * queue)
{
    while (!al_is_event_queue_empty(queue))
    {
        EVENT ev;
        
        al_wait_for_event(queue, &ev);
        
        if (ev.type == EVENT_MOUSE_AXES)
            set_state_menu_mouse_hover(menu, ev);
    }
}

void menu_mouse_hover(MENU * menu, QUEUE * queue_hover, FONT * font, COLOR color)
{
    get_state_menu_mouse_hover(menu, queue_hover);

    if (menu->continuar)
    {
        set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
        al_draw_text(font, color, DISPLAY_W - 150, DISPLAY_H - 100-150, ALIGN_CENTER, "Continuar");
    }

    else if (menu->personagens)
    {
        set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
        al_draw_text(font, color, DISPLAY_W - 150, DISPLAY_H - 100-100, ALIGN_CENTER, "Personagens");

    }

    else if (menu->creditos)
    {
        set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
        al_draw_text(font, color, DISPLAY_W - 150, DISPLAY_H - 100-50, ALIGN_CENTER, "Creditos");
    }

    else if (menu->sair)
    {
        set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
        al_draw_text(font, color, DISPLAY_W - 150, DISPLAY_H - 100, ALIGN_CENTER, "Sair");
    }

    else
    {
        set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
    }
}

/**

MENU menu;

EVENT_QUEUE * event_queue_menu_mouse_hover;
EVENT_QUEUE * event_queue_menu_mouse_click;


void set_state_menu_mouse_click(EVENT ev)
{
    if ((ev.mouse.x > (DISPLAY_W - 200) && ev.mouse.x < (DISPLAY_W - 100))
    &&   ev.mouse.y > (DISPLAY_H - 250) && ev.mouse.y < (DISPLAY_H - 210))
    {
        show_menu = false;
        show_history = true;
    }

    else if ((ev.mouse.x > (DISPLAY_W - 200) && ev.mouse.x < (DISPLAY_W - 100))
         &&   ev.mouse.y > (DISPLAY_H - 200) && ev.mouse.y < (DISPLAY_H - 160))
    {
        show_menu = false;
        show_personagens = true;
    }

    else if ((ev.mouse.x > (DISPLAY_W - 200) && ev.mouse.x < (DISPLAY_W - 100))
         &&   ev.mouse.y > (DISPLAY_H - 100) && ev.mouse.y < (DISPLAY_H - 60))
    {
        exit_game = true;
    }
}

void get_state_menu_mouse_click()
{
    while(!al_is_event_queue_empty(event_queue_menu_mouse_click))
    {
        EVENT ev;

        al_wait_for_event(event_queue_menu_mouse_click, &ev);

        if (ev.type == EVENT_MOUSE_BUTTON_UP)
            set_state_menu_mouse_click(ev);
    }
}



void menu_mouse_click()
{
    get_state_menu_mouse_click();
}
*/

static void video_display(ALLEGRO_VIDEO *video)
{
    ALLEGRO_BITMAP *frame = al_get_video_frame(video);
    int w, h, x, y;

    if (!frame)
        return;

    w = DISPLAY_W;
    h = DISPLAY_H;

    x = (DISPLAY_W - w) / 2;
    y = (DISPLAY_H - h) / 2;

    /* Display the frame. */
    al_draw_scaled_bitmap(frame, 0, 0,
                          al_get_bitmap_width(frame),
                          al_get_bitmap_height(frame), x, y, w, h, 0);
}