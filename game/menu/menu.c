#include "menu.h"

int menu(ALLEGRO_DISPLAY *display, int *option)
{
    if (*option == MENU_SAIR)
        return *option;

    *option = (contar_numero_de_personagens()) ? MENU_CONTINUAR : MENU_NOVO_JOGO;

    /** ----- Fontes ----- **/

    ALLEGRO_FONT *font[]
    =
    {
        [FONT_GTEK]       load_font("data/fonts/gtek-technology.ttf", 20, 0),
        [FONT_GTEK_TITLE] load_font("data/fonts/gtek-technology.ttf", 30, 0)
    };

    /** ----- Bitmaps ----- **/

    ALLEGRO_BITMAP *bitmap[]
    =
    {
        [BITMAP_BACKGROUND] load_bitmap_at_size("data/images/bg.jpeg", DISPLAY_W, DISPLAY_H)
    };

    /** ----- Fila de Eventos ----- **/

    ALLEGRO_EVENT_QUEUE *queue[]
    =
    {
        [EVENT_MOUSE_HOVER]       create_event_queue(),
        [EVENT_MOUSE_CLICK]       create_event_queue(),
        [EVENT_KEYBOARD_KEYDOWN]  create_event_queue(),
        [EVENT_KEYBOARD_KEYENTER] create_event_queue()
    };

    al_register_event_source(queue[EVENT_MOUSE_HOVER], al_get_mouse_event_source());
    al_register_event_source(queue[EVENT_MOUSE_CLICK], al_get_mouse_event_source());

    al_register_event_source(queue[EVENT_KEYBOARD_KEYDOWN], al_get_keyboard_event_source());
    al_register_event_source(queue[EVENT_KEYBOARD_KEYENTER], al_get_keyboard_event_source());

    /** ----- Loop ----- **/

    while(1)
    {
        /** ----- Desenhos ----- **/

        menu_draw_background(display, bitmap);
        menu_draw_text(display, font, option);

        al_flip_display();

        /** ----- Eventos ----- ***/

        event_display_resize();        
        
        menu_event_mouse_hover(display, queue, option);
        menu_event_keyboard_keydown(queue, option);

        if (menu_event_mouse_click(display, queue, option)
        ||  menu_event_keyboard_keyenter(queue, option)
        ||  event_display_close(option))
            break;
    }

    al_destroy_font(font[FONT_GTEK]);
    al_destroy_font(font[FONT_GTEK_TITLE]);

    al_destroy_bitmap(BITMAP_BACKGROUND);

    al_destroy_event_queue(queue[EVENT_MOUSE_CLICK]);
    al_destroy_event_queue(queue[EVENT_MOUSE_HOVER]);
    al_destroy_event_queue(queue[EVENT_KEYBOARD_KEYDOWN]);
    al_destroy_event_queue(queue[EVENT_KEYBOARD_KEYENTER]);

    return *option;
}

/** ----- Desenhos ----- **/

void menu_draw_background(ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP **bitmap)
{
    int display_width  = al_get_display_width(display);
    int display_height = al_get_display_height(display);

    int x1 = display_width  / 4;
    int y1 = display_height / 4 - 20;
    int x2 = display_width  - x1;
    int y2 = display_height - y1;

    al_draw_tinted_bitmap(bitmap[BITMAP_BACKGROUND], al_map_rgb(127.5, 127.5, 127.5), 0, 0, 0);
    al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgba(0, 0, 0, 170));
}

