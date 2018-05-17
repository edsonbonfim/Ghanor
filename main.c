#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_video.h>

ALLEGRO_DISPLAY *display;

ALLEGRO_COLOR COLOR_GRAY;
ALLEGRO_COLOR COLOR_WHITE;
ALLEGRO_COLOR COLOR_BLACK;

ALLEGRO_FONT *fontRoboto12;
ALLEGRO_FONT *fontGtek20;
ALLEGRO_FONT *fontGtek30;

ALLEGRO_EVENT_QUEUE *eventKeyEnter;

enum FONT
{
    FONT_GTEK,
    FONT_GTEK_TITLE,
    FONT_ROBOTO
};
enum BITMAP
{
    BITMAP_BACKGROUND
};
enum EVENT
{
    EVENT_MOUSE,
    EVENT_MOUSE_HOVER,
    EVENT_MOUSE_CLICK,
    EVENT_KEYBOARD_KEYDOWN,
    EVENT_KEYBOARD_KEYENTER
};
ALLEGRO_EVENT_QUEUE *event_queue_display_close = NULL;
ALLEGRO_EVENT_QUEUE *event_queue_display_resize = NULL;
int DISPLAY_W;
int DISPLAY_H;

/** ----- FUNCOES AUXILIARES ----- **/
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
ALLEGRO_FONT *load_font(char *filename, int size, int flag)
{
    ALLEGRO_FONT *font;

    if (!(font = al_load_ttf_font(filename, size, flag)))
    {
        fprintf(stderr, "Falha ao carregar fonte %s\n", filename);
        exit(-1);
    }

    return font;
}
ALLEGRO_BITMAP *load_bitmap(char *filename)
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
bool event_display_close(int *option)
{
    while (!al_is_event_queue_empty(event_queue_display_close))
    {
        ALLEGRO_EVENT event;

        al_wait_for_event(event_queue_display_close, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            *option = -1;
            return true;
        }

        return false;
    }
}
void event_display_resize()
{
    while (!al_is_event_queue_empty(event_queue_display_resize))
    {
        ALLEGRO_EVENT event;

        al_wait_for_event(event_queue_display_resize, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
            al_acknowledge_resize(event.display.source);
    }
}
int get_mouse_position(ALLEGRO_EVENT event, int x1, int y1, int x2, int y2)
{
    if (event.mouse.x > x1 && event.mouse.x < x2 && event.mouse.y > y1 && event.mouse.y < y2)
    {
        return 1;
    }

    return 0;
}
int contar_numero_de_personagens()
{
    FILE *file = fopen("Arquivos/count.txt", "r");

    if (file == NULL)
    {
        perror("Falha ao abrir o arquivo Arquivos/count.txt");
        exit(0);
    }

    int qnt;

    fscanf(file, "%d", &qnt);
    fclose(file);

    return qnt;
}
void atualizar_numero_de_personagens(int qnt)
{
    FILE *file = fopen("Arquivos/count.txt", "w");

    if (file == NULL)
    {
        perror("Falha ao abrir o arquivo Arquivos/count.txt");
        exit(0);
    }

    fprintf(file, "%d", qnt);
    fclose(file);
}
int count()
{
    FILE *file = fopen("Arquivos/count.txt", "r");

    if (file == NULL)
    {
        perror("Falha ao abrir o arquivo Arquivos/count.txt");
        exit(0);
    }

    int qnt;

    fscanf(file, "%d", &qnt);
    fclose(file);

    return qnt;
}
void upcount(int qnt)
{
    FILE *file = fopen("Arquivos/count.txt", "w");

    if (file == NULL)
    {
        perror("Falha ao abrir o arquivo Arquivos/count.txt");
        exit(0);
    }

    fprintf(file, "%d", qnt);
    fclose(file);
}

/** ----- VIDEO ----- **/
int redraw;
int FPS = 60;
ALLEGRO_EVENT event_video;
ALLEGRO_VIDEO *video = NULL;
ALLEGRO_TIMER *timer;
ALLEGRO_EVENT_QUEUE *queue_video;
void load_video(char *videoName)
{
    redraw = 1;

    video = al_open_video(videoName);
    timer = al_create_timer(1.0 / FPS);

    queue_video = create_event_queue();
    al_register_event_source(queue_video, al_get_video_event_source(video));
    al_register_event_source(queue_video, al_get_timer_event_source(timer));

    al_start_video(video, al_get_default_mixer());
    al_start_timer(timer);
}
void close_video()
{
    al_close_video(video);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue_video);
}
void g_event_video()
{
    al_wait_for_event(queue_video, &event_video);

    if (event_video.type == ALLEGRO_EVENT_TIMER)
        redraw = true;
}
static void video_display(ALLEGRO_VIDEO *video)
{
    int w, h, x, y;

    ALLEGRO_BITMAP *frame = al_get_video_frame(video);

    if (!frame)
        return;

    w = DISPLAY_W + 300;
    h = DISPLAY_H + 300;

    x = (DISPLAY_W - w) / 2;
    y = (DISPLAY_H - h) / 2;

    al_draw_scaled_bitmap(frame, 0, 0, al_get_bitmap_width(frame), al_get_bitmap_height(frame), x, y, w, h, 0);
    al_draw_filled_rectangle(0, 0, DISPLAY_W, DISPLAY_H, al_map_rgba(0, 0, 0, 170));
}

