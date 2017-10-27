int menu();

void menu_draw_text(ALLEGRO_FONT **font, int *option);
void menu_draw_background(ALLEGRO_BITMAP **bitmap);

int menu_event_mouse_hover_get_opt(ALLEGRO_EVENT event);
int menu_event_mouse_hover(ALLEGRO_EVENT_QUEUE **queue, int *option);
int menu_event_mouse_click(ALLEGRO_EVENT_QUEUE **queue, int *option);

void menu_event_keyboard_keydown_get_opt(ALLEGRO_EVENT event, int *option);
int menu_event_keyboard_keydown(ALLEGRO_EVENT_QUEUE **queue, int *option);
int menu_event_keyboard_keyenter(ALLEGRO_EVENT_QUEUE **queue, int *option);
