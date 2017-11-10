#include "create.h"

void personagem_create(Personagem *personagem)
{
    /** ----- Fontes ----- **/

    ALLEGRO_FONT *font[]
    =
    {
        [FONT_GTEK]   load_font("data/fonts/gtek-technology.ttf", 20, 0),
        [FONT_ROBOTO] load_font("data/fonts/roboto-regular.ttf",  12, 0)
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
        [EVENT_MOUSE] create_event_queue(),
        
        [EVENT_KEYBOARD_KEYDOWN]  create_event_queue(),
        [EVENT_KEYBOARD_KEYENTER] create_event_queue()
    };

    al_register_event_source(queue[EVENT_MOUSE], al_get_mouse_event_source());

    al_register_event_source(queue[EVENT_KEYBOARD_KEYDOWN],  al_get_keyboard_event_source());
    al_register_event_source(queue[EVENT_KEYBOARD_KEYENTER], al_get_keyboard_event_source());

    /** ----- Variaveis Auxiliares ----- **/

    int escape = 0;
    int position[2][4];

    ALLEGRO_COLOR bgcolor = COLOR_GRAY;

    /** ----- Loop ----- **/

    while(1)
    {
        /** ----- Desenhos ----- **/

        personagem_create_draw_background(bitmap, bgcolor, position);
        personagem_create_draw_text(font, personagem);
        
        al_flip_display();

        /** Eventos **/
        
        personagem_create_event_mouse(queue, &bgcolor, position);

        personagem_create_event_keyboard_keydown(queue, personagem, escape);
        personagem_create_event_keyboard_keyenter(queue, &escape);
    }

    al_destroy_font(font[FONT_GTEK]);
    al_destroy_font(font[FONT_ROBOTO]);

    al_destroy_bitmap(bitmap[BITMAP_BACKGROUND]);

    al_destroy_event_queue(queue[EVENT_MOUSE]);

    al_destroy_event_queue(queue[EVENT_KEYBOARD_KEYDOWN]);
    al_destroy_event_queue(queue[EVENT_KEYBOARD_KEYENTER]);
}

void personagem_create_draw_background(ALLEGRO_BITMAP **bitmap, ALLEGRO_COLOR bgcolor, int position[][4])
{
    int x1 = DISPLAY_W / 3;
    int y1 = DISPLAY_H / 5;
    int x2 = DISPLAY_W - x1;
    int y2 = DISPLAY_H - y1;

    al_draw_bitmap(bitmap[BITMAP_BACKGROUND], 0, 0, 0);

    personagem_layout_draw_background(x1, y1, x2, y2, al_map_rgba(0, 0, 0, 200));

    position[0][0] = x1 + 50;
    position[0][1] = y1 + 110;
    position[0][2] = x2 - 50;
    position[0][3] = y1 + 143;
    
    al_draw_rectangle(position[0][0], position[0][1], position[0][2], position[0][3], bgcolor, 1);

    position[1][0] = x1 + 50;
    position[1][1] = y2 - 63;
    position[1][2] = x2 - 50;
    position[1][3] = y2 - 30;
    
    al_draw_rectangle(position[1][0], position[1][1], position[1][2], position[1][3], bgcolor, 1);
}

void personagem_create_draw_text(ALLEGRO_FONT **font, Personagem *personagem)
{
    int x1  = DISPLAY_W / 2;
    int x2 = DISPLAY_W / 3;
    int y1 = DISPLAY_H / 5;
    int y2 = DISPLAY_H - y1;

    al_draw_text(font[FONT_GTEK], COLOR_WHITE, x1, y1 + 50,  ALLEGRO_ALIGN_CENTER, "identidade");
    al_draw_text(font[FONT_ROBOTO], COLOR_WHITE, x2 + 80, y1 + 120, ALLEGRO_ALIGN_CENTER, "NICK: ");
    al_draw_text(font[FONT_ROBOTO], COLOR_WHITE, DISPLAY_W / 2, y2 - 53, ALLEGRO_ALIGN_CENTER, "PRONTO");

    if (strlen(personagem->identidade.nome) > 0)
        al_draw_text(font[FONT_ROBOTO], COLOR_WHITE, x1, y1 + 120, ALLEGRO_ALIGN_CENTER, personagem->identidade.nome);
}

void personagem_create_event_keyboard_keydown(ALLEGRO_EVENT_QUEUE **queue, Personagem *personagem, int escape)
{
    while (!al_is_event_queue_empty(queue[EVENT_KEYBOARD_KEYDOWN]))
    {
        ALLEGRO_EVENT event;

        al_wait_for_event(queue[EVENT_KEYBOARD_KEYDOWN], &event);

        if (!escape)
        {
            if (event.type == ALLEGRO_EVENT_KEY_CHAR)
            {
                if (strlen(personagem->identidade.nome) <= 16)
                {
                    char temp[] = {toupper(event.keyboard.unichar), '\0'};
                    
                    if (toupper(event.keyboard.unichar) >= 'A'
                    &&  toupper(event.keyboard.unichar) <= 'Z')
                    {
                        strcat(personagem->identidade.nome, temp);
                    }
                }
        
                if (event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(personagem->identidade.nome) != 0)
                {
                    personagem->identidade.nome[strlen(personagem->identidade.nome) - 1] = '\0';
                }
            }
        }
    }
}

void personagem_create_event_keyboard_keyenter(ALLEGRO_EVENT_QUEUE **queue, int *escape)
{
    while (!al_is_event_queue_empty(queue[EVENT_KEYBOARD_KEYENTER]))
    {
        ALLEGRO_EVENT event;

        al_wait_for_event(queue[EVENT_KEYBOARD_KEYENTER], &event);

        if (event.keyboard.keycode == ALLEGRO_KEY_ENTER)
            *escape = 1;
    }
}

void personagem_create_event_mouse(ALLEGRO_EVENT_QUEUE **queue, ALLEGRO_COLOR *bgcolor, int position[][4])
{
    while (!al_is_event_queue_empty(queue[EVENT_MOUSE]))
    {
        ALLEGRO_EVENT event;

        al_wait_for_event(queue[EVENT_MOUSE], &event);

        if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
            personagem_create_event_mouse_hover(event, position, bgcolor);
    }
}

void personagem_create_event_mouse_hover(ALLEGRO_EVENT event, int position[][4], ALLEGRO_COLOR *bgcolor)
{
    int x1 = position[0][0];
    int y1 = position[0][1];
    int x2 = position[0][2];
    int y2 = position[0][3];

    *bgcolor = (get_mouse_position(event, x1, y1, x2, y2)) ? COLOR_WHITE : COLOR_GRAY;
}
