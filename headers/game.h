#include "game/menu/menu.c"
#include "game/new_game/new_game.c"

void game_loop()
{
    while (1)
    {
        switch(menu())
        {
            case 0:
                new_game();
                break;

            case -1:
                goto done;
        }
    }

    done:
    printf("\nDone!!\n\n");
}