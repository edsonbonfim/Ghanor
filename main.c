#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "headers/core.h"
#include "headers/game.h"

int main(int argc, char ** argv)
{
    init();

    // SET FULLSCREEN WINDOW
    al_set_new_display_flags(FULLSCREEN_WINDOW);

    // INSTALL ALLEGRO COMPONENTS
    install_allegro_components();

    // INSTALL ALLEGRO ADDONS
    init_allegro_addons();

    al_reserve_samples(1);

    // CREATE DISPLAY
    display = create_display(1, 1);

    // LOAD FONTS
    h1           = load_ttf_font("fonts/Bethanie-Snake.ttf", 45, 0);
    paragraph    = load_ttf_font("fonts/Roboto-Regular.ttf", 15, 0);
    //history_font = load_ttf_font("fonts/Roboto-Regular.ttf", 14, 0);

    //event_queue_menu_mouse_click = create_event_queue();
    //al_register_event_source(event_queue_menu_mouse_click, al_get_mouse_event_source());

    //event_queue_personagens_keyboard_char = create_event_queue();
    //al_register_event_source(event_queue_personagens_keyboard_char, al_get_keyboard_event_source());

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

    // DESTROY EVENT QUEUE
    //al_destroy_event_queue(event_queue_menu_mouse_hover);
    //al_destroy_event_queue(event_queue_menu_mouse_click);
    //al_destroy_event_queue(event_queue_personagens_keyboard_char);

    // EXIT SUCCESS
    return EXIT_SUCCESS;
} //END_OF_MAIN();
