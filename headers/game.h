#include "utils.c"
#include "game/menu/menu.c"
#include "game/creditos/creditos.c"
#include "game/new_game/new_game.c"
#include "game/continuar/continuar.c"

void game_loop()
{
    while (1)
    {
        switch(menu())
        {
            case 1:
                //continuar();
                break;

            case 2:
                //new_game();
                break;

            case 3:
                creditos();
                break;

            default:
                goto done;
        }
    }

    done:
    printf("\nDone!!\n\n");
}