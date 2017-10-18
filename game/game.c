#include "menu.c"
#include "creditos.c"
#include "new_game/new_game.c"
#include "fases/fase.c"

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
                if (!new_game())
                    goto done;

                fase();

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