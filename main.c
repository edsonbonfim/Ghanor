#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

int FPS = 60;
int DISPLAY_W;
int DISPLAY_H;

ALLEGRO_DISPLAY *display;

#define SHOW_VIDEO 1

#include "game/utils.c"

#if SHOW_VIDEO
    #include "game/video.c"
#endif

#include "game/game.c"

int main(int argc, char **argv)
{
    if (!al_init())
    {
        fprintf(stderr, "Falha ao inicializar o allegro.\n");
        return -1;
    }

    if (!al_install_mouse())
    {
        fprintf(stderr, "Falha ao inicializar o mouse.\n");
        return -1;
    }

    if (!al_install_keyboard())
    {
        fprintf(stderr, "Falha ao inicializar o teclado.\n");
        return -1;
    }

    if (!al_install_audio())
    {
        fprintf(stderr, "Falha ao inicializar o audio.\n");
        return -1;
    }

    al_init_font_addon();

    if (!al_init_ttf_addon())
    {
        fprintf(stderr, "Falha ao inicializar o add-on allegro_ttf.\n");
        return -1;
    }
    
    if (!al_init_image_addon())
    {
        fprintf(stderr, "Falha ao inicializar o add-on allegro_image.\n");
        return -1;
    }

    if (!al_init_primitives_addon())
    {
        fprintf(stderr, "Falha ao inicializar o add-on allegro_primitives.\n");
        return -1;
    }

    #if SHOW_VIDEO
        if (!al_init_video_addon())
        {
            fprintf(stderr, "Falha ao inicializar o add-on allegro_video.\n");
            return -1;
        }
    #endif

    al_reserve_samples(1);

    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);    

    if (!(display = al_create_display(1080, 720)))
    {
        fprintf(stderr, "Falha ao criar display.\n");
        return -1;
    }

    al_set_window_title(display, "RPG SEM NOME!!!");

    DISPLAY_W = al_get_display_width(display);
    DISPLAY_H = al_get_display_height(display);

    game_loop();

    al_uninstall_audio();
    al_uninstall_mouse();
    al_uninstall_keyboard();

    al_destroy_display(display);

    return 0;
}
//END_OF_MAIN();