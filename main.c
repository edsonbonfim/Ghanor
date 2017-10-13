#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "headers/core.h"
#include "headers/game.h"

int main(int argc, char **argv)
{
    init();

    // SET FULLSCREEN WINDOW
    al_set_new_display_flags(FULLSCREEN_WINDOW | ALLEGRO_PROGRAMMABLE_PIPELINE | ALLEGRO_OPENGL);

    // INSTALL ALLEGRO COMPONENTS
    install_allegro_components();

    // INSTALL ALLEGRO ADDONS
    init_allegro_addons();

    al_reserve_samples(1);

    // CREATE DISPLAY
    display = create_display(1080, 720);

    // DISPLAY DIMENTIONS
    DISPLAY_W = al_get_display_width(display);
    DISPLAY_H = al_get_display_height(display);

    // WINDOW TITLE
    al_set_window_title(display, "RPG SEM NOME!!!");

    // RUN GAME
    game_loop();

    // UNINSTALL KEYBOARD
    al_uninstall_keyboard();

    // DESTROY DISPLAY
    al_destroy_display(display);

    // EXIT SUCCESS
    return EXIT_SUCCESS;
} //END_OF_MAIN();