void menu_draw_text(ALLEGRO_DISPLAY *display, ALLEGRO_FONT **font, int *option)
{
    int x = al_get_display_width(display)  / 2;
    int y = al_get_display_height(display) / 2;
    
    ALLEGRO_FONT *title   = font[FONT_GTEK_TITLE];
    ALLEGRO_FONT *options = font[FONT_GTEK];
    
    int numero_de_personagens = contar_numero_de_personagens();

    ALLEGRO_COLOR continuar = (!numero_de_personagens) ? al_map_rgb(50, 50, 50) : COLOR_GRAY;
    
    al_draw_text(title,   COLOR_GRAY, x, y - 140, ALLEGRO_ALIGN_CENTER, "ghanor");
    al_draw_text(title,   COLOR_GRAY, x, y - 110, ALLEGRO_ALIGN_CENTER, "the revolution");
    al_draw_text(options, continuar,  x, y - 40,  ALLEGRO_ALIGN_CENTER, "continuar");
    al_draw_text(options, COLOR_GRAY, x, y + 10,  ALLEGRO_ALIGN_CENTER, "novo jogo");
    al_draw_text(options, COLOR_GRAY, x, y + 60,  ALLEGRO_ALIGN_CENTER, "creditos");
    al_draw_text(options, COLOR_GRAY, x, y + 110, ALLEGRO_ALIGN_CENTER, "sair");

    switch (*option)
    {
        case MENU_CONTINUAR:
            if (!numero_de_personagens)
                break;

            al_draw_text(options, COLOR_WHITE, x + 1, y - 39, ALLEGRO_ALIGN_CENTER, "continuar");
            break;

        case MENU_NOVO_JOGO:
            al_draw_text(options, COLOR_WHITE, x + 1, y + 11, ALLEGRO_ALIGN_CENTER, "novo jogo");
            break;

        case MENU_CREDITOS:
            al_draw_text(options, COLOR_WHITE, x + 1, y + 61, ALLEGRO_ALIGN_CENTER, "creditos");
            break;

        case MENU_SAIR:
            al_draw_text(options, COLOR_WHITE, x + 1, y + 111, ALLEGRO_ALIGN_CENTER, "sair");
    }
}

/** ----- Eventos ----- **/

int menu_event_mouse_hover_get_opt(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT event)
{
    int x = al_get_display_width(display)  / 2;
    int y = al_get_display_height(display) / 2;

    if (get_mouse_position(event, x - 70, y - 40, x + 70, y - 20))
        return MENU_CONTINUAR;

    if (get_mouse_position(event, x - 75, y + 10, x + 75, y + 30))
        return MENU_NOVO_JOGO;

    if (get_mouse_position(event, x - 65, y + 60, x + 65, y + 80))
        return MENU_CREDITOS;

    if (get_mouse_position(event, x - 30, y + 110, x + 30, y + 130))
        return MENU_SAIR;

    return 0;
}

int menu_event_mouse_hover(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE **queue, int *option)
{
    while (!al_is_event_queue_empty(queue[EVENT_MOUSE_HOVER]))
    {
        ALLEGRO_EVENT event;

        al_wait_for_event(queue[EVENT_MOUSE_HOVER], &event);

        if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
            *option = menu_event_mouse_hover_get_opt(display, event);
    }
}

int menu_event_mouse_click(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE **queue, int *option)
{
    while (!al_is_event_queue_empty(queue[EVENT_MOUSE_CLICK]))
    {
        ALLEGRO_EVENT event;

        al_wait_for_event(queue[EVENT_MOUSE_CLICK], &event);

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
        {
            *option = menu_event_mouse_hover_get_opt(display, event);
            return *option;
        }
    }
}

int menu_event_keyboard_keydown(ALLEGRO_EVENT_QUEUE **queue, int *option)
{
    while (!al_is_event_queue_empty(queue[EVENT_KEYBOARD_KEYDOWN]))
    {
        ALLEGRO_EVENT event;

        al_wait_for_event(queue[EVENT_KEYBOARD_KEYDOWN], &event);

        if (event.type == ALLEGRO_EVENT_KEY_DOWN)
            menu_event_keyboard_keydown_get_opt(event, option);
    }

    return MENU_CONTINUAR;
}

void menu_event_keyboard_keydown_get_opt(ALLEGRO_EVENT event, int *option)
{
    if (*option == MENU_SAIR + 1)
        *option = (contar_numero_de_personagens()) ? MENU_CONTINUAR : MENU_NOVO_JOGO;

    else if (*option == MENU_CONTINUAR - 1
         || (*option == MENU_CONTINUAR && !contar_numero_de_personagens()))
        *option = MENU_SAIR;

    else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
        (*option)++;

    else if (event.keyboard.keycode == ALLEGRO_KEY_UP)
        (*option)--;
}

int menu_event_keyboard_keyenter(ALLEGRO_EVENT_QUEUE **queue, int *option)
{
    while (!al_is_event_queue_empty(queue[EVENT_KEYBOARD_KEYENTER]))
    {
        ALLEGRO_EVENT event;

        al_wait_for_event(queue[EVENT_KEYBOARD_KEYENTER], &event);

        if (event.keyboard.keycode == ALLEGRO_KEY_ENTER)
            return 1;
    }

    return 0;
}