/** ----- JOGADOR ----- **/
int const PTS_ATRIBUTO = 30;
enum
{
    RACA_HUMANO = 1,
    RACA_SINTOZOIDE
};
enum
{
    CLASSE_SOLDADO = 1,
    CLASSE_ATIRADOR
};
typedef struct identidade
{
    int id;
    char nome[17];
    int raca;
    int classe;
} IDENTIDADE;
typedef struct influencia
{
    int humano;
    int sintozoide;
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
} Personagem;
void personagem_init(Personagem *personagem)
{
    personagem->identidade.id = contar_numero_de_personagens() + 1;
    personagem->identidade.nome[0] = '\0';
    personagem->identidade.raca = 0;
    personagem->identidade.classe = 0;
}
void save_personagem(FILE *file, Personagem *personagem, int id)
{
    fprintf(file, "Id: %d\n", id);
    fprintf(file, "Nome: %s\n", personagem->identidade.nome);
    fprintf(file, "Raca: %d\n", personagem->identidade.raca);
    fprintf(file, "Profissao: %d\n", personagem->identidade.classe);

    fprintf(file, "Influencia IA: %d\n", personagem->influencia.sintozoide);
    fprintf(file, "Influencia Humano: %d\n", personagem->influencia.humano);

    fprintf(file, "HP: %d\n", personagem->atributo.hp);
    fprintf(file, "XP: %d\n", personagem->atributo.xp);
    fprintf(file, "Forca: %d\n", personagem->atributo.forca);
    fprintf(file, "Agilidade: %d\n", personagem->atributo.agilidade);
    fprintf(file, "Inteligencia: %d\n", personagem->atributo.inteligencia);
}
void edit_personagem(Personagem *personagem)
{
    printf("\n\nEDITAR:\n\n");

    printf("NOVO NOME: ");
    scanf(" %[^\n]", personagem->identidade.nome);

    char sFile[80];

    sprintf(sFile, "Arquivos/p%d.txt", personagem->identidade.id);

    FILE *file = fopen(sFile, "w");

    if (file == NULL)
    {
        perror("Falha ao criar o arquivotnagens.txt");
        exit(0);
    }

    save_personagem(file, personagem, personagem->identidade.id);

    fclose(file);
}
void delete_personagem(Personagem **personagem, int id)
{
    int qnt = contar_numero_de_personagens();

    char string[80], string2[80];

    for (int i = id; i < qnt; i++)
    {
        sprintf(string, "data/personagens/p%d.txt", i);
        sprintf(string2, "Arquivos/p%d.txt", i + 1);

        remove(string);
        rename(string2, string);

        FILE *file = fopen(string, "w");

        if (file == NULL)
        {
            perror("Falha ao criar o arquivot /data/personagens/***.txt");
            exit(0);
        }

        save_personagem(file, personagem[i], personagem[i]->identidade.id - 1);

        fclose(file);
    }

    sprintf(string, "data/personagens/p%d.txt", qnt);
    remove(string);

    atualizar_numero_de_personagens(qnt - 1);
}
Personagem **load_personagens()
{
    int i, qnt = contar_numero_de_personagens();

    Personagem **personagem = (Personagem **)malloc(qnt * sizeof(Personagem *));

    for (i = 0; i < qnt; i++)
    {
        personagem[i] = (Personagem *)malloc(sizeof(Personagem));
    }

    char sFile[20];

    for (i = 0; i < qnt; i++)
    {
        sprintf(sFile, "Arquivos/p%d.txt", i + 1);

        FILE *file = fopen(sFile, "r");

        if (file == NULL)
        {
            perror("Falha ao abrir o arquivo");
            exit(0);
        }

        fscanf(file, " Id: %d", &personagem[i]->identidade.id);
        fscanf(file, " Nome: %[^\n]", personagem[i]->identidade.nome);
        fscanf(file, " Raca: %d", &personagem[i]->identidade.raca);
        fscanf(file, " Profissao: %d", &personagem[i]->identidade.classe);

        fscanf(file, " Influencia IA: %d\n", &personagem[i]->influencia.sintozoide);
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
void set_personagem_influencia(Personagem *personagem)
{
    srand(time(NULL));

    switch (personagem->identidade.raca)
    {
    case RACA_SINTOZOIDE:
        personagem->influencia.humano = rand() % 20;
        personagem->influencia.sintozoide = 30 + rand() % 20;
        break;

    case RACA_HUMANO:
        personagem->influencia.humano = 30 + rand() % 20;
        personagem->influencia.sintozoide = rand() % 20;
        break;
    }
}
void set_personagem_identidade_profissao_operario(Personagem *personagem)
{
    srand(time(NULL));

    personagem->atributo.inteligencia = rand() % PTS_ATRIBUTO;
    personagem->atributo.forca = 10 + rand() % (PTS_ATRIBUTO - 10);
    personagem->atributo.agilidade = 10 + rand() % (PTS_ATRIBUTO - 10);
}
void set_personagem_identidade_profissao_atirador(Personagem *personagem)
{
    srand(time(NULL));

    personagem->atributo.forca = rand() % PTS_ATRIBUTO;
    personagem->atributo.inteligencia = 10 + rand() % (PTS_ATRIBUTO - 10);
    personagem->atributo.agilidade = 10 + rand() % (PTS_ATRIBUTO - 10);
}
void set_personagem_identidade_profissao_soldado(Personagem *personagem)
{
    srand(time(NULL));

    personagem->atributo.inteligencia = rand() % PTS_ATRIBUTO;
    personagem->atributo.forca = 10 + rand() % (PTS_ATRIBUTO - 10);
    personagem->atributo.agilidade = 10 + rand() % (PTS_ATRIBUTO - 10);
}
void set_personagem_identidade_profissao_cientista(Personagem *personagem)
{
    srand(time(NULL));

    personagem->atributo.inteligencia = rand() % PTS_ATRIBUTO;
    personagem->atributo.agilidade = rand() % PTS_ATRIBUTO;
    personagem->atributo.forca = 15 + rand() % (PTS_ATRIBUTO - 15);
}
void set_personagem_atributo(Personagem *personagem)
{
    switch (personagem->identidade.classe)
    {
    case CLASSE_ATIRADOR:
        set_personagem_identidade_profissao_operario(personagem);
        break;

    case CLASSE_SOLDADO:
        set_personagem_identidade_profissao_soldado(personagem);
        break;
    }

    personagem->atributo.hp = 50;
    personagem->atributo.xp = 0;
}
void set_personagem(Personagem *personagem)
{
    set_personagem_atributo(personagem);
    set_personagem_influencia(personagem);

    int qnt = contar_numero_de_personagens();

    char sFile[20];

    sprintf(sFile, "Arquivos/p%d.txt", qnt + 1);

    FILE *file = fopen(sFile, "w+");

    if (file == NULL)
    {
        perror("Falha ao criar o arquivotnagens.txt");
        exit(0);
    }

    save_personagem(file, personagem, personagem->identidade.id);

    fclose(file);

    atualizar_numero_de_personagens(qnt + 1);
}

/** ----- RACA ----- **/
enum
{
    BITMAP_HUMANO = 1,
    BITMAP_SINTOZOIDE
};
void personagem_raca_humano_draw_background(ALLEGRO_BITMAP **bitmap, int *option)
{
    ALLEGRO_COLOR color_bg;
    ALLEGRO_COLOR color_font;
    ALLEGRO_COLOR color_bitmap;

    color_bg = al_map_rgba(0, 0, 0, 150);

    if (*option == 1)
        color_bitmap = al_map_rgb(255, 255, 255);

    else
        color_bitmap = al_map_rgb(100, 100, 100);

    int x1 = 100;
    int y1 = DISPLAY_H / 5 - 30;
    int x2 = DISPLAY_W / 2 - 50;
    int y2 = DISPLAY_H - y1;

    personagem_layout_draw_background(x1, y1, x2, y2, color_bg);

    int bitmap_width = al_get_bitmap_width(bitmap[BITMAP_HUMANO]);
    int bitmap_height = al_get_bitmap_height(bitmap[BITMAP_HUMANO]);

    int x = DISPLAY_W / 4 + 25 - bitmap_width / 2;
    int y = y2 - (bitmap_height / 2) - 275;

    al_draw_tinted_bitmap(bitmap[BITMAP_HUMANO], color_bitmap, x, y, 0);
}
void personagem_raca_humano_draw_text(ALLEGRO_FONT **font, int *option)
{
    ALLEGRO_COLOR color = (*option == 1) ? COLOR_WHITE : COLOR_GRAY;

    int x = DISPLAY_W / 4 + 25;
    int y1 = DISPLAY_H / 5;
    int y2 = DISPLAY_H - y1 + 25;

    char *text[] =
        {
            "humano",
            "SOLDADOS SUBSTITUÍDOS PELOS ATIRADORES. QUANDO AS",
            "IA's ASSUMIRAM O PODER, FORAM TOMADOS COMO HERÓIS.",
            "SE MOSTRARAM ESSENCIAIS PARA A LIBERTAÇÃO HUMANA"};

    personagem_layout_draw_text(x, y1, y2, font, color, text);
}
void personagem_raca_sintozoide_draw_background(ALLEGRO_BITMAP **bitmap, int *option)
{
    ALLEGRO_COLOR color_bg;
    ALLEGRO_COLOR color_font;
    ALLEGRO_COLOR color_bitmap;

    color_bg = al_map_rgba(0, 0, 0, 150);

    if (*option == 2)
        color_bitmap = al_map_rgb(255, 255, 255);

    else
        color_bitmap = al_map_rgb(100, 100, 100);

    int x1 = DISPLAY_W / 2 + 50;
    int y1 = DISPLAY_H / 5 - 30;
    int x2 = DISPLAY_W - 100;
    int y2 = DISPLAY_H - y1;

    personagem_layout_draw_background(x1, y1, x2, y2, color_bg);

    int bitmap_width = al_get_bitmap_width(bitmap[BITMAP_SINTOZOIDE]);
    int bitmap_height = al_get_bitmap_height(bitmap[BITMAP_SINTOZOIDE]);

    int x = (DISPLAY_W / 2) + (DISPLAY_W / 4) - 25 - bitmap_width / 2;
    int y = y2 - (bitmap_height / 2) - 275;

    al_draw_tinted_bitmap(bitmap[BITMAP_SINTOZOIDE], color_bitmap, x, y, 0);
}
void personagem_raca_sintozoide_draw_text(ALLEGRO_FONT **font, int *option)
{
    ALLEGRO_COLOR color = (*option == 2) ? COLOR_WHITE : COLOR_GRAY;

    int x = (DISPLAY_W / 2) + (DISPLAY_W / 4) - 25;
    int y1 = DISPLAY_H / 5;
    int y2 = DISPLAY_H - y1 + 25;

    char *text[] =
        {
            "sintozoide",
            "OPERÁRIOS ESPECIALISTAS EM MECÂNICA, SÃO OS ÚNICOS",
            "A CONHECER OS SEGREDOS DE TODAS AS TECNOLOGIAS DE",
            "GHANOR, INCLUSIVE DAS PRÓPRIAS INTELIGENCIAS ARTIFICIAIS"};

    personagem_layout_draw_text(x, y1, y2, font, color, text);
}
int personagem_raca(ALLEGRO_DISPLAY *display, int *option)
{
    /** ----- Fontes ----- **/

    ALLEGRO_FONT *font[] =
        {
                [FONT_GTEK] load_font("Fontes/gtek-technology.ttf", 25, 0),
                [FONT_ROBOTO] load_font("Fontes/Righteous-Regular.ttf", 12, 0)};

    /** ----- Bitmaps ----- **/

    ALLEGRO_BITMAP *bitmap[] =
        {
                [BITMAP_HUMANO] load_bitmap("Imagens/atirador.png"),
                [BITMAP_SINTOZOIDE] load_bitmap("Imagens/operario.png"),
                [BITMAP_BACKGROUND] load_bitmap_at_size("Imagens/bg2.jpeg", DISPLAY_W, DISPLAY_H)};

    /** ----- Fila de Eventos ----- **/

    ALLEGRO_EVENT_QUEUE *queue[] =
        {
                [EVENT_MOUSE_HOVER] create_event_queue(),
                [EVENT_MOUSE_CLICK] create_event_queue()};

    al_register_event_source(queue[EVENT_MOUSE_HOVER], al_get_mouse_event_source());
    al_register_event_source(queue[EVENT_MOUSE_CLICK], al_get_mouse_event_source());

    /** ----- Loop ----- **/

    while (1)
    {
        if (redraw && al_event_queue_is_empty(queue_video))
        {
            video_display(video);

            redraw = 0;

            /** ----- Desenhos ----- **/

            //personagem_raca_draw_background(bitmap);

            personagem_raca_humano_draw_background(bitmap, option);
            personagem_raca_sintozoide_draw_background(bitmap, option);

            personagem_raca_humano_draw_text(font, option);
            personagem_raca_sintozoide_draw_text(font, option);

            al_flip_display();

            /** ----- Eventos ----- **/

            event_display_resize();

            personagem_layout_event_mouse_hover(queue, option);

            if (personagem_layout_event_mouse_click(queue, option) || event_display_close(option))
                break;
        }

        g_event_video();
    }

    al_destroy_font(font[FONT_GTEK]);
    al_destroy_font(font[FONT_ROBOTO]);

    al_destroy_bitmap(bitmap[BITMAP_HUMANO]);
    al_destroy_bitmap(bitmap[BITMAP_SINTOZOIDE]);
    al_destroy_bitmap(bitmap[BITMAP_BACKGROUND]);

    al_destroy_event_queue(queue[EVENT_MOUSE_HOVER]);
    al_destroy_event_queue(queue[EVENT_MOUSE_CLICK]);

    return *option;
}

/** ----- CRIAR JOGADOR LAYOUT ----- **/
void personagem_layout_draw_background(int x1, int y1, int x2, int y2, ALLEGRO_COLOR bgcolor)
{
    x1 = x1 - 30;
    x2 = x2 + 30;
    al_draw_filled_rectangle(x1, y1, x2, y2, bgcolor);
}
void personagem_layout_draw_text(int x, int y1, int y2, ALLEGRO_FONT **font, ALLEGRO_COLOR color, char **text)
{
    al_draw_text(font[FONT_GTEK], color, x, y1 + 10, ALLEGRO_ALIGN_CENTER, text[0]);
    al_draw_text(font[FONT_ROBOTO], color, x, y2 - 110, ALLEGRO_ALIGN_CENTER, text[1]);
    al_draw_text(font[FONT_ROBOTO], color, x, y2 - 80, ALLEGRO_ALIGN_CENTER, text[2]);
    al_draw_text(font[FONT_ROBOTO], color, x, y2 - 50, ALLEGRO_ALIGN_CENTER, text[3]);
}
int personagem_layout_event_mouse_get_opt(ALLEGRO_EVENT event)
{
    int x1 = DISPLAY_W / 6;
    int y1 = DISPLAY_H / 5;
    int x2 = DISPLAY_W / 2.1;
    int y2 = DISPLAY_H - y1;

    if (get_mouse_position(event, x1, y1, x2, y2))
        return 1;

    x1 = DISPLAY_W / 1.9;
    x2 = DISPLAY_W - DISPLAY_W / 6;

    if (get_mouse_position(event, x1, y1, x2, y2))
        return 2;

    return 0;
}
void personagem_layout_event_mouse_hover(ALLEGRO_EVENT_QUEUE **queue, int *option)
{
    while (!al_is_event_queue_empty(queue[EVENT_MOUSE_HOVER]))
    {
        ALLEGRO_EVENT event;

        al_wait_for_event(queue[EVENT_MOUSE_HOVER], &event);

        if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
            *option = personagem_layout_event_mouse_get_opt(event);
    }
}
int personagem_layout_event_mouse_click(ALLEGRO_EVENT_QUEUE **queue, int *option)
{
    while (!al_is_event_queue_empty(queue[EVENT_MOUSE_CLICK]))
    {
        ALLEGRO_EVENT event;

        al_wait_for_event(queue[EVENT_MOUSE_CLICK], &event);

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
        {
            *option = personagem_layout_event_mouse_get_opt(event);
            return 1;
        }
    }
}

/** ----- CRIAR JOGADOR ----- **/
void personagem_create_draw_background(ALLEGRO_BITMAP **bitmap, ALLEGRO_COLOR bgcolor, int position[][4])
{
    int x1 = DISPLAY_W / 3;
    int y1 = DISPLAY_H / 5;
    int x2 = DISPLAY_W - x1;
    int y2 = DISPLAY_H - y1;
    personagem_layout_draw_background(x1, y1, x2, y2, al_map_rgba(0, 0, 0, 150));

    // Retangulo aonde o nome sera digitado
    position[0][0] = x1 + 50;
    position[0][1] = y1 + 260;
    position[0][2] = x2 - 50;
    position[0][3] = y1 + 293;
    al_draw_rectangle(position[0][0], position[0][1], position[0][2], position[0][3], COLOR_GRAY, 1);

    // Retangulo do botao para continuar
    position[1][0] = x1 + 50;
    position[1][1] = y2 - 63;
    position[1][2] = x2 - 50;
    position[1][3] = y2 - 30;
    al_draw_rectangle(position[1][0], position[1][1], position[1][2], position[1][3], bgcolor, 1);
}
void personagem_create_draw_text(ALLEGRO_FONT **font, Personagem *personagem)
{
    int x1 = DISPLAY_W / 2;
    int x2 = DISPLAY_W / 3;
    int y1 = DISPLAY_H / 5;
    int y2 = DISPLAY_H - y1;

    al_draw_text(font[FONT_GTEK], COLOR_WHITE, x1, y1 + 50, ALLEGRO_ALIGN_CENTER, "criar personagem");

    al_draw_text(font[FONT_ROBOTO], COLOR_WHITE, x1, y1 + 130, ALLEGRO_ALIGN_CENTER, "VOCE ESTA QUASE PRONTO PARA SE");
    al_draw_text(font[FONT_ROBOTO], COLOR_WHITE, x1, y1 + 160, ALLEGRO_ALIGN_CENTER, "AVENTURAR NAS TERRAS DE GHANOR,");
    al_draw_text(font[FONT_ROBOTO], COLOR_WHITE, x1, y1 + 210, ALLEGRO_ALIGN_CENTER, "PRECISAMOS SABER QUAL O SEU NOME");

    y1 = y1 + 150;

    al_draw_text(font[FONT_ROBOTO], COLOR_WHITE, x2 + 80, y1 + 120, ALLEGRO_ALIGN_CENTER, "NOME: ");
    al_draw_text(font[FONT_ROBOTO], COLOR_WHITE, DISPLAY_W / 2, y2 - 53, ALLEGRO_ALIGN_CENTER, "PRONTO");

    if (strlen(personagem->identidade.nome) > 0)
        al_draw_text(font[FONT_ROBOTO], COLOR_WHITE, x1, y1 + 120, ALLEGRO_ALIGN_CENTER, personagem->identidade.nome);
}
void personagem_create_event_keyboard_keydown(ALLEGRO_EVENT_QUEUE **queue, Personagem *personagem, int escape)
{
    while (!al_is_event_queue_empty(queue[EVENT_KEYBOARD_KEYDOWN]))
    {
        ALLEGRO_EVENT event;

        al_wait_for_event(queue[EVENT_KEYBOARD_KEYDOWN], &event);

        if (!escape)
        {
            if (event.type == ALLEGRO_EVENT_KEY_CHAR)
            {
                if (strlen(personagem->identidade.nome) <= 16)
                {
                    char temp[] = {event.keyboard.unichar, '\0'};

                    if ((event.keyboard.unichar >= 'A' && event.keyboard.unichar <= 'Z') || (event.keyboard.unichar >= 'a' && event.keyboard.unichar <= 'z') || (event.keyboard.unichar == ' '))
                    {
                        strcat(personagem->identidade.nome, temp);
                    }
                }

                if (event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(personagem->identidade.nome) != 0)
                {
                    personagem->identidade.nome[strlen(personagem->identidade.nome) - 1] = '\0';
                }
            }
        }
    }
}
void personagem_create_event_keyboard_keyenter(ALLEGRO_EVENT_QUEUE **queue, int *escape)
{
    while (!al_is_event_queue_empty(queue[EVENT_KEYBOARD_KEYENTER]))
    {
        ALLEGRO_EVENT event;

        al_wait_for_event(queue[EVENT_KEYBOARD_KEYENTER], &event);

        if (event.keyboard.keycode == ALLEGRO_KEY_ENTER)
            *escape = 1;
    }
}
void personagem_create_event_mouse(ALLEGRO_EVENT_QUEUE **queue, ALLEGRO_COLOR *bgcolor, int position[][4], int *escape)
{
    while (!al_is_event_queue_empty(queue[EVENT_MOUSE]))
    {
        ALLEGRO_EVENT event;

        al_wait_for_event(queue[EVENT_MOUSE], &event);

        if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
            personagem_create_event_mouse_hover(event, position, bgcolor);

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
            personagem_create_event_mouse_click(event, position, escape);
    }
}

void personagem_create_event_mouse_hover(ALLEGRO_EVENT event, int position[][4], ALLEGRO_COLOR *bgcolor)
{
    int x1 = position[1][0];
    int y1 = position[1][1];
    int x2 = position[1][2];
    int y2 = position[1][3];

    *bgcolor = (get_mouse_position(event, x1, y1, x2, y2)) ? COLOR_WHITE : COLOR_GRAY;
}

void personagem_create_event_mouse_click(ALLEGRO_EVENT event, int position[][4], int *escape)
{
    int x1 = position[1][0];
    int y1 = position[1][1];
    int x2 = position[1][2];
    int y2 = position[1][3];

    *escape = (get_mouse_position(event, x1, y1, x2, y2)) ? 1 : 0;
}
void personagem_create(Personagem *personagem)
{
    /** ----- Fontes ----- **/

    ALLEGRO_FONT *font[] =
        {
                [FONT_GTEK] load_font("Fontes/gtek-technology.ttf", 20, 0),
                [FONT_ROBOTO] load_font("Fontes/Righteous-Regular.ttf", 12, 0)};

    /** ----- Bitmaps ----- **/

    ALLEGRO_BITMAP *bitmap[] =
        {
                [BITMAP_BACKGROUND] load_bitmap_at_size("Imagens/bg2.jpeg", DISPLAY_W, DISPLAY_H)};

    /** ----- Fila de Eventos ----- **/

    ALLEGRO_EVENT_QUEUE *queue[] =
        {
                [EVENT_MOUSE] create_event_queue(),

                [EVENT_KEYBOARD_KEYDOWN] create_event_queue(),
                [EVENT_KEYBOARD_KEYENTER] create_event_queue()};

    al_register_event_source(queue[EVENT_MOUSE], al_get_mouse_event_source());

    al_register_event_source(queue[EVENT_KEYBOARD_KEYDOWN], al_get_keyboard_event_source());
    al_register_event_source(queue[EVENT_KEYBOARD_KEYENTER], al_get_keyboard_event_source());

    /** ----- Variaveis Auxiliares ----- **/

    int escape = 0;
    int position[2][4];

    ALLEGRO_COLOR bgcolor = COLOR_GRAY;

    /** ----- Loop ----- **/

    while (1)
    {
        if (redraw && al_event_queue_is_empty(queue_video))
        {
            video_display(video);

            redraw = 0;

            personagem_create_draw_background(bitmap, bgcolor, position);
            personagem_create_draw_text(font, personagem);

            al_flip_display();

            personagem_create_event_mouse(queue, &bgcolor, position, &escape);

            personagem_create_event_keyboard_keydown(queue, personagem, escape);
            personagem_create_event_keyboard_keyenter(queue, &escape);

            if (escape)
                break;
        }

        g_event_video();
    }

    al_destroy_font(font[FONT_GTEK]);
    al_destroy_font(font[FONT_ROBOTO]);

    al_destroy_bitmap(bitmap[BITMAP_BACKGROUND]);

    al_destroy_event_queue(queue[EVENT_MOUSE]);

    al_destroy_event_queue(queue[EVENT_KEYBOARD_KEYDOWN]);
    al_destroy_event_queue(queue[EVENT_KEYBOARD_KEYENTER]);
}

/** ----- Menu ----- **/
enum
{
    MENU_CONTINUAR = -4,
    MENU_NOVO_JOGO = -3,
    MENU_CREDITOS = -2,
    MENU_SAIR = -1
};
void menu_draw_text(ALLEGRO_DISPLAY *display, ALLEGRO_FONT **font, int *option)
{
    int x = al_get_display_width(display) / 2;
    int y = al_get_display_height(display) / 2;

    ALLEGRO_FONT *title = font[FONT_GTEK_TITLE];
    ALLEGRO_FONT *options = font[FONT_GTEK];

    int numero_de_personagens = contar_numero_de_personagens();

    ALLEGRO_COLOR continuar = (!numero_de_personagens) ? al_map_rgb(50, 50, 50) : COLOR_GRAY;

    al_draw_text(title, COLOR_WHITE, x, y - 150, ALLEGRO_ALIGN_CENTER, "ghanor");
    al_draw_text(title, COLOR_WHITE, x, y - 100, ALLEGRO_ALIGN_CENTER, "the revolution");
    //al_draw_text(options, continuar, x, y - 40, ALLEGRO_ALIGN_CENTER, "continuar");
    al_draw_text(options, COLOR_WHITE, x, y + 10, ALLEGRO_ALIGN_CENTER, "novo jogo");
    al_draw_text(options, COLOR_WHITE, x, y + 60, ALLEGRO_ALIGN_CENTER, "creditos");
    al_draw_text(options, COLOR_WHITE, x, y + 110, ALLEGRO_ALIGN_CENTER, "sair");

    switch (*option)
    {
    case MENU_CONTINUAR:
        if (!numero_de_personagens)
            break;

        //al_draw_text(options, COLOR_WHITE, x + 1, y - 39, ALLEGRO_ALIGN_CENTER, "continuar");
        break;

    case MENU_NOVO_JOGO:
        al_draw_text(options, COLOR_WHITE, x + 1, y + 11, ALLEGRO_ALIGN_CENTER, "novo jogo");
        break;

    case MENU_CREDITOS:
        al_draw_text(options, COLOR_WHITE, x + 1, y + 61, ALLEGRO_ALIGN_CENTER, "creditos");
        break;

    case MENU_SAIR:
        al_draw_text(options, COLOR_WHITE, x + 1, y + 111, ALLEGRO_ALIGN_CENTER, "sair");
    }
}
int menu_event_mouse_hover_get_opt(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT event)
{
    int x = al_get_display_width(display) / 2;
    int y = al_get_display_height(display) / 2;

    if (get_mouse_position(event, x - 70, y - 40, x + 70, y - 20))
        return MENU_CONTINUAR;

    if (get_mouse_position(event, x - 75, y + 10, x + 75, y + 30))
        return MENU_NOVO_JOGO;

    if (get_mouse_position(event, x - 65, y + 60, x + 65, y + 80))
        return MENU_CREDITOS;

    if (get_mouse_position(event, x - 30, y + 110, x + 30, y + 130))
        return MENU_SAIR;

    return 0;
}
int menu_event_mouse_hover(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE **queue, int *option)
{
    while (!al_is_event_queue_empty(queue[EVENT_MOUSE_HOVER]))
    {
        ALLEGRO_EVENT event;

        al_wait_for_event(queue[EVENT_MOUSE_HOVER], &event);

        if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
            *option = menu_event_mouse_hover_get_opt(display, event);
    }
}
int menu_event_mouse_click(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE **queue, int *option)
{
    while (!al_is_event_queue_empty(queue[EVENT_MOUSE_CLICK]))
    {
        ALLEGRO_EVENT event;

        al_wait_for_event(queue[EVENT_MOUSE_CLICK], &event);

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
        {
            *option = menu_event_mouse_hover_get_opt(display, event);
            return *option;
        }
    }
}
int menu_event_keyboard_keydown(ALLEGRO_EVENT_QUEUE **queue, int *option)
{
    while (!al_is_event_queue_empty(queue[EVENT_KEYBOARD_KEYDOWN]))
    {
        ALLEGRO_EVENT event;

        al_wait_for_event(queue[EVENT_KEYBOARD_KEYDOWN], &event);

        if (event.type == ALLEGRO_EVENT_KEY_DOWN)
            menu_event_keyboard_keydown_get_opt(event, option);
    }

    return MENU_CONTINUAR;
}
void menu_event_keyboard_keydown_get_opt(ALLEGRO_EVENT event, int *option)
{
    if (*option == MENU_SAIR + 1)
        *option = (contar_numero_de_personagens()) ? MENU_CONTINUAR : MENU_NOVO_JOGO;

    else if (*option == MENU_CONTINUAR - 1 || (*option == MENU_CONTINUAR && !contar_numero_de_personagens()))
        *option = MENU_SAIR;

    else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
        (*option)++;

    else if (event.keyboard.keycode == ALLEGRO_KEY_UP)
        (*option)--;
}
int menu_event_keyboard_keyenter(ALLEGRO_EVENT_QUEUE **queue, int *option)
{
    while (!al_is_event_queue_empty(queue[EVENT_KEYBOARD_KEYENTER]))
    {
        ALLEGRO_EVENT event;

        al_wait_for_event(queue[EVENT_KEYBOARD_KEYENTER], &event);

        if (event.keyboard.keycode == ALLEGRO_KEY_ENTER)
            return 1;
    }

    return 0;
}
int menu(ALLEGRO_DISPLAY *display, int *option)
{
    if (*option == MENU_SAIR)
        return *option;

    *option = (contar_numero_de_personagens()) ? MENU_CONTINUAR : MENU_NOVO_JOGO;

    /** ----- Fontes ----- **/

    ALLEGRO_FONT *font[] =
        {
                [FONT_GTEK] load_font("Fontes/gtek-technology.ttf", 20, 0),
                [FONT_GTEK_TITLE] load_font("Fontes/gtek-technology.ttf", 30, 0)};

    /** ----- Bitmaps ----- **/

    ALLEGRO_BITMAP *bitmap[] =
        {
                [BITMAP_BACKGROUND] load_bitmap_at_size("Imagens/bg.jpeg", DISPLAY_W, DISPLAY_H)};

    /** ----- Fila de Eventos ----- **/

    ALLEGRO_EVENT_QUEUE *queue[] =
        {
                [EVENT_MOUSE_HOVER] create_event_queue(),
                [EVENT_MOUSE_CLICK] create_event_queue(),
                [EVENT_KEYBOARD_KEYDOWN] create_event_queue(),
                [EVENT_KEYBOARD_KEYENTER] create_event_queue()};

    al_register_event_source(queue[EVENT_MOUSE_HOVER], al_get_mouse_event_source());
    al_register_event_source(queue[EVENT_MOUSE_CLICK], al_get_mouse_event_source());

    al_register_event_source(queue[EVENT_KEYBOARD_KEYDOWN], al_get_keyboard_event_source());
    al_register_event_source(queue[EVENT_KEYBOARD_KEYENTER], al_get_keyboard_event_source());

    /** ----- Video ----- **/
    if (!video)
        load_video("Videos/video.ogv");

    /** ----- Loop ----- **/

    while (1)
    {
        if (redraw && al_event_queue_is_empty(queue_video))
        {
            video_display(video);

            redraw = 0;

            menu_draw_text(display, font, option);

            al_flip_display();

            event_display_resize();

            menu_event_mouse_hover(display, queue, option);
            menu_event_keyboard_keydown(queue, option);

            if (menu_event_mouse_click(display, queue, option) || menu_event_keyboard_keyenter(queue, option) || event_display_close(option))
                break;
        }

        g_event_video();
    }

    al_destroy_font(font[FONT_GTEK]);
    al_destroy_font(font[FONT_GTEK_TITLE]);

    al_destroy_bitmap(BITMAP_BACKGROUND);

    al_destroy_event_queue(queue[EVENT_MOUSE_CLICK]);
    al_destroy_event_queue(queue[EVENT_MOUSE_HOVER]);
    al_destroy_event_queue(queue[EVENT_KEYBOARD_KEYDOWN]);
    al_destroy_event_queue(queue[EVENT_KEYBOARD_KEYENTER]);

    return *option;
}

/** ----- Estagio 1 ----- **/
int contar_linhas(FILE *arquivo)
{
    char c;

    rewind(arquivo);

    int count = 1;

    while (!feof(arquivo))
    {
        c = fgetc(arquivo);

        if (c == '\n')
            count++;
    }

    rewind(arquivo);

    return count;
}

void estagio1_draw_background(ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP **bitmap)
{
    int display_width = al_get_display_width(display);
    int display_height = al_get_display_height(display);

    int x1 = display_width / 6;
    int y1 = display_height / 6 - 20;
    int x2 = display_width - x1;
    int y2 = display_height - y1;

    al_draw_bitmap(bitmap[BITMAP_BACKGROUND], 0, 0, 0);
    al_draw_filled_rectangle(0, 0, al_get_display_width(display), al_get_display_height(display), al_map_rgba(0, 0, 0, 170));
}


void DrawWrappedText(ALLEGRO_FONT *af,char atext[1024],ALLEGRO_COLOR fc, int x1, int y1, int width, int flags,bool draw, Personagem *personagem)
{
    char stext[1024];
    char * pch;
    char word[255];
    char breakchar[12];
    char Lines[40][1024];
    char TempLine[1024];
    int CurrentLine = 0;
    int q;

    strcpy(stext,atext);
    strcpy(breakchar,"\n ");
    strcpy(TempLine,"");
    
    for(q = 0; q < 40; q += 1)
        sprintf(Lines[q],"");
    
    pch = strtok (stext," ");                              
    
    do
    {
        strcpy(word,"");
        sprintf(word,"%s ",pch);
        
        if (!strcmp(word, "P "))
            strcpy(word, personagem->identidade.nome);

        sprintf(TempLine,"%s%s",TempLine,word);            

        if(strcmp(word,breakchar) == 0)
        {
            CurrentLine+=2;                                
            strcpy(TempLine,"");                           
        }
        else
        {
            if(al_get_text_width(af,TempLine) >= (width))
            {
                strcpy(TempLine,word);
                CurrentLine+=2;
            }
            if(CurrentLine < 40)
            {
                strcat(Lines[CurrentLine],word);
            }
        }
        pch = strtok (NULL, " ");
    }
    
    while (pch != NULL);

    if(draw == true)                   
    {
        for(q = 0;q <=CurrentLine;q+=1)
        {
            if(flags == ALLEGRO_ALIGN_LEFT)
                al_draw_text(af,fc, x1, y1 + (q * al_get_font_line_height(af)), ALLEGRO_ALIGN_LEFT,Lines[q]);
            if(flags == ALLEGRO_ALIGN_CENTRE)
                al_draw_text(af,fc, x1 + (width/2), y1 + (q * al_get_font_line_height(af)), ALLEGRO_ALIGN_CENTRE,Lines[q]);
            if(flags == ALLEGRO_ALIGN_RIGHT)
                al_draw_text(af,fc, x1 + width, y1 + (q * al_get_font_line_height(af)), ALLEGRO_ALIGN_RIGHT,Lines[q]);
        }
    }
}


enum { ESTAGIO5_PARTES, CASO_AJUDE, CASO_NAO_AJUDE, SETE_DIAS };

void Estagio5(ALLEGRO_DISPLAY *display, int *option, Personagem *personagem)
{
    // Fontes
    ALLEGRO_FONT *font[] =
    {
        [FONT_GTEK] load_font("Fontes/Righteous-Regular.ttf", 22, 0),
        [FONT_ROBOTO] load_font("Fontes/gtek-technology.ttf", 22, 0),
        [FONT_GTEK_TITLE] load_font("Fontes/Righteous-Regular.ttf", 27, 0)
    };

    // Fila de Eventos
    ALLEGRO_EVENT_QUEUE *queue[] =
    {
        [EVENT_MOUSE_CLICK] create_event_queue()
    };

    al_register_event_source(queue[EVENT_MOUSE_CLICK], al_get_mouse_event_source());

    // Bitmaps
    ALLEGRO_BITMAP *bitmap[1];

    // Variaveis Auxiliares

    char nome_do_arquivo_texto[100];
    char nome_do_arquivo_fundo[100];

    int i, j = 1, n = 0, loop, count = 1, aux = 0, caso = 0, x1, y1, x2, y2;

    char buffer[1024];
    
    y1 = DISPLAY_H - (DISPLAY_H / 5) + 22 / 2 - 10;
    y2 = DISPLAY_H - (DISPLAY_H / 5) + 22 / 2 + 10;

    int estagio5partes = 12, casoAjude, casoNaoAjude, seteDias;

casos:
    for (i = 1; i <= j; i++)
    {
        aux = 0;
        count = 1;

        switch (caso)
        {
        case ESTAGIO5_PARTES:
            i = estagio5partes;
            j = estagio5partes;
            sprintf(nome_do_arquivo_fundo, "Imagens/Estagio5/parte%d.jpg", i);
            sprintf(nome_do_arquivo_texto, "Arquivos/Estagio5/parte%d.txt", i);
            break;
        case CASO_AJUDE:
            //i = casoAjude;
            j = casoAjude;
            sprintf(nome_do_arquivo_fundo, "Imagens/Estagio5/casoAjuda%d.jpg", i);
            sprintf(nome_do_arquivo_texto, "Arquivos/Estagio5/casoAjude%d.txt", i);
            break;
        case CASO_NAO_AJUDE:
            //i = casoNaoAjude;
            j = casoNaoAjude;
            sprintf(nome_do_arquivo_fundo, "Imagens/Estagio5/casoNaoAjuda%d.jpg", i);
            sprintf(nome_do_arquivo_texto, "Arquivos/Estagio5/casoNaoAjude%d.txt", i);
            break;
        case SETE_DIAS:
            //i = seteDias;
            j = seteDias;
            sprintf(nome_do_arquivo_fundo, "Imagens/Estagio5/seteDias%d.jpg", i);
            sprintf(nome_do_arquivo_texto, "Arquivos/Estagio5/seteDias%d.txt", i);
            break;
        }

        al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);

        FILE *arquivo = fopen(nome_do_arquivo_texto, "r");
        bitmap[BITMAP_BACKGROUND] = load_bitmap_at_size(nome_do_arquivo_fundo, DISPLAY_W, DISPLAY_H);

        while (fgetc(arquivo) != EOF)
            count++;

        rewind(arquivo);

        loop = 1;

        while (1)
        {
            if (loop)
            {
                while (!feof(arquivo))
                {
                    if (redraw && al_event_queue_is_empty(queue_video))
                    {
                        switch (caso)
                        {
                        case ESTAGIO5_PARTES:
                            estagio1_draw_background(display, bitmap);
                            if (i < 9)
                                al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "ESTÁGIO V - AGENTE DUPLO");
                            else
                            al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "DIA 18 DE NOVEMBRO. 1O:O4h. BASE DOS REVOLUCIONÁRIOS.");
                            break;
                        case CASO_AJUDE:
                            estagio1_draw_background(display, bitmap);
                            al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "CASO AJUDE");
                            break;
                        case CASO_NAO_AJUDE:
                            estagio1_draw_background(display, bitmap);
                            al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "CASO NAO AJUDE");
                            break;
                        case SETE_DIAS:
                            estagio1_draw_background(display, bitmap);
                            al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "- SETE DIAS DEPOIS -");
                            break;
                        }

                        buffer[aux] = fgetc(arquivo);
                        buffer[aux+1] = '\0';
                        aux++;

                        DrawWrappedText(font[FONT_GTEK], buffer, COLOR_WHITE, DISPLAY_W / 5, DISPLAY_H / 2 - 100, DISPLAY_W - DISPLAY_W / 2.5, ALLEGRO_ALIGN_LEFT, true, personagem);

                        if (i == j && aux == count)
                        {
                            al_rest(0.5);
                            switch (caso)
                            {
                            case ESTAGIO5_PARTES:
                                al_draw_text(font[FONT_ROBOTO], al_map_rgb(255, 255, 255), DISPLAY_W / 5, DISPLAY_H - (DISPLAY_H / 5), ALLEGRO_ALIGN_LEFT, "nao ajudar");
                                al_draw_text(font[FONT_ROBOTO], al_map_rgb(255, 255, 255), DISPLAY_W - (DISPLAY_W / 5), DISPLAY_H - (DISPLAY_H / 5), ALLEGRO_ALIGN_RIGHT, "ajudar");
                                break;
                            case CASO_AJUDE:
                                al_draw_text(font[FONT_ROBOTO], al_map_rgb(255, 255, 255), DISPLAY_W - (DISPLAY_W / 5), DISPLAY_H - (DISPLAY_H / 5), ALLEGRO_ALIGN_RIGHT, "sete dias");
                                break;
                            case CASO_NAO_AJUDE:
                            case SETE_DIAS:
                                al_draw_text(font[FONT_ROBOTO], al_map_rgb(255, 255, 255), DISPLAY_W - (DISPLAY_W / 5), DISPLAY_H - (DISPLAY_H / 5), ALLEGRO_ALIGN_RIGHT, "fim de jogo");
                                break;
                            }
                        }

                        al_flip_display();
                        //al_rest(0.05);
                    }

                    g_event_video();
                }

                loop = 0;
                buffer[0] = '\0';
            }

            while (!al_is_event_queue_empty(queue[EVENT_MOUSE_CLICK]))
            {
                ALLEGRO_EVENT event;

                al_wait_for_event(queue[EVENT_MOUSE_CLICK], &event);

                if (event.type == ALLEGRO_EVENT_MOUSE_AXES && i == j && aux == count)
                {
                    switch (caso)
                    {
                    case ESTAGIO5_PARTES:
                        x1 = 170;
                        x2 = 170;
                        break;
                    case CASO_AJUDE:
                    case CASO_NAO_AJUDE:
                    case SETE_DIAS:
                        x1 = 0;
                        x2 = 170;
                        break;
                    }
                    if (get_mouse_position(event, DISPLAY_W / 5, y1, DISPLAY_W / 5 + x1, y2))
                        al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);

                    else if (get_mouse_position(event, DISPLAY_W - (DISPLAY_W / 5) - x2, y1, DISPLAY_W - (DISPLAY_W / 5), y2))
                        al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);

                    else
                        al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
                }

                if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                {
                    if (i == j && aux == count)
                    {
                        if (get_mouse_position(event, DISPLAY_W / 5, y1, DISPLAY_W / 5 + x1, y2))
                        {
                            switch (caso)
                            {
                                case ESTAGIO5_PARTES:
                                    casoNaoAjude = 2;
                                    caso = CASO_NAO_AJUDE;
                                    goto casos;
                                    break;
                            }
                        }

                        else if (get_mouse_position(event, DISPLAY_W - (DISPLAY_W / 5) - x2, y1, DISPLAY_W - (DISPLAY_W / 5), y2))
                        {
                            switch (caso)
                            {
                                case ESTAGIO5_PARTES:
                                    casoAjude = 2;
                                    caso = CASO_AJUDE;
                                    goto casos;
                                    break;
                                case CASO_AJUDE:
                                    seteDias = 9;
                                    caso = SETE_DIAS;
                                    goto casos;
                                    break;
                                case CASO_NAO_AJUDE:
                                case SETE_DIAS:
                                    creditos(display, option);
                                    menu(display, option);
                                    break;
                            }
                        }
                    }

                    else
                        goto done;
                }
            }
        }

    done:
        fclose(arquivo);
        continue;
    }
}

