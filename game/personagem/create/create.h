void personagem_create(Personagem *personagem);

/** ----- Desenhos ----- **/

void personagem_create_draw_text(ALLEGRO_FONT **font, Personagem *personagem);
void personagem_create_draw_background(ALLEGRO_BITMAP **bitmap, ALLEGRO_COLOR bgcolor, int position[][4]);

/** ----- Eventos ----- **/

void personagem_create_event_mouse(ALLEGRO_EVENT_QUEUE **queue, ALLEGRO_COLOR *bgcolor, int position[][4]);
void personagem_create_event_mouse_hover(ALLEGRO_EVENT event, int position[][4], ALLEGRO_COLOR *bgcolor);

void personagem_create_event_keyboard_keyenter(ALLEGRO_EVENT_QUEUE **queue, int *escape);
void personagem_create_event_keyboard_keydown(ALLEGRO_EVENT_QUEUE **queue, Personagem *personagem, int escape);
