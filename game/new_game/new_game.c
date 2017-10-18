#include "personagem.c"
#include "layout.c"
#include "history.c"
#include "select_raca.c"
#include "select_classe_humano.c"
#include "select_classe_sintozoide.c"
#include "create_personagem.c"

int new_game()
{
    PERSONAGEM personagem;

    init_personagem(&personagem);

    if (!history())
        return 0;

    else
    {
        switch(select_raca())
        {
            case 0:
                return 0;

            case 1:
                personagem.identidade.raca = RACA_HUMANO;
                goto select_classe_humano;

            case 2:
                personagem.identidade.raca = RACA_SINTOZOIDE;
                goto select_classe_sintozoide;
        }

        select_classe_humano:

        switch(select_classe_humano())
        {
            case 0:
                return 0;

            case 1:
                personagem.identidade.classe = CLASSE_SOLDADO;
                goto done;

            case 2:
                personagem.identidade.classe = CLASSE_CIENTISTA;
                goto done;
        }

        select_classe_sintozoide:

        switch(select_classe_sintozoide())
        {
            case 0:
                return 0;

            case 1:
                personagem.identidade.classe = CLASSE_ATIRADOR;
                goto done;

            case 2:
                personagem.identidade.classe = CLASSE_OPERARIO;
                goto done;
        }

        done:

        if (!create_personagem(&personagem))
            return 0;

        return 1;
    }

    return 0;
}