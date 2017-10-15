int get_menu_select_option(ALLEGRO_EVENT ev);
int menu_mouse_click(ALLEGRO_EVENT_QUEUE *queue, int *qnt);
int menu_get_keyboard_opt(ALLEGRO_EVENT ev, int *qnt);
int menu_mouse_hover(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_FONT *font, int *qnt);

void draw_menu(ALLEGRO_FONT *h1, ALLEGRO_FONT *font);