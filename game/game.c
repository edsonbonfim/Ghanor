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

int game()
{
    /** Menu **/
    while (1)
    {
        switch(menu())
        {
            case 1:
    
                break;
                
            case 2:
                novo_jogo();
                break;
    
            case 3:
                creditos();
                break;
    
            case 4:
                goto done;
        }
    }

    done:

    printf("\n\nDone.\n\n");
}