enum { ESTAGIO4_PARTES, CASO_FIQUE, CASO_NAO_FIQUE };

void Estagio4(ALLEGRO_DISPLAY *display, int *option, Personagem *personagem)
{
    // Fontes
    ALLEGRO_FONT *font[] =
    {
        [FONT_GTEK] load_font("Fontes/Righteous-Regular.ttf", 22, 0),
        [FONT_ROBOTO] load_font("Fontes/gtek-technology.ttf", 22, 0),
        [FONT_GTEK_TITLE] load_font("Fontes/Righteous-Regular.ttf", 27, 0)
    };

    // Fila de Eventos
    ALLEGRO_EVENT_QUEUE *queue[] =
    {
        [EVENT_MOUSE_CLICK] create_event_queue()
    };

    al_register_event_source(queue[EVENT_MOUSE_CLICK], al_get_mouse_event_source());

    // Bitmaps
    ALLEGRO_BITMAP *bitmap[1];

    // Variaveis Auxiliares

    char nome_do_arquivo_texto[100];
    char nome_do_arquivo_fundo[100];

    int i, j = 1, n = 0, loop, count = 1, aux = 0, caso = 0, x1, y1, x2, y2;

    char buffer[1024];

    int estagio4Partes = 19, casoFique, casoNaoFique;
    
    y1 = DISPLAY_H - (DISPLAY_H / 5) + 22 / 2 - 10;
    y2 = DISPLAY_H - (DISPLAY_H / 5) + 22 / 2 + 10;

casos:
    for (i = 1; i <= j; i++)
    {
        aux = 0;
        count = 1;

        switch (caso)
        {
        case ESTAGIO4_PARTES:
            //i = estagio4Partes;
            j = estagio4Partes;
            sprintf(nome_do_arquivo_fundo, "Imagens/Estagio4/parte%d.jpg", i);
            sprintf(nome_do_arquivo_texto, "Arquivos/Estagio4/parte%d.txt", i);
            break;
        case CASO_FIQUE:
            //i = casoFique;
            j = casoFique;
            sprintf(nome_do_arquivo_fundo, "Imagens/Estagio4/casoFique%d.jpg", i);
            sprintf(nome_do_arquivo_texto, "Arquivos/Estagio4/casoFique%d.txt", i);
            break;
        case CASO_NAO_FIQUE:
            //i = casoNaoFique;
            j = casoNaoFique;
            sprintf(nome_do_arquivo_fundo, "Imagens/Estagio4/casoNaoFique%d.jpg", i);
            sprintf(nome_do_arquivo_texto, "Arquivos/Estagio4/casoNaoFique%d.txt", i);
            break;
        }

        al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);

        FILE *arquivo = fopen(nome_do_arquivo_texto, "r");
        bitmap[BITMAP_BACKGROUND] = load_bitmap_at_size(nome_do_arquivo_fundo, DISPLAY_W, DISPLAY_H);

        while (fgetc(arquivo) != EOF)
            count++;

        rewind(arquivo);

        loop = 1;

        while (1)
        {
            if (loop)
            {
                while (!feof(arquivo))
                {
                    if (redraw && al_event_queue_is_empty(queue_video))
                    {
                        switch (caso)
                        {
                        case ESTAGIO4_PARTES:
                            estagio1_draw_background(display, bitmap);
                            if (i < 3)
                                al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "ESTÁGIO IV - GUERRA IMINENTE");
                            else
                            al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "DIA 2O DE NOVEMBRO. 5:17h. NÚCLEO CENTRAL.");
                            break;
                        case CASO_FIQUE:
                            estagio1_draw_background(display, bitmap);
                            al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "CASO FIQUE");
                            break;
                        case CASO_NAO_FIQUE:
                            estagio1_draw_background(display, bitmap);
                            al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "CASO NAO FIQUE");
                            break;
                        }

                        buffer[aux] = fgetc(arquivo);
                        buffer[aux+1] = '\0';
                        aux++;

                        DrawWrappedText(font[FONT_GTEK], buffer, COLOR_WHITE, DISPLAY_W / 5, DISPLAY_H / 2 - 100, DISPLAY_W - DISPLAY_W / 2.5, ALLEGRO_ALIGN_LEFT, true, personagem);

                        if (i == j && aux == count)
                        {
                            al_rest(0.5);
                            switch (caso)
                            {
                            case ESTAGIO4_PARTES:
                                al_draw_text(font[FONT_ROBOTO], al_map_rgb(255, 255, 255), DISPLAY_W / 5, DISPLAY_H - (DISPLAY_H / 5), ALLEGRO_ALIGN_LEFT, "ficar");
                                al_draw_text(font[FONT_ROBOTO], al_map_rgb(255, 255, 255), DISPLAY_W - (DISPLAY_W / 5), DISPLAY_H - (DISPLAY_H / 5), ALLEGRO_ALIGN_RIGHT, "seguir");
                                break;
                            case CASO_NAO_FIQUE:
                            case CASO_FIQUE:
                                al_draw_text(font[FONT_ROBOTO], al_map_rgb(255, 255, 255), DISPLAY_W - (DISPLAY_W / 5), DISPLAY_H - (DISPLAY_H / 5), ALLEGRO_ALIGN_RIGHT, "fim de jogo");
                                break;
                            }
                        }

                        al_flip_display();
                        //al_rest(0.05);
                    }

                    g_event_video();
                }

                loop = 0;
                buffer[0] = '\0';
            }

            while (!al_is_event_queue_empty(queue[EVENT_MOUSE_CLICK]))
            {
                ALLEGRO_EVENT event;

                al_wait_for_event(queue[EVENT_MOUSE_CLICK], &event);

                if (event.type == ALLEGRO_EVENT_MOUSE_AXES && i == j && aux == count)
                {
                    switch (caso)
                    {
                    case ESTAGIO4_PARTES:
                        x1 = 170;
                        x2 = 170;
                        break;
                    case CASO_FIQUE:
                    case CASO_NAO_FIQUE:
                        x1 = 0;
                        x2 = 170;
                        break;
                    }
                    if (get_mouse_position(event, DISPLAY_W / 5, y1, DISPLAY_W / 5 + x1, y2))
                        al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);

                    else if (get_mouse_position(event, DISPLAY_W - (DISPLAY_W / 5) - x2, y1, DISPLAY_W - (DISPLAY_W / 5), y2))
                        al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);

                    else
                        al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
                }

                if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                {
                    if (i == j && aux == count)
                    {
                        if (get_mouse_position(event, DISPLAY_W / 5, y1, DISPLAY_W / 5 + x1, y2))
                        {
                            switch (caso)
                            {
                                case ESTAGIO4_PARTES:
                                    casoFique = 6;
                                    caso = CASO_FIQUE;
                                    goto casos;
                                    break;
                            }
                        }

                        else if (get_mouse_position(event, DISPLAY_W - (DISPLAY_W / 5) - x2, y1, DISPLAY_W - (DISPLAY_W / 5), y2))
                        {
                            switch (caso)
                            {
                                case ESTAGIO4_PARTES:
                                    casoNaoFique = 3;
                                    caso = CASO_NAO_FIQUE;
                                    goto casos;
                                    break;
                                case CASO_FIQUE:
                                case CASO_NAO_FIQUE:
                                    creditos(display, option);
                                    menu(display, option);
                                    break;
                            }
                        }
                    }

                    else
                        goto done;
                }
            }
        }

    done:
        fclose(arquivo);
        continue;
    }
}

