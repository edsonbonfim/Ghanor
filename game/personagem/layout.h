void personagem_layout_draw_background(int x1, int y1, int x2, int y2, ALLEGRO_COLOR bgcolor);
void personagem_layout_draw_text(int x, int y1, int y2, ALLEGRO_FONT **font, ALLEGRO_COLOR color, char **text);

int personagem_layout_event_mouse_get_opt(ALLEGRO_EVENT event);
void personagem_layout_event_mouse_hover(ALLEGRO_EVENT_QUEUE **queue, int *option);
int personagem_layout_event_mouse_click(ALLEGRO_EVENT_QUEUE **queue, int *option);
