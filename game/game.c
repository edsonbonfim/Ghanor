#include "utils.c"
#include "menu.c"
#include "creditos.c"
#include "new_game.c"

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
                new_game();
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