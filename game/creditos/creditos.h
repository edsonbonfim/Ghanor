void creditos(ALLEGRO_DISPLAY *display, int *option);

/** ----- Desenhos ----- **/

void creditos_draw_text(ALLEGRO_DISPLAY *display, ALLEGRO_FONT **font);
void creditos_draw_background(ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP **bitmap);

/** ----- Eventos ----- **/

// Teclado
int creditos_event_keyboard(ALLEGRO_EVENT_QUEUE **queue);
