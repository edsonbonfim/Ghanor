bool show_personagens;

int const PTS_ATRIBUTO = 30;

VIDEO *video;
EVENT_QUEUE *event_queue_personagens_keyboard_char;

enum raca
{
    ia = 1,
    humano
};
enum profissao
{
    soldado = 1,
    cientista,
    operario,
    atirador
};

bool get_keyboard_personagem_nome;

typedef struct identidade
{
    int id;
    char nome[17];
    int raca;
    int profissao;
} IDENTIDADE;

typedef struct influencia
{
    int humano;
    int ia;
} INFLUENCIA;

typedef struct atributo
{
    int hp;
    int xp;
    int forca;
    int agilidade;
    int inteligencia;
} ATRIBUTO;

typedef struct personagem
{
    IDENTIDADE identidade;
    INFLUENCIA influencia;
    ATRIBUTO atributo;
} PERSONAGEM;

void clrscr()
{
    system("@cls||clear");
}

void create_personagem(FILE *file, PERSONAGEM *personagem, int id)
{
    fprintf(file, "Id: %d\n", id);
    fprintf(file, "Nome: %s\n", personagem->identidade.nome);
    fprintf(file, "Raca: %d\n", personagem->identidade.raca);
    fprintf(file, "Profissao: %d\n", personagem->identidade.profissao);

    fprintf(file, "Influencia IA: %d\n", personagem->influencia.ia);
    fprintf(file, "Influencia Humano: %d\n", personagem->influencia.humano);

    fprintf(file, "HP: %d\n", personagem->atributo.hp);
    fprintf(file, "XP: %d\n", personagem->atributo.xp);
    fprintf(file, "Forca: %d\n", personagem->atributo.forca);
    fprintf(file, "Agilidade: %d\n", personagem->atributo.agilidade);
    fprintf(file, "Inteligencia: %d\n", personagem->atributo.inteligencia);
}

void edit_personagem(PERSONAGEM *personagem)
{
    printf("\n\nEDITAR:\n\n");

    printf("NOVO NOME: ");
    scanf(" %[^\n]", personagem->identidade.nome);

    char sFile[80];

    sprintf(sFile, "data/personagens/p%d.txt", personagem->identidade.id);

    FILE *file = fopen(sFile, "w");

    if (file == NULL)
    {
        perror("Falha ao criar o arquivotnagens.txt");
        exit(0);
    }

    create_personagem(file, personagem, personagem->identidade.id);

    fclose(file);
}

void delete_personagem(PERSONAGEM **personagem, int id)
{
    int qnt = count();

    char string[80], string2[80];

    for (int i = id; i < qnt; i++)
    {
        sprintf(string, "data/personagens/p%d.txt", i);
        sprintf(string2, "data/personagens/p%d.txt", i + 1);

        remove(string);
        rename(string2, string);

        FILE *file = fopen(string, "w");

        if (file == NULL)
        {
            perror("Falha ao criar o arquivot /data/personagens/***.txt");
            exit(0);
        }

        create_personagem(file, personagem[i], personagem[i]->identidade.id - 1);

        fclose(file);
    }

    sprintf(string, "data/personagens/p%d.txt", qnt);
    remove(string);

    upcount(qnt - 1);
}

PERSONAGEM **load_personagens()
{
    int i, qnt = count();

    PERSONAGEM **personagem = (PERSONAGEM **)malloc(qnt * sizeof(PERSONAGEM *));

    for (i = 0; i < qnt; i++)
    {
        personagem[i] = (PERSONAGEM *)malloc(sizeof(PERSONAGEM));
    }

    char sFile[20];

    for (i = 0; i < qnt; i++)
    {
        sprintf(sFile, "data/personagens/p%d.txt", i + 1);

        FILE *file = fopen(sFile, "r");

        if (file == NULL)
        {
            perror("Falha ao abrir o arquivo");
            exit(0);
        }

        fscanf(file, " Id: %d", &personagem[i]->identidade.id);
        fscanf(file, " Nome: %[^\n]", personagem[i]->identidade.nome);
        fscanf(file, " Raca: %d", &personagem[i]->identidade.raca);
        fscanf(file, " Profissao: %d", &personagem[i]->identidade.profissao);

        fscanf(file, " Influencia IA: %d\n", &personagem[i]->influencia.ia);
        fscanf(file, " Influencia Humano: %d\n", &personagem[i]->influencia.humano);

        fscanf(file, " HP: %d\n", &personagem[i]->atributo.hp);
        fscanf(file, " XP: %d\n", &personagem[i]->atributo.xp);
        fscanf(file, " Forca: %d\n", &personagem[i]->atributo.forca);
        fscanf(file, " Agilidade: %d\n", &personagem[i]->atributo.agilidade);
        fscanf(file, " Inteligencia: %d\n", &personagem[i]->atributo.inteligencia);

        fclose(file);
    }

    return personagem;
}

