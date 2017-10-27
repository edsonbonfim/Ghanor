enum { BITMAP_SOLDADO = 1, BITMAP_CIENTISTA };

int personagem_classe_humano();

void personagem_classe_humano_draw_background(ALLEGRO_BITMAP **bitmap);

void personagem_classe_humano_soldado_draw_background(ALLEGRO_BITMAP **bitmap, int option);
void personagem_classe_humano_soldado_draw_text(ALLEGRO_FONT **font, int option);

void personagem_classe_humano_cientista_draw_background(ALLEGRO_BITMAP **bitmap, int option);
void personagem_classe_humano_cientista_draw_text(ALLEGRO_FONT **font, int option);
