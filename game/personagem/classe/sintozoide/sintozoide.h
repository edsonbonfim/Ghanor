enum { BITMAP_ATIRADOR = 1, BITMAP_OPERARIO };

int personagem_classe_sintozoide();

void personagem_classe_sintozoide_draw_background(ALLEGRO_BITMAP **bitmap);

void personagem_classe_sintozoide_atirador_draw_background(ALLEGRO_BITMAP **bitmap, int option);
void personagem_classe_sintozoide_atirador_draw_text(ALLEGRO_FONT **font, int option);

void personagem_classe_sintozoide_operario_draw_background(ALLEGRO_BITMAP **bitmap, int option);
void personagem_classe_sintozoide_operario_draw_text(ALLEGRO_FONT **font, int option);
