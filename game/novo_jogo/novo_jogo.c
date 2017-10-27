void novo_jogo()
{
    Personagem personagem;

    personagem_init(&personagem);

    personagem.identidade.raca = personagem_raca();

    if (personagem.identidade.raca == RACA_HUMANO)
        personagem.identidade.classe = personagem_classe_humano();

    if (personagem.identidade.raca == RACA_SINTOZOIDE)
        personagem.identidade.classe = personagem_classe_sintozoide() + 2;

    personagem_create(&personagem);

    return;
}