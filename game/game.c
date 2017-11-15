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

#include "estagios/estagio1.c"

enum { BITMAP_PERSONAGEM_FRENTE };

void game(ALLEGRO_DISPLAY *display)
{
    /** Menu **/

    int option;

    while (1)
    {
        switch(menu(display, &option))
        {
            case MENU_CONTINUAR:
    
                break;
                
            case MENU_NOVO_JOGO:
                estagio1(display, &option);
                break;
    
            case MENU_CREDITOS:
                creditos(display, &option);
                break;
    
            case MENU_SAIR:
                return;
        }
    }
}