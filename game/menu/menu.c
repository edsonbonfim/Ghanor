#include "menu.h"

int menu()
{
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
        [BITMAP_BACKGROUND] load_bitmap_at_size("data/images/bg.png", DISPLAY_W, DISPLAY_H)
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

    /** ----- Variaveis Auxiliares ----- **/

    int option = (contar_numero_de_personagens()) ? 1 : 2;

    /** ----- Loop ----- **/

    while(1)
    {
        /** ----- Desenhos ----- **/

        menu_draw_background(bitmap);
        menu_draw_text(font, &option);

        al_flip_display();

        /** ----- Eventos ----- ***/

        menu_event_mouse_hover(queue, &option);
        menu_event_keyboard_keydown(queue, &option);

        if (menu_event_mouse_click(queue, &option)
        ||  menu_event_keyboard_keyenter(queue, &option))
            break;
    }

    al_destroy_font(font[FONT_GTEK]);
    al_destroy_font(font[FONT_GTEK_TITLE]);

    al_destroy_bitmap(BITMAP_BACKGROUND);

    al_destroy_event_queue(queue[EVENT_MOUSE_CLICK]);
    al_destroy_event_queue(queue[EVENT_MOUSE_HOVER]);
    al_destroy_event_queue(queue[EVENT_KEYBOARD_KEYDOWN]);
    al_destroy_event_queue(queue[EVENT_KEYBOARD_KEYENTER]);

    return option;
}

/** ----- Desenhos ----- **/

void menu_draw_text(ALLEGRO_FONT **font, int *option)
{
    int x = DISPLAY_W / 2;
    int y = DISPLAY_H / 2;

    ALLEGRO_FONT *title   = font[FONT_GTEK_TITLE];
    ALLEGRO_FONT *options = font[FONT_GTEK];

    al_draw_text(title, COLOR_GRAY, x, y - 110, ALLEGRO_ALIGN_CENTER, "without name");

    if (1)
        al_draw_text(options, al_map_rgb(50, 50, 50), x, y - 30, ALLEGRO_ALIGN_CENTER, "continuar");
    else
        al_draw_text(options, COLOR_GRAY, x, y - 30, ALLEGRO_ALIGN_CENTER, "continuar");

    al_draw_text(options, COLOR_GRAY, x, y + 20, ALLEGRO_ALIGN_CENTER, "novo jogo");
    al_draw_text(options, COLOR_GRAY, x, y + 70, ALLEGRO_ALIGN_CENTER, "creditos");
    al_draw_text(options, COLOR_GRAY, x, y + 120, ALLEGRO_ALIGN_CENTER, "sair");

    switch (*option)
    {
        case 1:
            if (!contar_numero_de_personagens())
                break;

            al_draw_text(options, COLOR_WHITE, x + 1, y - 29, ALLEGRO_ALIGN_CENTER, "continuar");
            break;

        case 2:
            al_draw_text(options, COLOR_WHITE, x + 1, y + 21, ALLEGRO_ALIGN_CENTER, "novo jogo");
            break;

        case 3:
            al_draw_text(options, COLOR_WHITE, x + 1, y + 71, ALLEGRO_ALIGN_CENTER, "creditos");
            break;

        case 4:
            al_draw_text(options, COLOR_WHITE, x + 1, y + 121, ALLEGRO_ALIGN_CENTER, "sair");
    }
}

void menu_draw_background(ALLEGRO_BITMAP **bitmap)
{
    int x1 = DISPLAY_W / 4;
    int y1 = DISPLAY_H / 4;
    int x2 = DISPLAY_W - x1;
    int y2 = DISPLAY_H - y1;

    al_draw_bitmap(bitmap[BITMAP_BACKGROUND], 0, 0, 0);

    al_draw_filled_rectangle(0, 0, x1 * 4, y1 * 4, al_map_rgba(0, 0, 0, 100));
    al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgba(0, 0, 0, 200));
    al_draw_filled_rectangle(x1 - 1, y1 - 1, x2 + 1, y2 + 1, al_map_rgba(0, 0, 0, 50));
    al_draw_filled_rectangle(x1 - 2, y1 - 2, x2 + 2, y2 + 2, al_map_rgba(0, 0, 0, 30));
    al_draw_filled_rectangle(x1 - 3, y1 - 3, x2 + 3, y2 + 3, al_map_rgba(0, 0, 0, 10));
}

/** ----- Eventos ----- **/

int menu_event_mouse_hover_get_opt(ALLEGRO_EVENT event)
{
    int x = DISPLAY_W / 2;
    int y = DISPLAY_H / 2;

    if (get_mouse_position(event, x - 70, y - 30, x + 70, y - 10))
        return 1;

    if (get_mouse_position(event, x - 75, y + 20, x + 75, y + 40))
        return 2;

    if (get_mouse_position(event, x - 65, y + 70, x + 65, y + 90))
        return 3;

    if (get_mouse_position(event, x - 30, y + 120, x + 30, y + 140))
        return 4;

    return 0;
}

int menu_event_mouse_hover(ALLEGRO_EVENT_QUEUE **queue, int *option)
{
    while (!al_is_event_queue_empty(queue[EVENT_MOUSE_HOVER]))
    {
        ALLEGRO_EVENT event;

        al_wait_for_event(queue[EVENT_MOUSE_HOVER], &event);

        if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
            *option = menu_event_mouse_hover_get_opt(event);
    }
}

int menu_event_mouse_click(ALLEGRO_EVENT_QUEUE **queue, int *option)
{
    while (!al_is_event_queue_empty(queue[EVENT_MOUSE_CLICK]))
    {
        ALLEGRO_EVENT event;

        al_wait_for_event(queue[EVENT_MOUSE_CLICK], &event);

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
        {
            *option = menu_event_mouse_hover_get_opt(event);
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

    return 1;
}

void menu_event_keyboard_keydown_get_opt(ALLEGRO_EVENT event, int *option)
{
    if (*option == 5)
        *option = (contar_numero_de_personagens()) ? 1 : 2;

    else if (*option == 0 || (*option == 1 && !contar_numero_de_personagens()))
        *option = 4;

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