void print_personagem(PERSONAGEM *personagem)
{
    clrscr();

    printf("\nNome: %s\n", personagem->identidade.nome);

    switch (personagem->identidade.raca)
    {
    case ia:
        printf("Raca: IA\n");
        break;

    case humano:
        printf("Raca: IA\n");
        break;
    }

    switch (personagem->identidade.profissao)
    {
    case soldado:
        printf("Profissao: Soldado\n");
        break;

    case cientista:
        printf("Profissao: Cientista\n");
        break;

    case operario:
        printf("Profissao: Operario\n");
        break;

    case atirador:
        printf("Profissao: Atirador\n");
    }

    printf("Influencia IA: %d\n", personagem->influencia.ia);
    printf("Influencia Humano: %d\n", personagem->influencia.humano);
    printf("HP: %d\n", personagem->atributo.hp);
    printf("XP: %d\n", personagem->atributo.xp);
    printf("Forca: %d\n", personagem->atributo.forca);
    printf("Agilidade: %d\n", personagem->atributo.agilidade);
    printf("Inteligencia: %d\n", personagem->atributo.inteligencia);
}

void init_personagem(PERSONAGEM *personagem)
{
    personagem->identidade.id = count() + 1;
    personagem->identidade.nome[0] = '\0';
    personagem->identidade.raca = 0;
    personagem->identidade.profissao = 0;
}

void set_personagem_influencia(PERSONAGEM *personagem)
{
    srand(time(NULL));

    switch (personagem->identidade.raca)
    {
    case ia:
        personagem->influencia.humano = rand() % 20;
        personagem->influencia.ia = 30 + rand() % 20;
        break;

    case humano:
        personagem->influencia.humano = 30 + rand() % 20;
        personagem->influencia.ia = rand() % 20;
        break;
    }
}

void set_personagem_identidade_profissao_operario(PERSONAGEM *personagem)
{
    srand(time(NULL));

    personagem->atributo.inteligencia = rand() % PTS_ATRIBUTO;
    personagem->atributo.forca = 10 + rand() % (PTS_ATRIBUTO - 10);
    personagem->atributo.agilidade = 10 + rand() % (PTS_ATRIBUTO - 10);
}

void set_personagem_identidade_profissao_atirador(PERSONAGEM *personagem)
{
    srand(time(NULL));

    personagem->atributo.forca = rand() % PTS_ATRIBUTO;
    personagem->atributo.inteligencia = 10 + rand() % (PTS_ATRIBUTO - 10);
    personagem->atributo.agilidade = 10 + rand() % (PTS_ATRIBUTO - 10);
}

void set_personagem_identidade_profissao_soldado(PERSONAGEM *personagem)
{
    srand(time(NULL));

    personagem->atributo.inteligencia = rand() % PTS_ATRIBUTO;
    personagem->atributo.forca = 10 + rand() % (PTS_ATRIBUTO - 10);
    personagem->atributo.agilidade = 10 + rand() % (PTS_ATRIBUTO - 10);
}

void set_personagem_identidade_profissao_cientista(PERSONAGEM *personagem)
{
    srand(time(NULL));

    personagem->atributo.inteligencia = rand() % PTS_ATRIBUTO;
    personagem->atributo.agilidade = rand() % PTS_ATRIBUTO;
    personagem->atributo.forca = 15 + rand() % (PTS_ATRIBUTO - 15);
}

void set_personagem_atributo(PERSONAGEM *personagem)
{
    switch (personagem->identidade.profissao)
    {
    case operario:
        set_personagem_identidade_profissao_operario(personagem);
        break;

    case atirador:
        set_personagem_identidade_profissao_operario(personagem);
        break;

    case soldado:
        set_personagem_identidade_profissao_soldado(personagem);
        break;

    case cientista:
        set_personagem_identidade_profissao_cientista(personagem);
    }

    personagem->atributo.hp = 50;
    personagem->atributo.xp = 0;
}

