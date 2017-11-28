#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

ALLEGRO_DISPLAY *display;
ALLEGRO_COLOR COLOR_GRAY;
ALLEGRO_COLOR COLOR_WHITE;
ALLEGRO_COLOR COLOR_BLACK;
enum FONT { FONT_GTEK, FONT_GTEK_TITLE, FONT_ROBOTO };
enum BITMAP { BITMAP_BACKGROUND };
enum EVENT { EVENT_MOUSE, EVENT_MOUSE_HOVER, EVENT_MOUSE_CLICK, EVENT_KEYBOARD_KEYDOWN, EVENT_KEYBOARD_KEYENTER };
ALLEGRO_EVENT_QUEUE *event_queue_display_close  = NULL;
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
bool event_display_close(int *option)
{
    while(!al_is_event_queue_empty(event_queue_display_close))
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
    while(!al_is_event_queue_empty(event_queue_display_resize))
    {
        ALLEGRO_EVENT event;

        al_wait_for_event(event_queue_display_resize, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
            al_acknowledge_resize(event.display.source);
    }
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
    FILE * file = fopen("Arquivos/count.data", "r");

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
    FILE * file = fopen("Arquivos/count.txt", "w");

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
    FILE * file = fopen("Arquivos/count.data", "r");

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
    FILE * file = fopen("Arquivos/count.txt", "w");

    if (file == NULL)
    {
        perror("Falha ao abrir o arquivo Arquivos/count.txt");
        exit(0);
    }

    fprintf(file, "%d", qnt);
    fclose(file);
}

/** ----- JOGADOR ----- **/
int const PTS_ATRIBUTO = 30;
enum { RACA_HUMANO = 1, RACA_SINTOZOIDE };
enum { CLASSE_SOLDADO = 1, CLASSE_ATIRADOR };
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
enum { BITMAP_HUMANO = 1, BITMAP_SINTOZOIDE };
void personagem_raca_draw_background(ALLEGRO_BITMAP **bitmap)
{
    al_draw_bitmap(bitmap[BITMAP_BACKGROUND], 0, 0, 0);
}
void personagem_raca_humano_draw_background(ALLEGRO_BITMAP **bitmap, int *option)
{
    ALLEGRO_COLOR color_bg;
    ALLEGRO_COLOR color_font;
    ALLEGRO_COLOR color_bitmap;

    if (*option == 1)
    {
        color_bg     = al_map_rgba(50, 0, 0, 200);
        color_bitmap = al_map_rgb(255, 255, 255);
    }

    else
    {
        color_bg     = al_map_rgba(0, 0, 0, 200);
        color_bitmap = al_map_rgb(100, 100, 100);
    }

    int x1 = DISPLAY_W / 6;
    int y1 = DISPLAY_H / 5;
    int x2 = DISPLAY_W / 2.1;
    int y2 = DISPLAY_H - y1;

    personagem_layout_draw_background(x1, y1, x2, y2, color_bg);

    int bitmap_width  = al_get_bitmap_width(bitmap[BITMAP_HUMANO]);
    int bitmap_height = al_get_bitmap_height(bitmap[BITMAP_HUMANO]); 

    int x = DISPLAY_W / 3.1 - bitmap_width / 2;
    int y = y2 - (bitmap_height / 2) - 250;

    al_draw_tinted_bitmap(bitmap[BITMAP_HUMANO], color_bitmap, x, y, 0);
}
void personagem_raca_humano_draw_text(ALLEGRO_FONT **font, int *option)
{
    ALLEGRO_COLOR color = (*option == 1) ? COLOR_WHITE : COLOR_GRAY;
    
    int x  = DISPLAY_W / 3.1;
    int y1 = DISPLAY_H / 5;
    int y2 = DISPLAY_H - y1;

    char *text[]
    =
    {
        "humano",
        "SOLDADOS SUBSTITUIDOS PELOS ATIRADORES. QUANDO AS",
        "IA's ASSUMIRAM O PODER, FORAM TOMADOS COMO HEROIS.",
        "SE MOSTRARAM ESSENCIAIS PARA A LIBERTACAO HUMANA"
    };

    personagem_layout_draw_text(x, y1, y2, font, color, text);
}
void personagem_raca_sintozoide_draw_background(ALLEGRO_BITMAP **bitmap, int *option)
{
    ALLEGRO_COLOR color_bg;
    ALLEGRO_COLOR color_font;
    ALLEGRO_COLOR color_bitmap;

    if (*option == 2)
    {
        color_bg     = al_map_rgba(0, 0, 50, 200);
        color_bitmap = al_map_rgb(255, 255, 255);
    }

    else
    {
        color_bg     = al_map_rgba(0, 0, 0, 200);
        color_bitmap = al_map_rgb(100, 100, 100);
    }

    int x1 = DISPLAY_W / 1.9;
    int y1 = DISPLAY_H / 5;
    int x2 = DISPLAY_W - DISPLAY_W / 6;
    int y2 = DISPLAY_H - y1;

    personagem_layout_draw_background(x1, y1, x2, y2, color_bg);

    int bitmap_width  = al_get_bitmap_width(bitmap[BITMAP_SINTOZOIDE]);
    int bitmap_height = al_get_bitmap_height(bitmap[BITMAP_SINTOZOIDE]); 

    int x = DISPLAY_W - (DISPLAY_W / 3.1) - (bitmap_width / 2);
    int y = y2 - (bitmap_height / 2) - 250;

    al_draw_tinted_bitmap(bitmap[BITMAP_SINTOZOIDE], color_bitmap, x, y, 0);
}
void personagem_raca_sintozoide_draw_text(ALLEGRO_FONT **font, int *option)
{
    ALLEGRO_COLOR color = (*option == 2) ? COLOR_WHITE : COLOR_GRAY;
    
    int x  = DISPLAY_W - DISPLAY_W / 3.1;
    int y1 = DISPLAY_H / 5;
    int y2 = DISPLAY_H - y1;

    char *text[]
    =
    {
        "sintozoides",
        "OPERARIOS ESPECIALISTAS EM MECANICA, SAO",
        "OS UNICOS A CONHECER OS SEGREDOS",
        "DAS TECNOLOGIAS DE GHANOR"
    };

    personagem_layout_draw_text(x, y1, y2, font, color, text);
}
int personagem_raca(ALLEGRO_DISPLAY *display, int *option)
{
    /** ----- Fontes ----- **/

    ALLEGRO_FONT *font[]
    =
    {
        [FONT_GTEK]   load_font("Fontes/gtek-technology.ttf", 20, 0),
        [FONT_ROBOTO] load_font("Fontes/roboto-regular.ttf",  12, 0)
    };

    /** ----- Bitmaps ----- **/

    ALLEGRO_BITMAP *bitmap[]
    =
    {
        [BITMAP_HUMANO]     load_bitmap("Imagens/atirador.png"),
        [BITMAP_SINTOZOIDE] load_bitmap("Imagens/operario.png"),
        [BITMAP_BACKGROUND] load_bitmap_at_size("Imagens/bg.jpeg", DISPLAY_W, DISPLAY_H)
    };

    /** ----- Fila de Eventos ----- **/

    ALLEGRO_EVENT_QUEUE *queue[]
    =
    {
        [EVENT_MOUSE_HOVER] create_event_queue(),
        [EVENT_MOUSE_CLICK] create_event_queue()
    };

    al_register_event_source(queue[EVENT_MOUSE_HOVER], al_get_mouse_event_source());
    al_register_event_source(queue[EVENT_MOUSE_CLICK], al_get_mouse_event_source());

    /** ----- Loop ----- **/

    while(1)
    {
        /** ----- Desenhos ----- **/

        personagem_raca_draw_background(bitmap);
        
        personagem_raca_humano_draw_background(bitmap, option);
        personagem_raca_sintozoide_draw_background(bitmap, option);

        personagem_raca_humano_draw_text(font, option);
        personagem_raca_sintozoide_draw_text(font, option);
        
        al_flip_display();

        /** ----- Eventos ----- **/

        event_display_resize();
            
        personagem_layout_event_mouse_hover(queue, option);
        
        if (personagem_layout_event_mouse_click(queue, option)
        ||  event_display_close(option))
            break;
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
    x1 = x1 - 20;
    x2 = x2 + 20;
    al_draw_filled_rectangle(x1, y1, x2, y2, bgcolor);
}
void personagem_layout_draw_text(int x, int y1, int y2, ALLEGRO_FONT **font, ALLEGRO_COLOR color, char **text)
{
    al_draw_text(font[FONT_GTEK],   color, x, y1 + 50,  ALLEGRO_ALIGN_CENTER, text[0]);
    al_draw_text(font[FONT_ROBOTO], color, x, y2 - 110, ALLEGRO_ALIGN_CENTER, text[1]);
    al_draw_text(font[FONT_ROBOTO], color, x, y2 - 80,  ALLEGRO_ALIGN_CENTER, text[2]);
    al_draw_text(font[FONT_ROBOTO], color, x, y2 - 50,  ALLEGRO_ALIGN_CENTER, text[3]);
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

    al_draw_bitmap(bitmap[BITMAP_BACKGROUND], 0, 0, 0);

    personagem_layout_draw_background(x1, y1, x2, y2, al_map_rgba(0, 0, 0, 200));

    position[0][0] = x1 + 50;
    position[0][1] = y1 + 110;
    position[0][2] = x2 - 50;
    position[0][3] = y1 + 143;
    
    al_draw_rectangle(position[0][0], position[0][1], position[0][2], position[0][3], bgcolor, 1);

    position[1][0] = x1 + 50;
    position[1][1] = y2 - 63;
    position[1][2] = x2 - 50;
    position[1][3] = y2 - 30;
    
    al_draw_rectangle(position[1][0], position[1][1], position[1][2], position[1][3], bgcolor, 1);
}
void personagem_create_draw_text(ALLEGRO_FONT **font, Personagem *personagem)
{
    int x1  = DISPLAY_W / 2;
    int x2 = DISPLAY_W / 3;
    int y1 = DISPLAY_H / 5;
    int y2 = DISPLAY_H - y1;

    al_draw_text(font[FONT_GTEK], COLOR_WHITE, x1, y1 + 50,  ALLEGRO_ALIGN_CENTER, "identidade");
    al_draw_text(font[FONT_ROBOTO], COLOR_WHITE, x2 + 80, y1 + 120, ALLEGRO_ALIGN_CENTER, "NICK: ");
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
                    char temp[] = {toupper(event.keyboard.unichar), '\0'};
                    
                    if (toupper(event.keyboard.unichar) >= 'A'
                    &&  toupper(event.keyboard.unichar) <= 'Z')
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
void personagem_create_event_mouse(ALLEGRO_EVENT_QUEUE **queue, ALLEGRO_COLOR *bgcolor, int position[][4])
{
    while (!al_is_event_queue_empty(queue[EVENT_MOUSE]))
    {
        ALLEGRO_EVENT event;

        al_wait_for_event(queue[EVENT_MOUSE], &event);

        if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
            personagem_create_event_mouse_hover(event, position, bgcolor);
    }
}
void personagem_create_event_mouse_hover(ALLEGRO_EVENT event, int position[][4], ALLEGRO_COLOR *bgcolor)
{
    int x1 = position[0][0];
    int y1 = position[0][1];
    int x2 = position[0][2];
    int y2 = position[0][3];

    *bgcolor = (get_mouse_position(event, x1, y1, x2, y2)) ? COLOR_WHITE : COLOR_GRAY;
}
void personagem_create(Personagem *personagem)
{
    /** ----- Fontes ----- **/

    ALLEGRO_FONT *font[]
    =
    {
        [FONT_GTEK]   load_font("Fontes/gtek-technology.ttf", 20, 0),
        [FONT_ROBOTO] load_font("Fontes/roboto-regular.ttf",  12, 0)
    };

    /** ----- Bitmaps ----- **/

    ALLEGRO_BITMAP *bitmap[]
    =
    {
        [BITMAP_BACKGROUND] load_bitmap_at_size("Imagens/bg.jpeg", DISPLAY_W, DISPLAY_H)
    };

    /** ----- Fila de Eventos ----- **/

    ALLEGRO_EVENT_QUEUE *queue[]
    =
    {
        [EVENT_MOUSE] create_event_queue(),
        
        [EVENT_KEYBOARD_KEYDOWN]  create_event_queue(),
        [EVENT_KEYBOARD_KEYENTER] create_event_queue()
    };

    al_register_event_source(queue[EVENT_MOUSE], al_get_mouse_event_source());

    al_register_event_source(queue[EVENT_KEYBOARD_KEYDOWN],  al_get_keyboard_event_source());
    al_register_event_source(queue[EVENT_KEYBOARD_KEYENTER], al_get_keyboard_event_source());

    /** ----- Variaveis Auxiliares ----- **/

    int escape = 0;
    int position[2][4];

    ALLEGRO_COLOR bgcolor = COLOR_GRAY;

    /** ----- Loop ----- **/

    while(1)
    {
        /** ----- Desenhos ----- **/

        personagem_create_draw_background(bitmap, bgcolor, position);
        personagem_create_draw_text(font, personagem);
        
        al_flip_display();

        /** Eventos **/
        
        personagem_create_event_mouse(queue, &bgcolor, position);

        personagem_create_event_keyboard_keydown(queue, personagem, escape);
        personagem_create_event_keyboard_keyenter(queue, &escape);
    }

    al_destroy_font(font[FONT_GTEK]);
    al_destroy_font(font[FONT_ROBOTO]);

    al_destroy_bitmap(bitmap[BITMAP_BACKGROUND]);

    al_destroy_event_queue(queue[EVENT_MOUSE]);

    al_destroy_event_queue(queue[EVENT_KEYBOARD_KEYDOWN]);
    al_destroy_event_queue(queue[EVENT_KEYBOARD_KEYENTER]);
}

/** ----- Menu ----- **/
enum { MENU_CONTINUAR = -4, MENU_NOVO_JOGO = -3, MENU_CREDITOS = -2, MENU_SAIR = -1 };
void menu_draw_background(ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP **bitmap)
{
    int display_width  = al_get_display_width(display);
    int display_height = al_get_display_height(display);

    int x1 = display_width  / 4;
    int y1 = display_height / 4 - 20;
    int x2 = display_width  - x1;
    int y2 = display_height - y1;

    al_draw_tinted_bitmap(bitmap[BITMAP_BACKGROUND], al_map_rgb(127.5, 127.5, 127.5), 0, 0, 0);
    al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgba(0, 0, 0, 170));
}
void menu_draw_text(ALLEGRO_DISPLAY *display, ALLEGRO_FONT **font, int *option)
{
    int x = al_get_display_width(display)  / 2;
    int y = al_get_display_height(display) / 2;
    
    ALLEGRO_FONT *title   = font[FONT_GTEK_TITLE];
    ALLEGRO_FONT *options = font[FONT_GTEK];
    
    int numero_de_personagens = contar_numero_de_personagens();

    ALLEGRO_COLOR continuar = (!numero_de_personagens) ? al_map_rgb(50, 50, 50) : COLOR_GRAY;
    
    al_draw_text(title,   COLOR_GRAY, x, y - 140, ALLEGRO_ALIGN_CENTER, "ghanor");
    al_draw_text(title,   COLOR_GRAY, x, y - 110, ALLEGRO_ALIGN_CENTER, "the revolution");
    al_draw_text(options, continuar,  x, y - 40,  ALLEGRO_ALIGN_CENTER, "continuar");
    al_draw_text(options, COLOR_GRAY, x, y + 10,  ALLEGRO_ALIGN_CENTER, "novo jogo");
    al_draw_text(options, COLOR_GRAY, x, y + 60,  ALLEGRO_ALIGN_CENTER, "creditos");
    al_draw_text(options, COLOR_GRAY, x, y + 110, ALLEGRO_ALIGN_CENTER, "sair");

    switch (*option)
    {
        case MENU_CONTINUAR:
            if (!numero_de_personagens)
                break;

            al_draw_text(options, COLOR_WHITE, x + 1, y - 39, ALLEGRO_ALIGN_CENTER, "continuar");
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
    int x = al_get_display_width(display)  / 2;
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

    else if (*option == MENU_CONTINUAR - 1
         || (*option == MENU_CONTINUAR && !contar_numero_de_personagens()))
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

    ALLEGRO_FONT *font[]
    =
    {
        [FONT_GTEK]       load_font("Fontes/gtek-technology.ttf", 20, 0),
        [FONT_GTEK_TITLE] load_font("Fontes/gtek-technology.ttf", 30, 0)
    };

    /** ----- Bitmaps ----- **/

    ALLEGRO_BITMAP *bitmap[]
    =
    {
        [BITMAP_BACKGROUND] load_bitmap_at_size("Imagens/bg.jpeg", DISPLAY_W, DISPLAY_H)
    };

    /** ----- Fila de Eventos ----- **/

    ALLEGRO_EVENT_QUEUE *queue[]
    =
    {
        [EVENT_MOUSE_HOVER]       create_event_queue(),
        [EVENT_MOUSE_CLICK]       create_event_queue(),
        [EVENT_KEYBOARD_KEYDOWN]  create_event_queue(),
        [EVENT_KEYBOARD_KEYENTER] create_event_queue()
    };

    al_register_event_source(queue[EVENT_MOUSE_HOVER], al_get_mouse_event_source());
    al_register_event_source(queue[EVENT_MOUSE_CLICK], al_get_mouse_event_source());

    al_register_event_source(queue[EVENT_KEYBOARD_KEYDOWN], al_get_keyboard_event_source());
    al_register_event_source(queue[EVENT_KEYBOARD_KEYENTER], al_get_keyboard_event_source());

    /** ----- Loop ----- **/

    while(1)
    {
        /** ----- Desenhos ----- **/

        menu_draw_background(display, bitmap);
        menu_draw_text(display, font, option);

        al_flip_display();

        /** ----- Eventos ----- ***/

        event_display_resize();        
        
        menu_event_mouse_hover(display, queue, option);
        menu_event_keyboard_keydown(queue, option);

        if (menu_event_mouse_click(display, queue, option)
        ||  menu_event_keyboard_keyenter(queue, option)
        ||  event_display_close(option))
            break;
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
    int display_width  = al_get_display_width(display);
    int display_height = al_get_display_height(display);

    int x1 = display_width  / 6;
    int y1 = display_height / 6 - 20;
    int x2 = display_width  - x1;
    int y2 = display_height - y1;

    al_draw_bitmap(bitmap[BITMAP_BACKGROUND], 0, 0, 0);
    al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgba(0, 0, 0, 170));
}
void casoNao(ALLEGRO_DISPLAY *display, int *option)
{
    ALLEGRO_FONT *font[]
    =
    {
        [FONT_GTEK] load_font("Fontes/Righteous-Regular.ttf", 14, 0),
        [FONT_GTEK_TITLE] load_font("Fontes/Righteous-Regular.ttf", 20, 0)
    };

    ALLEGRO_EVENT_QUEUE *queue[]
    =
    {
        [EVENT_MOUSE_CLICK] create_event_queue()
    };

    al_register_event_source(queue[EVENT_MOUSE_CLICK], al_get_mouse_event_source());

    int k;

    char nome_do_arquivo[50];

    ALLEGRO_BITMAP *bitmap[1];

    int j = 3;

    for (k = 1; k <= j; k++)
    {
        /** Carrega a imagem de fundo **/
        bitmap[BITMAP_BACKGROUND] = load_bitmap_at_size("Imagens/Estagio1/casoFale1.jpg", DISPLAY_W, DISPLAY_H);

        /** Carrega o arquivo **/
        sprintf(nome_do_arquivo, "Arquivos/Estagio1/casoNao%d.txt", k);
        FILE *parte1 = fopen(nome_do_arquivo, "r");

         char lixo;
        char c;
        char l[1000];

        int i, count = contar_linhas(parte1);

        while(1)
        {
            estagio1_draw_background(display, bitmap);
                al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "VOCE DECIDIU NAO FALAR OU NAO SEGUIR");

            rewind(parte1);

            for (i = 0; i < count; i++)
            {
                fscanf(parte1, "%[^\n]", l);
                fscanf(parte1, "%c", &lixo);
                al_draw_text(font[FONT_GTEK], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 100 + (25 * i), ALLEGRO_ALIGN_CENTER, l);
            }

            al_flip_display();

            while (!al_is_event_queue_empty(queue[EVENT_MOUSE_CLICK]))
            {
                ALLEGRO_EVENT event;

                al_wait_for_event(queue[EVENT_MOUSE_CLICK], &event);

                if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                {
                    if (k == j)
                    {
                        /** Nao **/
                        if (event.mouse.x < DISPLAY_W / 2)
                            casoNao(display, option);

                        /** Sim **/
                        //if (event.mouse.x > DISPLAY_W / 2)
                            //casoFale(dis);
                    }

                    else
                        goto done;
                }
            }
        }

        done:

        continue;
    }
}
void casoFale(ALLEGRO_DISPLAY *display, int *option)
{
    ALLEGRO_FONT *font[]
    =
    {
        [FONT_GTEK] load_font("Fontes/Righteous-Regular.ttf", 14, 0),
        [FONT_GTEK_TITLE] load_font("Fontes/Righteous-Regular.ttf", 20, 0)
    };

    ALLEGRO_EVENT_QUEUE *queue[]
    =
    {
        [EVENT_MOUSE_CLICK] create_event_queue()
    };

    al_register_event_source(queue[EVENT_MOUSE_CLICK], al_get_mouse_event_source());

    int k;

    char nome_do_arquivo[50];

    ALLEGRO_BITMAP *bitmap[1];

    int j = 2;

    for (k = 1; k <= j; k++)
    {
        bitmap[BITMAP_BACKGROUND] = load_bitmap_at_size("Imagens/Estagio1/casoFale1.jpg", DISPLAY_W, DISPLAY_H);

        /** Carrega o arquivo **/
        sprintf(nome_do_arquivo, "Arquivos/Estagio1/casoFale%d.txt", k);
        FILE *parte1 = fopen(nome_do_arquivo, "r");

         char lixo;
        char c;
        char l[1000];

        int i, count = contar_linhas(parte1);


        while(1)
        {
            estagio1_draw_background(display, bitmap);
                al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "VC DECIDIU FALAR");

            rewind(parte1);

            for (i = 0; i < count; i++)
            {
                fscanf(parte1, "%[^\n]", l);
                fscanf(parte1, "%c", &lixo);
                al_draw_text(font[FONT_GTEK], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 100 + (25 * i), ALLEGRO_ALIGN_CENTER, l);
            }

            al_flip_display();

            while (!al_is_event_queue_empty(queue[EVENT_MOUSE_CLICK]))
            {
                ALLEGRO_EVENT event;

                al_wait_for_event(queue[EVENT_MOUSE_CLICK], &event);

                if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                {
                    if (k == j)
                    {
                        /** Nao **/
                        if (event.mouse.x < DISPLAY_W / 2)
                            casoNao(display, option);

                        /** Sim **/
                        if (event.mouse.x > DISPLAY_W / 2)
                            casoSiga(display, option);
                    }

                    else
                        goto done;
                }
            }
        }

        done:

        continue;
    }
}
void casoSiga(ALLEGRO_DISPLAY *display, int *option){

    ALLEGRO_FONT *font[]
    =
    {
        [FONT_GTEK] load_font("Fontes/Righteous-Regular.ttf", 14, 0),
        [FONT_GTEK_TITLE] load_font("Fontes/Righteous-Regular.ttf", 20, 0)
    };

    ALLEGRO_EVENT_QUEUE *queue[]
    =
    {
        [EVENT_MOUSE_CLICK] create_event_queue()
    };

    al_register_event_source(queue[EVENT_MOUSE_CLICK], al_get_mouse_event_source());

    int k;

    char nome_do_arquivo[50];

    ALLEGRO_BITMAP *bitmap[1];

    int j = 2;

    for (k = 1; k <= j; k++)
    {
        bitmap[BITMAP_BACKGROUND] = load_bitmap_at_size("Imagens/Estagio1/casoFale1.jpg", DISPLAY_W, DISPLAY_H);

        /** Carrega o arquivo **/
        sprintf(nome_do_arquivo, "Arquivos/Estagio1/casoSiga%d.txt", k);
        FILE *parte1 = fopen(nome_do_arquivo, "r");

         char lixo;
        char c;
        char l[1000];

        int i, count = contar_linhas(parte1);


        while(1)
        {
            estagio1_draw_background(display, bitmap);
                al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "VOCE DECIDIU SEGUIR");

            rewind(parte1);

            for (i = 0; i < count; i++)
            {
                fscanf(parte1, "%[^\n]", l);
                fscanf(parte1, "%c", &lixo);
                al_draw_text(font[FONT_GTEK], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 100 + (25 * i), ALLEGRO_ALIGN_CENTER, l);
            }

            al_flip_display();

            while (!al_is_event_queue_empty(queue[EVENT_MOUSE_CLICK]))
            {
                ALLEGRO_EVENT event;

                al_wait_for_event(queue[EVENT_MOUSE_CLICK], &event);

                if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                {
                    if (k == j)
                    {
                        /** Estagio2 **/
                        //if (event.mouse.x < DISPLAY_W / 2)
                          //  estagio2(display, option);

                        /** Sim **/
                        //if (event.mouse.x > DISPLAY_W / 2)
                            //casoFale(dis);
                    }

                    else
                        goto done;
                }
            }
        }

        done:

        continue;
    }

}
void Estagio1(ALLEGRO_DISPLAY *display, int *option)
{
    /** Fontes **/

    ALLEGRO_FONT *font[]
    =
    {
        [FONT_GTEK] load_font("Fontes/Righteous-Regular.ttf", 14, 0),
        [FONT_GTEK_TITLE] load_font("Fontes/Righteous-Regular.ttf", 20, 0)
    };

    /** Fila de Eventos **/

    ALLEGRO_EVENT_QUEUE *queue[]
    =
    {
        [EVENT_MOUSE_CLICK] create_event_queue()
    };

    al_register_event_source(queue[EVENT_MOUSE_CLICK], al_get_mouse_event_source());

    /** Bitmaps **/

    ALLEGRO_BITMAP *bitmap[1];

    /** Variaveis Auxiliares **/

    char nome_do_arquivo[50];

    int k, j = 5;
    char lixo;
    char c;
    char l[1000];

    for (k = 1; k <= j; k++)
    {
        /** Carrega a imagem de fundo **/
        sprintf(nome_do_arquivo, "Imagens/Estagio1/fundo%d.jpg", k);
        bitmap[BITMAP_BACKGROUND] = load_bitmap_at_size(nome_do_arquivo, DISPLAY_W, DISPLAY_H);

        /** Carrega o arquivo **/
        sprintf(nome_do_arquivo, "Arquivos/Estagio1/parte%d.txt", k);
        FILE *parte1 = fopen(nome_do_arquivo, "r");

        int i, count = contar_linhas(parte1);

        while(1)
        {
            if (k == 1)
            {
                estagio1_draw_background(display, bitmap);
                al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "ESTÁGIO I - FALHA DE SEGURANÇA");
            }

            else
            {
                estagio1_draw_background(display, bitmap);
                al_draw_text(font[FONT_GTEK_TITLE], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 200, ALLEGRO_ALIGN_CENTER, "DIA 15 DE NOVEMBRO. 19:32h. NÚCLEO CENTRAL");
            }

            rewind(parte1);

            for (i = 0; i < count; i++)
            {
                fscanf(parte1, "%[^\n]", l);
                fscanf(parte1, "%c", &lixo);
                al_draw_text(font[FONT_GTEK], al_map_rgb(255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2 - 100 + (25 * i), ALLEGRO_ALIGN_CENTER, l);
            }

            al_flip_display();

            while (!al_is_event_queue_empty(queue[EVENT_MOUSE_CLICK]))
            {
                ALLEGRO_EVENT event;

                al_wait_for_event(queue[EVENT_MOUSE_CLICK], &event);

                if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                {
                    if (k == j)
                    {
                        /** Nao **/
                        if (event.mouse.x < DISPLAY_W / 2)
                            casoNao(display, option);

                        /** Sim **/
                        if (event.mouse.x > DISPLAY_W / 2)
                            casoFale(display, option);
                    }

                    else
                        goto done;
                }
            }
        }

        done:

        continue;
    }
}

