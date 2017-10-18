struct print_text
{
    char text1[100];
    char text2[100];
    char text3[100];
    char text4[100];
};

int layout_menu_get_event_keyboard(ALLEGRO_EVENT_QUEUE *queue_keyboard);
int layout_menu_event_mouse_button_up(ALLEGRO_EVENT ev);
int layout_menu_get_event_mouse(ALLEGRO_EVENT_QUEUE *queue_mouse, int *hover_humano, int *hover_sintozoide);

void layout_menu_draw_box(int x1, int y1, int x2, int y2, ALLEGRO_COLOR bgcolor);
void layout_menu_event_mouse_axes(ALLEGRO_EVENT ev, int *hover_humano, int *hover_sintozoide);
void layout_menu_draw_text(int x, int y1, int y2, ALLEGRO_FONT *gtek, ALLEGRO_FONT *roboto, ALLEGRO_COLOR color, struct print_text text);