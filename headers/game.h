#include "game/variables.h"
#include "game/personagens.h"
#include "game/menu/menu.c"

#include "game/enum.h"
//#include "game/history.h"
//#include "game/video.h"

void game_loop()
{
    const int FPS = 60;

    bool exit_game = false;
    bool show_menu = true;

    while (!exit_game)
    {
        if (show_menu)
            g_menu(FPS, show_menu, exit_game);
    }
}