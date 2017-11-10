enum { MENU_CONTINUAR = -4, MENU_NOVO_JOGO = -3, MENU_CREDITOS = -2, MENU_SAIR = -1 };

int menu(ALLEGRO_DISPLAY *display, int *option);

/** ----- Desenhos ----- **/

void menu_draw_text(ALLEGRO_DISPLAY *display, ALLEGRO_FONT **font, int *option);
void menu_draw_background(ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP **bitmap);

/** ----- Eventos ----- **/

// Mouse
int menu_event_mouse_hover_get_opt(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT event);
int menu_event_mouse_hover(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE **queue, int *option);
int menu_event_mouse_click(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE **queue, int *option);

// Teclado
void menu_event_keyboard_keydown_get_opt(ALLEGRO_EVENT event, int *option);
int menu_event_keyboard_keydown(ALLEGRO_EVENT_QUEUE **queue, int *option);
int menu_event_keyboard_keyenter(ALLEGRO_EVENT_QUEUE **queue, int *option);