void set_personagem()
{
    clrscr();

    PERSONAGEM *personagem = (PERSONAGEM *)malloc(sizeof(PERSONAGEM));

    //set_personagem_identidade(personagem);
    //set_personagem_influencia(personagem);
    //set_personagem_atributo(personagem);

    int qnt = count();

    char sFile[20];

    sprintf(sFile, "data/personagens/p%d.txt", qnt + 1);

    FILE *file = fopen(sFile, "w+");

    if (file == NULL)
    {
        perror("Falha ao criar o arquivotnagens.txt");
        exit(0);
    }

    create_personagem(file, personagem, personagem->identidade.id);

    fclose(file);

    upcount(qnt + 1);
}

void set_state_personagens_keyboard_char(PERSONAGEM *personagem, EVENT ev)
{
    if (ev.type == ALLEGRO_EVENT_KEY_CHAR)
    {
        if (strlen(personagem->identidade.nome) <= 16)
        {
            char temp[] = {ev.keyboard.unichar, '\0'};
            if (ev.keyboard.unichar == ' ')
            {
                strcat(personagem->identidade.nome, temp);
            }
            else if (ev.keyboard.unichar >= '0' &&
                     ev.keyboard.unichar <= '9')
            {
                strcat(personagem->identidade.nome, temp);
            }
            else if (ev.keyboard.unichar >= 'A' &&
                     ev.keyboard.unichar <= 'Z')
            {
                strcat(personagem->identidade.nome, temp);
            }
            else if (ev.keyboard.unichar >= 'a' &&
                     ev.keyboard.unichar <= 'z')
            {
                strcat(personagem->identidade.nome, temp);
            }
        }

        if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(personagem->identidade.nome) != 0)
        {
            personagem->identidade.nome[strlen(personagem->identidade.nome) - 1] = '\0';
        }
    }
}

void get_state_personagens_keyboard_char(PERSONAGEM *personagem)
{
    while (!al_is_event_queue_empty(event_queue_personagens_keyboard_char))
    {
        EVENT ev;

        al_wait_for_event(event_queue_personagens_keyboard_char, &ev);

        if (!get_keyboard_personagem_nome)
        {
            set_state_personagens_keyboard_char(personagem, ev);

            if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
            {
                get_keyboard_personagem_nome = true;
            }
        }
    }
}

void personagens_keyboard_char(PERSONAGEM *personagem)
{
    get_state_personagens_keyboard_char(personagem);

    //if (strlen(personagem->identidade.nome) > 0)
    //al_draw_text(paragraph, color, 160, 170, ALIGN_LEFT, personagem->identidade.nome);
}

int personagem()
{
    int redraw = 1;

    EVENT event_video;

    COLOR color = al_map_rgb(153, 153, 153);

    FONT *h1 = load_ttf_font("fonts/gtek-technology.ttf", 30, 0);
    FONT *font = load_ttf_font("fonts/gtek-technology.ttf", 20, 0);
    FONT *roboto = load_ttf_font("fonts/Roboto-Regular.ttf", 12, 0);

    VIDEO *video = al_open_video("videos/video.ogv");
    TIMER *timer = al_create_timer(1.0 / FPS);

    QUEUE *queue_video = create_event_queue();
    al_register_event_source(queue_video, al_get_video_event_source(video));
    al_register_event_source(queue_video, al_get_timer_event_source(timer));

    QUEUE *queue_mouse = create_event_queue();
    al_register_event_source(queue_mouse, al_get_mouse_event_source());

    QUEUE *queue_keyboard = create_event_queue();
    al_register_event_source(queue_keyboard, al_get_keyboard_event_source());

    al_start_video(video, al_get_default_mixer());
    al_start_timer(timer);

    while (1)
    {
        if (redraw && al_event_queue_is_empty(queue_video))
        {
            video_display(video);

            al_draw_filled_rectangle(0, 0, DISPLAY_W, DISPLAY_H, al_map_rgba(0, 0, 0, 100));

            

            // EVENTS

            while (!al_event_queue_is_empty(queue_keyboard))
            {
                EVENT ev;

                al_wait_for_event(queue_keyboard, &ev);

                if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
                    goto done;
            }

            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));

            redraw = false;
        }

        al_wait_for_event(queue_video, &event_video);

        if (event_video.type == ALLEGRO_EVENT_TIMER)
            redraw = true;
    }

done:

    al_destroy_font(h1);
    al_destroy_font(font);
    al_destroy_font(roboto);
    al_close_video(video);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue_video);
    al_destroy_event_queue(queue_keyboard);

    return 0;
}