enum { ESTAGIO3_PARTES, CASO_ESP, CASO_MISSAO_1, CASO_NAO_SUCESSO, CASO_SEQUEST, CASO_SUCESSO, ESTAGIO3_HORAS_DEPOIS };

void Estagio3(ALLEGRO_DISPLAY *display, int *option, Personagem *personagem)
{
    // Fontes
    ALLEGRO_FONT *font[] =
    {
        [FONT_GTEK] load_font("Fontes/Righteous-Regular.ttf", 22, 0),
        [FONT_ROBOTO] load_font("Fontes/gtek-technology.ttf", 22, 0),
        [FONT_GTEK_TITLE] load_font("Fontes/Righteous-Regular.ttf", 27, 0)
    };

    // Fila de Eventos
    ALLEGRO_EVENT_QUEUE *queue[] =
    {
        [EVENT_MOUSE_CLICK] create_event_queue()
    };

    al_register_event_source(queue[EVENT_MOUSE_CLICK], al_get_mouse_event_source());

    // Bitmaps
    ALLEGRO_BITMAP *bitmap[1];

    // Variaveis Auxiliares

    char nome_do_arquivo_texto[100];
    char nome_do_arquivo_fundo[100];

    int i, j = 1, n = 0, loop, count = 1, aux = 0, caso = 0, x1, y1, x2, y2;

    char buffer[1024];

    int estagio3Partes = 11, casoEsp, casoMissao1, casoNaoSucesso, casoSequest, casoSucesso, estagio3HorasDepois;

    y1 = DISPLAY_H - (DISPLAY_H / 5) + 22 / 2 - 10;
    y2 = DISPLAY_H - (DISPLAY_H / 5) + 22 / 2 + 10;

casos:
    for (i = 1; i <= j; i++)
    {
        aux = 0;
        count = 1;

        switch (caso)
        {
        case ESTAGIO3_PARTES:
            //i = estagio3Partes;
            j = estagio3Partes;
            sprintf(nome_do_arquivo_fundo, "Imagens/Estagio3/parte%d.jpg", i);
            sprintf(nome_do_arquivo_texto, "Arquivos/Estagio3/parte%d.txt", i);
            break;
        case CASO_MISSAO_1:
            //i = casoMissao1;
            j = casoMissao1;
            sprintf(nome_do_arquivo_fundo, "Imagens/Estagio3/casoMissaoUm%d.jpg", i);
            sprintf(nome_do_arquivo_texto, "Arquivos/Estagio3/casoMissaoUm%d.txt", i);
            break;
        case CASO_ESP:
            //i = casoEsp;
            j = casoEsp;
            sprintf(nome_do_arquivo_fundo, "Imagens/Estagio3/casoEsp%d.jpg", i);
            sprintf(nome_do_arquivo_texto, "Arquivos/Estagio3/casoEsp%d.txt", i);
            break;
        case CASO_SEQUEST:
            //i = casoSequest;
            j = casoSequest;
            sprintf(nome_do_arquivo_fundo, "Imagens/Estagio3/casoSequest%d.jpg", i);
            sprintf(nome_do_arquivo_texto, "Arquivos/Estagio3/casoSequest%d.txt", i);
            break;
        case CASO_NAO_SUCESSO:
            //i = casoNaoSucesso;
            j = casoNaoSucesso;
            sprintf(nome_do_arquivo_fundo, "Imagens/Estagio3/casoNaoSucesso%d.jpg", i);
            sprintf(nome_do_arquivo_texto, "Arquivos/Estagio3/casoNaoSucesso%d.txt", i);
            break;
        case CASO_SUCESSO:
            //i = casoSucesso;
            j = casoSucesso;
            sprintf(nome_do_arquivo_fundo, "Imagens/Estagio3/casoSucesso%d.jpg", i);
            sprintf(nome_do_arquivo_texto, "Arquivos/Estagio3/casoSucesso%d.txt", i);
            break;
        case ESTAGIO3_HORAS_DEPOIS:
            //i = estagio3HorasDepois;
            j = estagio3HorasDepois;
            sprintf(nome_do_arquivo_fundo, "Imagens/Estagio3/horasDepois%d.jpg", i);
            sprintf(nome_do_arquivo_texto, "Arquivos/Estagio3/horasDepois%d.txt", i);
            break;
        }

        al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);

        FILE *arquivo = fopen(nome_do_arquivo_texto, "r");
        bitmap[BITMAP_BACKGROUND] = load_bitmap_at_size(nome_do_arquivo_fundo, DISPLAY_W, DISPLAY_H);

        while (fgetc(arquivo) != EOF)
            count++;

        rewind(arquivo);

        loop = 1;

        while (1)
        {
            if (loop)
            {
                while (!feof(arquivo))
                {
                    if (redraw && al_event_queue_is_empty(queue_video))
                    {
                        switch (caso)
                        {
                        case ESTAGIO3_PARTES:
                            estagio1_draw_background(display, bitmap);
                            al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "CÚMPLICE");
                            break;
                        case CASO_MISSAO_1:
                            estagio1_draw_background(display, bitmap);
                            al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "MISSAO UM");
                            break;
                        case CASO_ESP:
                            estagio1_draw_background(display, bitmap);
                            al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "CASO ESPERAR");
                            break;
                        case CASO_SEQUEST:
                            estagio1_draw_background(display, bitmap);
                            al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "CASO SEQUESTAR");
                            break;
                        case CASO_NAO_SUCESSO:
                            estagio1_draw_background(display, bitmap);
                            al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "CASO NAO SUCESSO");
                            break;
                        case CASO_SUCESSO:
                            estagio1_draw_background(display, bitmap);
                            al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "CASO SUCESSO");
                            break;
                        case ESTAGIO3_HORAS_DEPOIS:
                            estagio1_draw_background(display, bitmap);
                            al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "HORAS DEPOIS");
                            break;
                        }

                        buffer[aux] = fgetc(arquivo);
                        buffer[aux+1] = '\0';
                        aux++;

                        DrawWrappedText(font[FONT_GTEK], buffer, COLOR_WHITE, DISPLAY_W / 5, DISPLAY_H / 2 - 100, DISPLAY_W - DISPLAY_W / 2.5, ALLEGRO_ALIGN_LEFT, true, personagem);

                        if (i == j && aux == count)
                        {
                            al_rest(0.5);
                            switch (caso)
                            {
                            case ESTAGIO3_PARTES:
                                al_draw_text(font[FONT_ROBOTO], al_map_rgb(255, 255, 255), DISPLAY_W / 5, DISPLAY_H - (DISPLAY_H / 5), ALLEGRO_ALIGN_LEFT, "fim de jogo");
                                al_draw_text(font[FONT_ROBOTO], al_map_rgb(255, 255, 255), DISPLAY_W - (DISPLAY_W / 5), DISPLAY_H - (DISPLAY_H / 5), ALLEGRO_ALIGN_RIGHT, "missao i");
                                break;
                            case CASO_MISSAO_1:
                                al_draw_text(font[FONT_ROBOTO], al_map_rgb(255, 255, 255), DISPLAY_W / 5, DISPLAY_H - (DISPLAY_H / 5), ALLEGRO_ALIGN_LEFT, "esperar");
                                al_draw_text(font[FONT_ROBOTO], al_map_rgb(255, 255, 255), DISPLAY_W - (DISPLAY_W / 5), DISPLAY_H - (DISPLAY_H / 5), ALLEGRO_ALIGN_RIGHT, "sequestrar");
                                break;
                            case CASO_ESP:
                                al_draw_text(font[FONT_ROBOTO], al_map_rgb(255, 255, 255), DISPLAY_W - (DISPLAY_W / 5), DISPLAY_H - (DISPLAY_H / 5), ALLEGRO_ALIGN_RIGHT, "rolar dado");
                                break;
                            case CASO_SEQUEST:
                            case CASO_NAO_SUCESSO:
                                al_draw_text(font[FONT_ROBOTO], al_map_rgb(255, 255, 255), DISPLAY_W - (DISPLAY_W / 5), DISPLAY_H - (DISPLAY_H / 5), ALLEGRO_ALIGN_RIGHT, "fim de jogo");
                                break;
                            case CASO_SUCESSO:
                                al_draw_text(font[FONT_ROBOTO], al_map_rgb(255, 255, 255), DISPLAY_W - (DISPLAY_W / 5), DISPLAY_H - (DISPLAY_H / 5), ALLEGRO_ALIGN_RIGHT, "horas depois");
                                break; 
                            case ESTAGIO3_HORAS_DEPOIS:
                                al_draw_text(font[FONT_ROBOTO], al_map_rgb(255, 255, 255), DISPLAY_W - (DISPLAY_W / 5), DISPLAY_H - (DISPLAY_H / 5), ALLEGRO_ALIGN_RIGHT, "estagio v");
                                break;
                            }
                        }

                        al_flip_display();
                        //al_rest(0.05);
                    }

                    g_event_video();
                }

                loop = 0;
                buffer[0] = '\0';
            }

            while (!al_is_event_queue_empty(queue[EVENT_MOUSE_CLICK]))
            {
                ALLEGRO_EVENT event;

                al_wait_for_event(queue[EVENT_MOUSE_CLICK], &event);

                if (event.type == ALLEGRO_EVENT_MOUSE_AXES && i == j && aux == count)
                {
                    switch (caso)
                    {
                    case CASO_NAO_SUCESSO:
                        x1 = 0;
                        x2 = 170;
                        break;
                    case CASO_SEQUEST:
                        x1 = 0;
                        x2 = 110;
                        break;
                    case ESTAGIO3_PARTES:
                        x1 = 170;
                        x2 = 110;
                        break;
                    case CASO_MISSAO_1:
                        x1 = 130;
                        x2 = 195;
                        break;
                    case CASO_ESP:
                        x1 = 0;
                        x2 = 175;
                        break;
                    case CASO_SUCESSO:
                    case ESTAGIO3_HORAS_DEPOIS:
                        x1 = 0;
                        x2 = 135;
                    }
                    if (get_mouse_position(event, DISPLAY_W / 5, y1, DISPLAY_W / 5 + x1, y2))
                        al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);

                    else if (get_mouse_position(event, DISPLAY_W - (DISPLAY_W / 5) - x2, y1, DISPLAY_W - (DISPLAY_W / 5), y2))
                        al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);

                    else
                        al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
                }

                if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                {
                    if (i == j && aux == count)
                    {
                        if (get_mouse_position(event, DISPLAY_W / 5, y1, DISPLAY_W / 5 + x1, y2))
                        {
                            switch (caso)
                            {
                            case ESTAGIO3_PARTES:
                                creditos(display, option);
                                menu(display, option);
                                break;
                            case CASO_MISSAO_1:
                                casoEsp = 7;
                                caso = CASO_ESP;
                                goto casos;
                                break;
                            }
                        }

                        else if (get_mouse_position(event, DISPLAY_W - (DISPLAY_W / 5) - x2, y1, DISPLAY_W - (DISPLAY_W / 5), y2))
                        {
                            switch (caso)
                            {
                            case ESTAGIO3_PARTES:
                                casoMissao1 = 14;
                                caso = CASO_MISSAO_1;
                                goto casos;
                                break;
                            case CASO_MISSAO_1:
                                casoSequest = 17;
                                caso = CASO_SEQUEST;
                                goto casos;
                                break;
                            case CASO_SEQUEST:
                            case CASO_NAO_SUCESSO:
                                creditos(display, option);
                                menu(display, option);
                                break;
                            case CASO_ESP:
                                srand(time(NULL));
                                if ((1+(rand()%10)) <= 6)
                                {
                                    casoNaoSucesso = 4;
                                    caso = CASO_NAO_SUCESSO;
                                }
                                else
                                {
                                    casoSucesso = 10;
                                    caso = CASO_SUCESSO;
                                }
                                goto casos;
                                break;
                            case CASO_SUCESSO:
                                estagio3HorasDepois = 5;
                                caso = ESTAGIO3_HORAS_DEPOIS;
                                goto casos;
                                break;
                            case ESTAGIO3_HORAS_DEPOIS:
                                Estagio5(display, option, personagem);
                                break;
                            }
                        }
                    }

                    else
                        goto done;
                }
            }
        }

    done:
        fclose(arquivo);
        continue;
    }
}

