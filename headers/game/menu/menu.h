void draw_menu(FONT *h1, FONT *font);

int get_menu_select_option(EVENT ev);

int menu_get_keyboard_opt(EVENT ev, int *qnt);

void menu_mouse_hover(QUEUE *queue, FONT *font, int *qnt);

int menu_mouse_click(QUEUE *queue, int *qnt);

static void video_display(ALLEGRO_VIDEO *video);