void novo_jogo(ALLEGRO_DISPLAY *display, int *option)
{
    Personagem personagem;

    personagem_init(&personagem);

    personagem.identidade.raca = personagem_raca(display, option);

    personagem_create(&personagem);
}