enum {ESTAGIO2_PARTES, CASO_DECODI, CASO_DESAC, CASO_ENC, CASO_NAO_DECODI, CASO_NAO_DESAC, CASO_NAO_ENC, CASO_VA_1_ANDAR, CASO_VA_5_ANDAR, HORAS_DEPOIS};

void Estagio2(ALLEGRO_DISPLAY *display, int *option, Personagem *personagem)
{
    // Fontes
    ALLEGRO_FONT *font[] =
    {
        [FONT_GTEK] load_font("Fontes/Righteous-Regular.ttf", 22, 0),
        [FONT_ROBOTO] load_font("Fontes/gtek-technology.ttf", 22, 0),
        [FONT_GTEK_TITLE] load_font("Fontes/Righteous-Regular.ttf", 27, 0)
    };

    // Fila de Eventos
    ALLEGRO_EVENT_QUEUE *queue[] =
    {
        [EVENT_MOUSE_CLICK] create_event_queue()
    };

    al_register_event_source(queue[EVENT_MOUSE_CLICK], al_get_mouse_event_source());

    // Bitmaps
    ALLEGRO_BITMAP *bitmap[1];

    // Variaveis Auxiliares

    char nome_do_arquivo_texto[100];
    char nome_do_arquivo_fundo[100];

    int i, j = 1, n = 0, loop, count = 1, aux = 0, caso = 0, x1, y1, x2, y2;

    char buffer[1024];

    int estagio2Partes = 5;
    int casoDecodi, casoDesac, casoEnc, casoNaoDecodi, casoNaoDesac, casoNaoEnc, casoVa1Andar, casoVa5Andar, horasDepois;

    y1 = DISPLAY_H - (DISPLAY_H / 5) + 22 / 2 - 10;
    y2 = DISPLAY_H - (DISPLAY_H / 5) + 22 / 2 + 10;

casos:
    for (i = 1; i <= j; i++)
    {
        aux = 0;
        count = 1;

        switch (caso)
        {
        case ESTAGIO2_PARTES:
            //i = estagio2Partes;
            j = estagio2Partes;
            sprintf(nome_do_arquivo_fundo, "Imagens/Estagio2/parte%d.jpg", i);
            sprintf(nome_do_arquivo_texto, "Arquivos/Estagio2/parte%d.txt", i);
            break;
        case CASO_VA_1_ANDAR:
            //i = casoVa1Andar;
            j = casoVa1Andar;
            sprintf(nome_do_arquivo_fundo, "Imagens/Estagio2/casoVa1andar%d.jpg", i);
            sprintf(nome_do_arquivo_texto, "Arquivos/Estagio2/casoVa1andar%d.txt", i);
            if (i == 1)
                load_video("Videos/Estagio2/casoVa1andar1.ogv");
            if (i == 3)
                load_video("Videos/Estagio2/casoVa1andar3-4-5.ogv");
            break;
        case CASO_VA_5_ANDAR:
            //i = casoVa5Andar;
            j = casoVa5Andar;
            sprintf(nome_do_arquivo_fundo, "Imagens/Estagio2/casoVa5andar%d.jpg", i);
            sprintf(nome_do_arquivo_texto, "Arquivos/Estagio2/casoVa5andar%d.txt", i);
            break;
        case CASO_NAO_ENC:
            //i = casoNaoEnc;
            j = casoNaoEnc;
            sprintf(nome_do_arquivo_fundo, "Imagens/Estagio2/casoNaoEnc%d.jpg", i);
            sprintf(nome_do_arquivo_texto, "Arquivos/Estagio2/casoNaoEnc%d.txt", i);
            break;
        case CASO_ENC:
            //i = casoEnc;
            j = casoEnc;
            sprintf(nome_do_arquivo_fundo, "Imagens/Estagio2/casoEnc%d.jpg", i);
            sprintf(nome_do_arquivo_texto, "Arquivos/Estagio2/casoEnc%d.txt", i);
            break;
        case CASO_NAO_DESAC:
            //i = casoNaoDesac;
            j = casoNaoDesac;
            sprintf(nome_do_arquivo_fundo, "Imagens/Estagio2/casoNaoDesac%d.jpg", i);
            sprintf(nome_do_arquivo_texto, "Arquivos/Estagio2/casoNaoDesac%d.txt", i);
            break;
        case CASO_DESAC:
            //i = casoDesac;
            j = casoDesac;
            sprintf(nome_do_arquivo_fundo, "Imagens/Estagio2/casoDesac%d.jpg", i);
            sprintf(nome_do_arquivo_texto, "Arquivos/Estagio2/casoDesac%d.txt", i);
            break;
        case CASO_DECODI:
            //i = casoDecodi;
            j = casoDecodi;
            sprintf(nome_do_arquivo_fundo, "Imagens/Estagio2/casoDecodi%d.jpg", i);
            sprintf(nome_do_arquivo_texto, "Arquivos/Estagio2/casoDecodi%d.txt", i);
            break;
        case CASO_NAO_DECODI:
            //i = casoNaoDecodi;
            j = casoNaoDecodi;
            sprintf(nome_do_arquivo_fundo, "Imagens/Estagio2/casoNaoDecodi%d.jpg", i);
            sprintf(nome_do_arquivo_texto, "Arquivos/Estagio2/casoNaoDecodi%d.txt", i);
            break;
        case HORAS_DEPOIS:
            //i = horasDepois;
            j = horasDepois;
            sprintf(nome_do_arquivo_fundo, "Imagens/Estagio2/horasDepois%d.jpg", i);
            sprintf(nome_do_arquivo_texto, "Arquivos/Estagio2/horasDepois%d.txt", i);
            break;
        }

        al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);

        FILE *arquivo = fopen(nome_do_arquivo_texto, "r");
        bitmap[BITMAP_BACKGROUND] = load_bitmap_at_size(nome_do_arquivo_fundo, DISPLAY_W, DISPLAY_H);

        while (fgetc(arquivo) != EOF)
            count++;

        rewind(arquivo);

        loop = 1;

        while (1)
        {
            if (loop)
            {
                while (!feof(arquivo))
                {
                    if (redraw && al_event_queue_is_empty(queue_video))
                    {
                        switch (caso)
                        {
                        case ESTAGIO2_PARTES:
                            estagio1_draw_background(display, bitmap);
                            al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "SENTINELA");
                            break;
                        case CASO_VA_1_ANDAR:
                            estagio1_draw_background(display, bitmap);
                            if (i == 1 || i == 3 || i == 4 || i == 5)
                                video_display(video);
                            al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "DIA 15 DE NOVEMBRO. 21:O7h. TORRE LESTE.");
                            break;
                        case CASO_VA_5_ANDAR:
                        case CASO_NAO_ENC:
                        case CASO_ENC:
                        case CASO_DESAC:
                        case CASO_NAO_DESAC:
                        case CASO_DECODI:
                        case CASO_NAO_DECODI:
                            estagio1_draw_background(display, bitmap);
                            al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "DIA 15 DE NOVEMBRO. 21:O7h. TORRE LESTE.");
                            break;
                        case HORAS_DEPOIS:
                            estagio1_draw_background(display, bitmap);
                            al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "HORAS DEPOIS");
                            break;
                        }

                        buffer[aux] = fgetc(arquivo);
                        buffer[aux+1] = '\0';
                        aux++;

                        DrawWrappedText(font[FONT_GTEK], buffer, COLOR_WHITE, DISPLAY_W / 5, DISPLAY_H / 2 - 100, DISPLAY_W - DISPLAY_W / 2.5, ALLEGRO_ALIGN_LEFT, true, personagem);

                        if (i == j && aux == count)
                        {
                            //al_rest(0.5);
                            switch (caso)
                            {
                            case ESTAGIO2_PARTES:
                                al_draw_text(font[FONT_ROBOTO], al_map_rgb(255, 255, 255), DISPLAY_W / 5, DISPLAY_H - (DISPLAY_H / 5), ALLEGRO_ALIGN_LEFT, "i andar");
                                al_draw_text(font[FONT_ROBOTO], al_map_rgb(255, 255, 255), DISPLAY_W - (DISPLAY_W / 5), DISPLAY_H - (DISPLAY_H / 5), ALLEGRO_ALIGN_RIGHT, "iv andar");
                                break;
                            case CASO_VA_1_ANDAR:
                            case CASO_VA_5_ANDAR:
                            case CASO_ENC:
                                al_draw_text(font[FONT_ROBOTO], al_map_rgb(255, 255, 255), DISPLAY_W - (DISPLAY_W / 5), DISPLAY_H - (DISPLAY_H / 5), ALLEGRO_ALIGN_RIGHT, "rolar dado");
                                break;
                            case CASO_NAO_ENC:
                                al_draw_text(font[FONT_ROBOTO], al_map_rgb(255, 255, 255), DISPLAY_W - (DISPLAY_W / 5), DISPLAY_H - (DISPLAY_H / 5), ALLEGRO_ALIGN_RIGHT, "seguir para o estagio iii");
                                break;
                            case CASO_DESAC:
                            case CASO_DECODI:
                                al_draw_text(font[FONT_ROBOTO], al_map_rgb(255, 255, 255), DISPLAY_W - (DISPLAY_W / 5), DISPLAY_H - (DISPLAY_H / 5), ALLEGRO_ALIGN_RIGHT, "seguir para o estagio iv");
                                break;
                            case CASO_NAO_DESAC:
                                al_draw_text(font[FONT_ROBOTO], al_map_rgb(255, 255, 255), DISPLAY_W - (DISPLAY_W / 5), DISPLAY_H - (DISPLAY_H / 5), ALLEGRO_ALIGN_RIGHT, "horas depois");
                                break;
                            case CASO_NAO_DECODI:
                            case HORAS_DEPOIS:
                                al_draw_text(font[FONT_ROBOTO], al_map_rgb(255, 255, 255), DISPLAY_W - (DISPLAY_W / 5), DISPLAY_H - (DISPLAY_H / 5), ALLEGRO_ALIGN_RIGHT, "fim de jogo");
                                break;
                            }
                        }

                        al_flip_display();
                        //al_rest(0.05);
                    }

                    g_event_video();
                }

                loop = 0;
                buffer[0] = '\0';
            }

            while (!al_is_event_queue_empty(queue[EVENT_MOUSE_CLICK]))
            {
                ALLEGRO_EVENT event;

                al_wait_for_event(queue[EVENT_MOUSE_CLICK], &event);

                if (event.type == ALLEGRO_EVENT_MOUSE_AXES && i == j && aux == count)
                {
                    switch (caso)
                    {
                    case ESTAGIO2_PARTES:
                        x1 = 105;
                        x2 = 125;
                        break;
                    case CASO_VA_1_ANDAR:
                    case CASO_VA_5_ANDAR:
                    case CASO_ENC:
                        x1 = 0;
                        x2 = 180;
                        break;
                    case CASO_NAO_ENC:
                        x1 = 0;
                        x2 = 335;
                        break;
                    case CASO_NAO_DESAC:
                        x1 = 0;
                        x2 = 200;
                        break;
                    case CASO_DESAC:
                    case CASO_DECODI:
                        x1 = 0;
                        x2 = 330;
                        break;
                    case CASO_NAO_DECODI:
                    case HORAS_DEPOIS:
                        x1 = 0;
                        x2 = 170;
                        break;
                    }
                    if (get_mouse_position(event, DISPLAY_W / 5, y1, DISPLAY_W / 5 + x1, y2))
                        al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);

                    else if (get_mouse_position(event, DISPLAY_W - (DISPLAY_W / 5) - x2, y1, DISPLAY_W - (DISPLAY_W / 5), y2))
                        al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);

                    else
                        al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
                }

                if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                {
                    if (i == j && aux == count)
                    {
                        if (get_mouse_position(event, DISPLAY_W / 5, y1, DISPLAY_W / 5 + x1, y2))
                        {
                            switch (caso)
                            {
                            case ESTAGIO2_PARTES:
                                casoVa1Andar = 10;
                                caso = CASO_VA_1_ANDAR;
                                goto casos;
                                break;
                            }
                        }

                        else if (get_mouse_position(event, DISPLAY_W - (DISPLAY_W / 5) - x2, y1, DISPLAY_W - (DISPLAY_W / 5), y2))
                        {
                            switch (caso)
                            {
                            case ESTAGIO2_PARTES:
                                casoVa5Andar = 6;
                                caso = CASO_VA_5_ANDAR;
                                goto casos;
                                break;
                            case CASO_VA_1_ANDAR:
                                srand(time(NULL));
                                if ((1+(rand()%10)) <= 4)
                                {
                                    casoNaoEnc = 5;
                                    caso = CASO_NAO_ENC;
                                }
                                else
                                {
                                    casoEnc = 4;
                                    caso = CASO_ENC;
                                }
                                goto casos;
                                break;
                            case CASO_VA_5_ANDAR:
                                srand(time(NULL));
                                if ((1+(rand()%10)) <= 4)
                                {
                                    casoNaoDecodi = 5;
                                    caso = CASO_NAO_DECODI;
                                }
                                else
                                {
                                    casoDecodi = 7;
                                    caso = CASO_DECODI;
                                }
                                goto casos;
                                break;
                            case CASO_NAO_ENC:
                                Estagio3(display, option, personagem);
                                break;
                            case CASO_ENC:
                                srand(time(NULL));
                                if ((1+(rand()%10)) <= 5)
                                {
                                    casoNaoDesac = 2;
                                    caso = CASO_NAO_DESAC;
                                }
                                else
                                {
                                    casoDesac = 4;
                                    caso = CASO_DESAC;
                                }
                                goto casos;
                                break;
                            case CASO_DESAC:
                            case CASO_DECODI:
                                Estagio4(display, option, personagem);
                                break;
                            case CASO_NAO_DESAC:
                                horasDepois = 5;
                                caso = HORAS_DEPOIS;
                                goto casos;
                                break;
                            case CASO_NAO_DECODI:
                            case HORAS_DEPOIS:
                                creditos(display, option);
                                menu(display, option);
                                break;
                            }
                        }
                    }

                    else
                        goto done;
                }
            }
        }

    done:
        fclose(arquivo);
        continue;
    }
}