/** ----- NOVO JOGO ----- **/
void NovoJogo(ALLEGRO_DISPLAY *display, int *option)
{
    Personagem personagem;

    personagem_init(&personagem);

    personagem.identidade.raca = personagem_raca(display, option);

    personagem_create(&personagem);
}

/** ----- GAME ----- **/
void game(ALLEGRO_DISPLAY *display)
{
    /** Menu **/

    int option;

    while (1)
    {
        switch(menu(display, &option))
        {
            case MENU_CONTINUAR:

                break;

            case MENU_NOVO_JOGO:
                NovoJogo(display, &option);
                //Estagio1(display, &option);
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
void creditos_draw_text(ALLEGRO_DISPLAY *display, ALLEGRO_FONT **font)
{
    int x = al_get_display_width(display)  / 2;
    int y = al_get_display_height(display) / 2;

    ALLEGRO_FONT *title   = font[FONT_GTEK_TITLE];
    ALLEGRO_FONT *options = font[FONT_GTEK];
    ALLEGRO_FONT *roboto  = font[FONT_ROBOTO];

    al_draw_text(title,   COLOR_GRAY, x, y - 120, ALLEGRO_ALIGN_CENTER, "creditos");
    al_draw_text(options, COLOR_GRAY, x, y - 40,  ALLEGRO_ALIGN_CENTER, "alline ribeiro");
    al_draw_text(options, COLOR_GRAY, x, y + 10,  ALLEGRO_ALIGN_CENTER, "beatriz rangel");
    al_draw_text(options, COLOR_GRAY, x, y + 60,  ALLEGRO_ALIGN_CENTER, "edson onildo");
    al_draw_text(options, COLOR_GRAY, x, y + 110, ALLEGRO_ALIGN_CENTER, "isabela carvalho");
    
    al_draw_text(roboto, COLOR_WHITE, x, (al_get_display_height(display) - (y / 2)) + 50, ALLEGRO_ALIGN_CENTER, "PRESSIONE ESC PARA VOLTAR");
}
void creditos_draw_background(ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP **bitmap)
{
    int display_width  = al_get_display_width(display);
    int display_height = al_get_display_height(display); 

    int x1 = display_width  / 4;
    int y1 = display_height / 4;
    int x2 = display_width  - x1;
    int y2 = display_height - y1;

    al_draw_tinted_bitmap(bitmap[BITMAP_BACKGROUND], al_map_rgb(127.5, 127.5, 127.5), 0, 0, 0);
    al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgba(0, 0, 0, 170));
}
int creditos_event_keyboard(ALLEGRO_EVENT_QUEUE **queue)
{
    while (!al_event_queue_is_empty(queue[EVENT_KEYBOARD_KEYDOWN]))
    {
        ALLEGRO_EVENT event;

        al_wait_for_event(queue[EVENT_KEYBOARD_KEYDOWN], &event);

        if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            return 1;
    }

    return 0;
}
void creditos(ALLEGRO_DISPLAY *display, int *option)
{
    /** ----- Fontes ----- **/
    
    ALLEGRO_FONT *font[]
    =
    {
        [FONT_ROBOTO]     load_font("Fontes/roboto-regular.ttf",  12, 0),
        [FONT_GTEK]       load_font("Fontes/gtek-technology.ttf", 20, 0),
        [FONT_GTEK_TITLE] load_font("Fontes/gtek-technology.ttf", 30, 0)
    };

    /** ----- Bitmaps ----- **/
    
    ALLEGRO_BITMAP *bitmap[]
    =
    {
        [BITMAP_BACKGROUND] load_bitmap_at_size("Imagens/bg.jpeg", DISPLAY_W, DISPLAY_H)
    };

    /** ----- Fila de Eventos ----- **/
    
    ALLEGRO_EVENT_QUEUE *queue[]
    =
    {
        [EVENT_KEYBOARD_KEYDOWN] create_event_queue()
    };

    al_register_event_source(queue[EVENT_KEYBOARD_KEYDOWN], al_get_keyboard_event_source());

    /** ----- Loop ----- **/

    while(1)
    {
        /** ----- Desenhos ----- **/
        
        creditos_draw_background(display, bitmap);
        creditos_draw_text(display, font);
        
        al_flip_display();

        /** ----- Eventos ----- ***/

        event_display_resize();

        if (creditos_event_keyboard(queue)
        ||  event_display_close(option))
            break;
    }

    al_destroy_font(font[FONT_ROBOTO]);
    al_destroy_font(font[FONT_GTEK]);
    al_destroy_font(font[FONT_GTEK_TITLE]);

    al_destroy_bitmap(BITMAP_BACKGROUND);

    al_destroy_event_queue(queue[EVENT_KEYBOARD_KEYDOWN]);
}

/** ----- MAIN ----- **/
int main (int argv, char **argc)
{
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

    COLOR_GRAY  = al_map_rgb(153, 153, 153);
    COLOR_WHITE = al_map_rgb(255, 255, 255);
    COLOR_BLACK = al_map_rgb(0, 0, 0);

    event_queue_display_close = create_event_queue();
    al_register_event_source(event_queue_display_close, al_get_display_event_source(display));

    event_queue_display_resize = create_event_queue();
    al_register_event_source(event_queue_display_resize, al_get_display_event_source(display));

    game(display);

    return 0;
}