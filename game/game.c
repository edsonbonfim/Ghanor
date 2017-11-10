#include "tools/tools.c"

#include "personagem/layout.c"
#include "personagem/personagem.c"
#include "personagem/raca/raca.c"
#include "personagem/create/create.c"
#include "personagem/classe/humano/humano.c"
#include "personagem/classe/sintozoide/sintozoide.c"

#include "menu/menu.c"
#include "creditos/creditos.c"
#include "novo_jogo/novo_jogo.c"

enum { BITMAP_PERSONAGEM_FRENTE };

void test(ALLEGRO_DISPLAY *display, int *option)
{
    /** ----- Bitmaps ----- **/

    ALLEGRO_BITMAP *bitmap[]
    =
    {
        [BITMAP_PERSONAGEM_FRENTE] load_bitmap("data/images/personagem_frente.png")
    };

    ALLEGRO_BITMAP *personagem = bitmap[BITMAP_PERSONAGEM_FRENTE];

    /** ----- Eventos ----- **/
    
    ALLEGRO_EVENT_QUEUE *event_queue[]
    =
    {
        [EVENT_MOUSE_HOVER] create_event_queue(),
        [EVENT_KEYBOARD_KEYDOWN] create_event_queue()
    };

    al_register_event_source(event_queue[EVENT_MOUSE_HOVER], al_get_mouse_event_source());
    al_register_event_source(event_queue[EVENT_KEYBOARD_KEYDOWN], al_get_keyboard_event_source());

    int x = al_get_display_width(display)  / 2;
    int y = al_get_display_height(display) / 2;

    int test = 1;

    while (1)
    {
        al_clear_to_color(al_map_rgb(0, 0, 0));

        ALLEGRO_EVENT event_mouse;
        ALLEGRO_EVENT event_keyboard;

        while (!al_is_event_queue_empty(event_queue[EVENT_MOUSE_HOVER]))
        {
            al_wait_for_event(event_queue[EVENT_MOUSE_HOVER], &event_mouse);
        }

        while (!al_is_event_queue_empty(event_queue[EVENT_KEYBOARD_KEYDOWN]))
        {
            al_wait_for_event(event_queue[EVENT_KEYBOARD_KEYDOWN], &event_keyboard);

            if (event_keyboard.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                switch(event_keyboard.keyboard.keycode)
                {
                    case ALLEGRO_KEY_UP:
                        while (1)
                        {
                            y--;
                        }
                        break;
                }
            }
        }

        al_draw_bitmap(personagem, x, y, 0);
        al_flip_display();
    }
}

void game(ALLEGRO_DISPLAY *display)
{
    /** Menu **/

    int option;

    while (1)
    {
        switch(menu(display, &option))
        {
            case MENU_CONTINUAR:
    
                break;
                
            case MENU_NOVO_JOGO:
                //test(display, &option);
                novo_jogo(display, &option);
                break;
    
            case MENU_CREDITOS:
                creditos(display, &option);
                break;
    
            case MENU_SAIR:
                return;
        }
    }
}