enum {PARTE1, CASO_FALE, CASO_NAO, CASO_SIGA};

void Estagio1(ALLEGRO_DISPLAY *display, int *option, Personagem *personagem)
{
    // Fontes
    ALLEGRO_FONT *font[] =
    {
            [FONT_GTEK] load_font("Fontes/Righteous-Regular.ttf", 22, 0),
            [FONT_ROBOTO] load_font("Fontes/gtek-technology.ttf", 22, 0),
            [FONT_GTEK_TITLE] load_font("Fontes/Righteous-Regular.ttf", 27, 0)
    };

    // Fila de Eventos
    ALLEGRO_EVENT_QUEUE *queue[] =
    {
        [EVENT_MOUSE_CLICK] create_event_queue()
    };

    al_register_event_source(queue[EVENT_MOUSE_CLICK], al_get_mouse_event_source());

    // Bitmaps
    ALLEGRO_BITMAP *bitmap[1];

    // Variaveis Auxiliares
    char nome_do_arquivo_texto[50];
    char nome_do_arquivo_fundo[50];

    int i, j = 1, n = 0, loop, count = 1, aux = 0, caso = 0, x1, y1, x2, y2;

    char buffer[1024];

    int parte1 = 13;
    int casoFale, casoNao, casoSiga;

    y1 = DISPLAY_H - (DISPLAY_H / 5) + 22 / 2 - 10;
    y2 = DISPLAY_H - (DISPLAY_H / 5) + 22 / 2 + 10;

casos:
    for (i = 1; i <= j; i++)
    {
        aux = 0;
        count = 1;

        switch (caso)
        {
        case PARTE1:
            //i = 13;
            j = parte1;
            sprintf(nome_do_arquivo_fundo, "Imagens/Estagio1/fundo%d.jpg", i);
            sprintf(nome_do_arquivo_texto, "Arquivos/Estagio1/parte%d.txt", i);
            if (i == 4)
                load_video("Videos/Estagio1/fundo4.ogv");
            if (i == 6)
                load_video("Videos/Estagio1/fundo6.ogv");
            break;

        case CASO_FALE:
            //i = 4;
            j = casoFale;
            sprintf(nome_do_arquivo_fundo, "Imagens/Estagio1/casoFale%d.jpg", i);
            sprintf(nome_do_arquivo_texto, "Arquivos/Estagio1/casoFale%d.txt", i);
            break;

        case CASO_NAO:
            //i = 7;
            j = casoNao;
            sprintf(nome_do_arquivo_fundo, "Imagens/Estagio1/casoNao%d.jpg", i);
            sprintf(nome_do_arquivo_texto, "Arquivos/Estagio1/casoNao%d.txt", i);
            if (i == 1)
                load_video("Videos/Estagio1/casoNao1.ogv");
            break;

        case CASO_SIGA:
            //i = 13;
            j = casoSiga;
            sprintf(nome_do_arquivo_fundo, "Imagens/Estagio1/casoSiga%d.jpg", i);
            sprintf(nome_do_arquivo_texto, "Arquivos/Estagio1/casoSiga%d.txt", i);
            if (i == 1)
                load_video("Videos/Estagio1/casoSiga1.ogv");
        }

        al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);

        FILE *arquivo = fopen(nome_do_arquivo_texto, "r");
        bitmap[BITMAP_BACKGROUND] = load_bitmap_at_size(nome_do_arquivo_fundo, DISPLAY_W, DISPLAY_H);

        while (fgetc(arquivo) != EOF)
            count++;

        rewind(arquivo);

        loop = 1;

        while (1)
        {
            if (loop)
            {
                while (!feof(arquivo))
                {
                    if (redraw && al_event_queue_is_empty(queue_video))
                    {
                        switch (caso)
                        {
                        case PARTE1:
                            if (i <= 3)
                            {
                                estagio1_draw_background(display, bitmap);
                                al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "ESTÁGIO I - FALHA DE SEGURANÇA");
                            }

                            if (i >= 4 && i <= 7)
                            {
                                video_display(video);
                                al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "DIA 15 DE NOVEMBRO. 19:32h. NÚCLEO CENTRAL");
                                redraw = 0;
                            }

                            if (i >= 8)
                            {
                                estagio1_draw_background(display, bitmap);
                                al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "DIA 15 DE NOVEMBRO. 19:32h. NÚCLEO CENTRAL");
                            }
                            break;

                        case CASO_FALE:
                            estagio1_draw_background(display, bitmap);
                            al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "VOCE DECIDIU FALAR");
                            break;

                        case CASO_NAO:
                            if (i == 1)
                                video_display(video);
                            else
                                estagio1_draw_background(display, bitmap);
                            al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "VOCE DECIDIU NAO FALAR OU NAO SEGUIR");
                            break;

                        case CASO_SIGA:
                            if (i == 1)
                                video_display(video);
                            else
                                estagio1_draw_background(display, bitmap);
                            al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "VOCE DECIDIU SEGUIR");
                            break;
                        }

                        buffer[aux] = fgetc(arquivo);
                        buffer[aux+1] = '\0';
                        aux++;

                        DrawWrappedText(font[FONT_GTEK], buffer, COLOR_WHITE, DISPLAY_W / 5, DISPLAY_H / 2 - 100, DISPLAY_W - DISPLAY_W / 2.5, ALLEGRO_ALIGN_LEFT, true, personagem);

                        if (i == j && aux == count)
                        {
                            al_rest(0.5);
                            switch (caso)
                            {
                            case PARTE1:
                                al_draw_text(font[FONT_ROBOTO], al_map_rgb(255, 255, 255), DISPLAY_W / 5, DISPLAY_H - (DISPLAY_H / 5), ALLEGRO_ALIGN_LEFT, "nao falar");
                                al_draw_text(font[FONT_ROBOTO], al_map_rgb(255, 255, 255), DISPLAY_W - (DISPLAY_W / 5), DISPLAY_H - (DISPLAY_H / 5), ALLEGRO_ALIGN_RIGHT, "falar");
                                break;

                            case CASO_FALE:
                                al_draw_text(font[FONT_ROBOTO], al_map_rgb(255, 255, 255), DISPLAY_W / 5, DISPLAY_H - (DISPLAY_H / 5), ALLEGRO_ALIGN_LEFT, "nao seguir");
                                al_draw_text(font[FONT_ROBOTO], al_map_rgb(255, 255, 255), DISPLAY_W - (DISPLAY_W / 5), DISPLAY_H - (DISPLAY_H / 5), ALLEGRO_ALIGN_RIGHT, "seguir");
                                break;

                            case CASO_NAO:
                                al_draw_text(font[FONT_ROBOTO], al_map_rgb(255, 255, 255), DISPLAY_W - (DISPLAY_W / 5), DISPLAY_H - (DISPLAY_H / 5), ALLEGRO_ALIGN_RIGHT, "seguir para o estagio ii");
                                break;

                            case CASO_SIGA:
                                al_draw_text(font[FONT_ROBOTO], al_map_rgb(255, 255, 255), DISPLAY_W - (DISPLAY_W / 5), DISPLAY_H - (DISPLAY_H / 5), ALLEGRO_ALIGN_RIGHT, "seguir para o estagio iii");
                                break;
                            }
                        }

                        al_flip_display();
                        //al_rest(0.05);
                    }

                    g_event_video();
                }

                loop = 0;
                buffer[0] = '\0';
            }

            while (!al_is_event_queue_empty(queue[EVENT_MOUSE_CLICK]))
            {
                ALLEGRO_EVENT event;

                al_wait_for_event(queue[EVENT_MOUSE_CLICK], &event);

                if (event.type == ALLEGRO_EVENT_MOUSE_AXES && i == j && aux == count)
                {
                    switch (caso)
                    {
                    case PARTE1:
                        x1 = 155;
                        x2 = 90;
                        break;
                    case CASO_FALE:
                        x1 = 165;
                        x2 = 100;
                        break;
                    case CASO_NAO:
                        x1 = 0;
                        x2 = 330;
                        break;
                    case CASO_SIGA:
                        x1 = 0;
                        x2 = 335;
                    }
                    if (get_mouse_position(event, DISPLAY_W / 5, y1, DISPLAY_W / 5 + x1, y2))
                        al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);

                    else if (get_mouse_position(event, DISPLAY_W - (DISPLAY_W / 5) - x2, y1, DISPLAY_W - (DISPLAY_W / 5), y2))
                        al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);

                    else
                        al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
                }

                if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                {
                    if (i == j && aux == count)
                    {
                        if (get_mouse_position(event, DISPLAY_W / 5, y1, DISPLAY_W / 5 + x1, y2))
                        {
                            switch (caso)
                            {
                            case PARTE1:
                            case CASO_FALE:
                                casoNao = 7;
                                caso = 2;
                                goto casos;
                                break;
                            }
                        }

                        else if (get_mouse_position(event, DISPLAY_W - (DISPLAY_W / 5) - x2, y1, DISPLAY_W - (DISPLAY_W / 5), y2))
                        {
                            switch (caso)
                            {
                            case PARTE1:
                                casoFale = 4;
                                caso = 1;
                                goto casos;
                                break;
                            case CASO_FALE:
                                casoSiga = 13;
                                caso = 3;
                                goto casos;
                                break;
                            case CASO_NAO:
                                Estagio2(display, option, personagem);
                                break;
                            case CASO_SIGA:
                                Estagio3(display, option, personagem);
                            }
                        }
                    }

                    else
                        goto done;
                }
            }
        }

    done:

        fclose(arquivo);

        continue;
    }
}

