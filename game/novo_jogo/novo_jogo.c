void novo_jogo(ALLEGRO_DISPLAY *display, int *option)
{
    Personagem personagem;

    personagem_init(&personagem);

    personagem.identidade.raca = personagem_raca(display, option);

    if (*option == MENU_SAIR)
        return;

    if (personagem.identidade.raca == RACA_HUMANO)
        personagem.identidade.classe = personagem_classe_humano();

    if (personagem.identidade.raca == RACA_SINTOZOIDE)
        personagem.identidade.classe = personagem_classe_sintozoide() + 2;

    personagem_create(&personagem);
}
