void draw_menu(FONT * h1, FONT * font, COLOR color)
{
    al_draw_filled_rectangle(0, 0, DISPLAY_W, DISPLAY_H, al_map_rgba(0, 0, 0, 200));
    al_draw_filled_rectangle(DISPLAY_W / 4, DISPLAY_H / 4, DISPLAY_W - (DISPLAY_W / 4), DISPLAY_H - (DISPLAY_H / 4), al_map_rgba(0, 0, 0, 200));
    al_draw_text(h1,   color, DISPLAY_W/2, (DISPLAY_H/2)-110, ALIGN_CENTER, "rebelation");
    al_draw_text(font, color, DISPLAY_W/2, (DISPLAY_H/2)-30,  ALIGN_CENTER, "continuar");
    al_draw_text(font, color, DISPLAY_W/2, (DISPLAY_H/2)+20,  ALIGN_CENTER, "novo jogo");
    al_draw_text(font, color, DISPLAY_W/2, (DISPLAY_H/2)+70,  ALIGN_CENTER, "creditos");
    al_draw_text(font, color, DISPLAY_W/2, (DISPLAY_H/2)+120, ALIGN_CENTER, "sair");
}

int get_menu_mouse_opt(EVENT ev)
{
    if ((ev.mouse.x > (DISPLAY_W/4)+270 && ev.mouse.x < (DISPLAY_W/4)+415)
    &&   ev.mouse.y > (DISPLAY_H/4)+160 && ev.mouse.y < (DISPLAY_H/4)+185)
    
    return 0;

    else if ((ev.mouse.x > (DISPLAY_W/4)+268 && ev.mouse.x < (DISPLAY_W/4)+420)
         &&   ev.mouse.y > (DISPLAY_H/4)+210 && ev.mouse.y < (DISPLAY_H/4)+235)

    return 1;

    else if ((ev.mouse.x > (DISPLAY_W/4)+278 && ev.mouse.x < (DISPLAY_W/4)+410)
         &&   ev.mouse.y > (DISPLAY_H/4)+260 && ev.mouse.y < (DISPLAY_H/4)+285)

    return 2;

    else if ((ev.mouse.x > (DISPLAY_W/4)+312 && ev.mouse.x < (DISPLAY_W/4)+375)
         &&   ev.mouse.y > (DISPLAY_H/4)+310 && ev.mouse.y < (DISPLAY_H/4)+335)

    return 3;

    return -1;
}

int get_menu_keyboard_opt(EVENT ev, int * count)
{
    switch(ev.keyboard.keycode)
    {
        case ALLEGRO_KEY_DOWN:
        case ALLEGRO_KEY_RIGHT:
            *count = *count + 1;
            return *count;
            break;

        case ALLEGRO_KEY_UP:
        case ALLEGRO_KEY_LEFT:
            *count = *count - 1;
            return *count;
            break;
    }

    return -1;
}

void menu_mouse_hover(int * opt, int * count, QUEUE * queue, FONT * font, COLOR color)
{
    if (*count > 3)
    {
        *opt = 0;
        *count = 0;
    }

    if (*count < 0)
    {
        *opt = 3;
        *count = 3;
    }

    while (!al_is_event_queue_empty(queue))
    {
        EVENT ev;
        
        al_wait_for_event(queue, &ev);
        
        if (ev.type == EVENT_MOUSE_AXES)
            *opt = get_menu_mouse_opt(ev);

        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
            *opt = get_menu_keyboard_opt(ev, count);
    }

    switch(*opt)
    {
        case 0:
            set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
            al_draw_text(font, color, (DISPLAY_W/2)+1, (DISPLAY_H/2)-29, ALIGN_CENTER, "continuar");
            break;

        case 1:
            set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
            al_draw_text(font, color, (DISPLAY_W/2)+1, (DISPLAY_H/2)+21, ALIGN_CENTER, "novo jogo");
            break;

        case 2:
            set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
            al_draw_text(font, color, (DISPLAY_W/2)+1, (DISPLAY_H/2)+71, ALIGN_CENTER, "creditos");
            break;

        case 3:
            set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
            al_draw_text(font, color, (DISPLAY_W/2)+1, (DISPLAY_H/2)+121, ALIGN_CENTER, "sair");
            break;

        default:
            set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
    }
}

int menu_mouse_click(int * opt, int * count, QUEUE * queue)
{
    while (!al_is_event_queue_empty(queue))
    {
        EVENT ev;
        
        al_wait_for_event(queue, &ev);
        
        if (ev.type == EVENT_MOUSE_BUTTON_UP)
            *opt = get_menu_mouse_opt(ev);

        else if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
            *opt = *count;
    }

    switch (*opt)
    {
        case 0:
            return 0;

        case 2:
            return 2;

        case 3:
            return -1;

        default:
            return -2;
    }

    return -2;
}

static void video_display(ALLEGRO_VIDEO *video)
{
    ALLEGRO_BITMAP * frame = al_get_video_frame(video);
    int w, h, x, y;

    if (!frame)
        return;

    w = DISPLAY_W + 300;
    h = DISPLAY_H + 300;

    x = (DISPLAY_W - w) / 2;
    y = (DISPLAY_H - h) / 2;

    al_draw_scaled_bitmap(frame, 0, 0,
                          al_get_bitmap_width(frame),
                          al_get_bitmap_height(frame), x, y, w, h, 0);
}