/** ----- GAME ----- **/
void game(ALLEGRO_DISPLAY *display)
{
    /** Menu **/

    int option;

    Personagem personagem;
    personagem_init(&personagem);

    while (1)
    {
        switch (menu(display, &option))
        {
        case MENU_CONTINUAR:
            
            break;

        case MENU_NOVO_JOGO:
            
            //personagem.identidade.raca = personagem_raca(display, option);
            personagem_create(&personagem);
            set_personagem(&personagem);
            Estagio1(display, option, &personagem);
            break;

        case MENU_CREDITOS:
            creditos(display, &option);
            break;

        case MENU_SAIR:
            return;
        }
    }
}

/** ----- CREDITOS ----- **/
int creditos_event_keyboard(ALLEGRO_EVENT_QUEUE *queue)
{
    while (!al_event_queue_is_empty(queue))
    {
        ALLEGRO_EVENT event;

        al_wait_for_event(queue, &event);

        if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            return 1;
    }
    return 0;
}
void creditos(ALLEGRO_DISPLAY *display, int *option)
{
    ALLEGRO_EVENT_QUEUE *eventEscape = al_create_event_queue();
    al_register_event_source(eventEscape, al_get_keyboard_event_source());

    while (1)
    {
        if (redraw && al_event_queue_is_empty(queue_video))
        {
            video_display(video);

            redraw = 0;

            al_draw_text(fontGtek30, COLOR_WHITE, DISPLAY_W / 2, DISPLAY_H / 2 - 120, ALLEGRO_ALIGN_CENTER, "creditos");
            al_draw_text(fontGtek20, COLOR_WHITE, DISPLAY_W / 2, DISPLAY_H / 2 - 40, ALLEGRO_ALIGN_CENTER, "alline ribeiro");
            al_draw_text(fontGtek20, COLOR_WHITE, DISPLAY_W / 2, DISPLAY_H / 2 + 10, ALLEGRO_ALIGN_CENTER, "beatriz rangel");
            al_draw_text(fontGtek20, COLOR_WHITE, DISPLAY_W / 2, DISPLAY_H / 2 + 60, ALLEGRO_ALIGN_CENTER, "edson onildo");
            al_draw_text(fontGtek20, COLOR_WHITE, DISPLAY_W / 2, DISPLAY_H / 2 + 110, ALLEGRO_ALIGN_CENTER, "isabela carvalho");

            al_draw_text(fontRoboto12, COLOR_WHITE, DISPLAY_W / 2, (DISPLAY_H - (DISPLAY_H / 4)) + 50, ALLEGRO_ALIGN_CENTER, "PRESSIONE ESC PARA VOLTAR");

            al_flip_display();

            if (creditos_event_keyboard(eventEscape) || event_display_close(option))
                break;
        }

        g_event_video();
    }

    al_destroy_event_queue(eventEscape);
}

