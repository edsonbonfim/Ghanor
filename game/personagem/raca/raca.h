enum { BITMAP_HUMANO = 1, BITMAP_SINTOZOIDE };

int personagem_raca();

void personagem_raca_draw_background(ALLEGRO_BITMAP **bitmap);

void personagem_raca_humano_draw_background(ALLEGRO_BITMAP **bitmap, int *option);
void personagem_raca_humano_draw_text(ALLEGRO_FONT **font, int *option);

void personagem_raca_sintozoide_draw_background(ALLEGRO_BITMAP **bitmap, int *option);
void personagem_raca_sintozoide_draw_text(ALLEGRO_FONT **font, int *option);
