#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // toupper

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

ALLEGRO_DISPLAY *display;

ALLEGRO_COLOR COLOR_GRAY;
ALLEGRO_COLOR COLOR_WHITE;
ALLEGRO_COLOR COLOR_BLACK;

enum FONT { FONT_GTEK, FONT_GTEK_TITLE, FONT_ROBOTO };
enum BITMAP { BITMAP_BACKGROUND };
enum EVENT { EVENT_MOUSE, EVENT_MOUSE_HOVER, EVENT_MOUSE_CLICK, EVENT_KEYBOARD_KEYDOWN, EVENT_KEYBOARD_KEYENTER };

int DISPLAY_W;
int DISPLAY_H;

#include "game/game.c"

int main (int argv, char **argc)
{
    if (!(al_init()))
    {
        fprintf(stderr, "Falha ao inicializar allegro.\n");
        return -1;
    }

    if (!(al_install_mouse()))
    {
        fprintf(stderr, "Falha ao instalar mouse.\n");
        return -1;
    }

    if (!(al_install_keyboard()))
    {
        fprintf(stderr, "Falha ao instalar keyboard.\n");
        return -1;
    }

    al_init_font_addon();

    if (!(al_init_ttf_addon()))
    {
        fprintf(stderr, "Falha ao inicializar ttf add-on.\n");
        return -1;
    }

    if (!(al_init_image_addon()))
    {
        fprintf(stderr, "Falha ao inicializar image add-on.\n");
        return -1;
    }

    if (!(al_init_primitives_addon()))
    {
        fprintf(stderr, "Falha ao inicializar primitives add-on.\n");
        return -1;
    }

    ALLEGRO_DISPLAY *display = al_create_display(1080, 720);
    
    if (!display)
    {
        fprintf(stderr, "Falha ao criar display.\n");
        return -1;
    }
    
    DISPLAY_W = al_get_display_width(display);
    DISPLAY_H = al_get_display_height(display);

    COLOR_GRAY  = al_map_rgb(153, 153, 153);
    COLOR_WHITE = al_map_rgb(255, 255, 255);
    COLOR_BLACK = al_map_rgb(0, 0, 0);

    game();

    return 0;
}
