ALLEGRO_EVENT_QUEUE *create_event_queue()
{
    ALLEGRO_EVENT_QUEUE *queue;

    if (!(queue = al_create_event_queue()))
    {
        fprintf(stderr, "Falha ao criar fila de evento.\n");
        return NULL;
    }

    return queue;
}

ALLEGRO_FONT *load_font(char * filename, int size, int flag)
{
    ALLEGRO_FONT *font;

    if (!(font = al_load_ttf_font(filename, size, flag)))
    {
        fprintf(stderr, "Falha ao carregar fonte %s\n", filename);
        exit(-1);
    }

    return font;
}

ALLEGRO_BITMAP *load_bitmap(char * filename)
{
    ALLEGRO_BITMAP *bitmap;

    if (!(bitmap = al_load_bitmap(filename)))
    {
        fprintf(stderr, "Falha ao carregar bitmap %s\n", filename);
        exit(-1);
    }

    return bitmap;
}

ALLEGRO_BITMAP *load_bitmap_at_size(char *filename, int w, int h)
{
    ALLEGRO_BITMAP *resized_bmp, *loaded_bmp, *prev_target;

    resized_bmp = al_create_bitmap(w, h);
    if (!resized_bmp)
        return NULL;

    loaded_bmp = load_bitmap(filename);

    prev_target = al_get_target_bitmap();
    al_set_target_bitmap(resized_bmp);

    al_draw_scaled_bitmap(loaded_bmp,
                          0, 0,                            
                          al_get_bitmap_width(loaded_bmp), 
                          al_get_bitmap_height(loaded_bmp),
                          0, 0,                            
                          w, h,                            
                          0);

    al_set_target_bitmap(prev_target);

    return resized_bmp;
}

int get_mouse_position(ALLEGRO_EVENT event, int x1, int y1, int x2, int y2)
{
    if (event.mouse.x > x1 && event.mouse.x < x2 
    &&  event.mouse.y > y1 && event.mouse.y < y2)
    {
        return 1;
    }

    return 0;
}

int contar_numero_de_personagens()
{
    FILE * file = fopen("data/files/count.data", "r");

    if (file == NULL)
    {
        perror("Falha ao abrir o arquivo data/files/count.txt");
        exit(0);
    }

    int qnt;

    fscanf(file, "%d", &qnt);
    fclose(file);

    return qnt;
}

void atualizar_numero_de_personagens(int qnt)
{
    FILE * file = fopen("data/files/count.txt", "w");

    if (file == NULL)
    {
        perror("Falha ao abrir o arquivo data/files/count.txt");
        exit(0);
    }

    fprintf(file, "%d", qnt);
    fclose(file);
}

int count()
{
    FILE * file = fopen("data/files/count.data", "r");

    if (file == NULL)
    {
        perror("Falha ao abrir o arquivo data/files/count.txt");
        exit(0);
    }

    int qnt;

    fscanf(file, "%d", &qnt);
    fclose(file);

    return qnt;
}

void upcount(int qnt)
{
    FILE * file = fopen("data/files/count.txt", "w");

    if (file == NULL)
    {
        perror("Falha ao abrir o arquivo data/files/count.txt");
        exit(0);
    }

    fprintf(file, "%d", qnt);
    fclose(file);
}