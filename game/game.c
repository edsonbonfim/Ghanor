/**typedef struct
{
    bool sair;

    bool tecla[ALLEGRO_KEY_MAX];

    float jogador_x, jogador_y;
    int dir;

    ALLEGRO_DISPLAY *janela;

    float altura_janela, largura_janela;
} Jogo;

Jogo novo_jogo(void)
{
    Jogo *jogo = calloc(1, sizeof *jogo);

    jogo->janela = al_create_display(640, 480);
}

void logica_do_jogo(Jogo *jogo)
{
    if (jogo->tecla[ALLEGRO_KEY_SCAPE])
        jogo->sair = true;

    if (jogo->tecla[ALLEGRO_KEY_LEFT])
        jogo->dir = 0;

    if (jogo->tecla[ALLEGRO_KEY_RIGHT])
        jogo->dir = 1;

    if (jogo->tecla[ALLEGRO_KEY_UP])
        jogo->dir = 2;

    if (jogo->tecla[ALLEGRO_KEY_DOWN])
        jogo->dir = 3;

    if (jogo->dir == 0)
        jogo->jogador_x -= 1;

    if (jogo->dir == 1)
        jogo->jogador_x += 1;

    if (jogo->dir == 2)
        jogo->jogador_y -= 1;

    if (jogo->dir == 3)
        jogo->jogador_y += 1;

    if (jogo->jogador_x < 0)
        jogo->jogador_x = 0;

    if (jogo->jogador_x > al_get_display_width(jogo->janela))
        jogo->jogador_x = al_get_display_width(jogo->janela);

    if (jogo->jogador_y < 0)
        jogo->jogador_y = 0;

    if (jogo->jogador_y > al_get_display_height(jogo->janela))
        jogo->jogador_y = al_get_display_height(jogo->janela);
}

void renderizar_jogo(Jogo *jogo)
{
    al_clear_to_color(al_map_rgb_f(0, 0, 0));
    al_draw_bitmap(jogo->imagem_de_fundo, 0, 0, 0);
    al_flip_display();
}
**/

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