/** ----- MAIN ----- **/
int main(int argv, char **argc)
{
    setlocale(LC_ALL, "Portuguese");

    if (!(al_init()))
    {
        fprintf(stderr, "Falha ao inicializar allegro.\n");
        return -1;
    }

    if (!(al_install_mouse()))
    {
        fprintf(stderr, "Falha ao instalar mouse.\n");
        return -1;
    }

    if (!(al_install_keyboard()))
    {
        fprintf(stderr, "Falha ao instalar keyboard.\n");
        return -1;
    }

    al_install_audio();

    al_init_font_addon();

    if (!(al_init_ttf_addon()))
    {
        fprintf(stderr, "Falha ao inicializar ttf add-on.\n");
        return -1;
    }

    if (!(al_init_image_addon()))
    {
        fprintf(stderr, "Falha ao inicializar image add-on.\n");
        return -1;
    }

    if (!(al_init_primitives_addon()))
    {
        fprintf(stderr, "Falha ao inicializar primitives add-on.\n");
        return -1;
    }

    al_init_video_addon();

    al_reserve_samples(1);

    al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE | ALLEGRO_GENERATE_EXPOSE_EVENTS);

    ALLEGRO_DISPLAY *display = al_create_display(1024, 720);

    if (!display)
    {
        fprintf(stderr, "Falha ao criar display.\n");
        return -1;
    }

    al_set_window_title(display, "GHANOR ");

    DISPLAY_W = al_get_display_width(display);
    DISPLAY_H = al_get_display_height(display);

    COLOR_GRAY = al_map_rgb(153, 153, 153);
    COLOR_WHITE = al_map_rgb(255, 255, 255);
    COLOR_BLACK = al_map_rgb(0, 0, 0);

    fontRoboto12 = load_font("Fontes/roboto-regular.ttf", 12, 0);
    fontGtek20 = load_font("Fontes/gtek-technology.ttf", 20, 0);
    fontGtek30 = load_font("Fontes/gtek-technology.ttf", 30, 0);

    eventKeyEnter = create_event_queue();
    al_register_event_source(eventKeyEnter, al_get_keyboard_event_source());

    event_queue_display_close = create_event_queue();
    al_register_event_source(event_queue_display_close, al_get_display_event_source(display));

    event_queue_display_resize = create_event_queue();
    al_register_event_source(event_queue_display_resize, al_get_display_event_source(display));

    game(display);

    return 0;
}
