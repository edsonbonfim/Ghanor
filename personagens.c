#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

enum { ia = 1, humano };
enum { soldado = 1, cientista, operario, atirador };

typedef struct identidade
{
    int  id;
    char nome[20];
    int raca;
    int profissao;
} IDENTIDADE;

typedef struct influencia
{
    int humano;
    int ia;
} INFLUENCIA;

typedef struct atributos
{
    int forca;
    int agilidade;
    int inteligencia;
} ATRIBUTOS;

typedef struct personagem
{
    IDENTIDADE identidade;
    INFLUENCIA influencia;
    ATRIBUTOS  atributos;
} PERSONAGEM;

void clrscr()
{
    system("@cls||clear");
}

int count()
{
    FILE * file = fopen("data/personagens/count.txt", "r");

    if (file == NULL)
    {
        perror("Falha ao abrir o arquivo data/personagens/count.txt");
        exit(0);
    }

    int qnt;

    fscanf(file, "%d", &qnt);
    fclose(file);

    return qnt;
}

void upcount(int qnt)
{
    FILE * file = fopen("data/personagens/count.txt", "w");

    if (file == NULL)
    {
        perror("Falha ao abrir o arquivo data/personagens/count.txt");
        exit(0);
    }

    fprintf(file, "%d", qnt);
    fclose(file);
}

// CREATE PERSONAGEM

void create_personagem(FILE * file, PERSONAGEM * personagem, int id)
{
    fprintf(file, "Id: %d\n",        id);
    fprintf(file, "Nome: %s\n",      personagem->identidade.nome);
    fprintf(file, "Raca: %d\n",      personagem->identidade.raca);
    fprintf(file, "Profissao: %d\n", personagem->identidade.profissao);

    fprintf(file, "Influencia IA: %d\n",     personagem->influencia.ia);
    fprintf(file, "Influencia Humano: %d\n", personagem->influencia.humano);

    fprintf(file, "Forca: %d\n",        personagem->atributos.forca);
    fprintf(file, "Agilidade: %d\n",    personagem->atributos.agilidade);
    fprintf(file, "Inteligencia: %d\n", personagem->atributos.inteligencia);
}

// EDIT PERSONAGEM

void edit_personagem(PERSONAGEM * personagem)
{
    printf("\n\nEDITAR:\n\n");
    
    printf("NOVO NOME: ");
    scanf(" %[^\n]", personagem->identidade.nome);
    
    char sFile[80];
    
    sprintf(sFile, "data/personagens/p%d.txt", personagem->identidade.id);
    
    FILE * file = fopen(sFile, "w");
    
    if (file == NULL)
    {
        perror("Falha ao criar o arquivotnagens.txt");
        exit(0);
    }

    create_personagem(file, personagem, personagem->identidade.id);

    fclose(file);
}

// DELETE PERSONAGEM

void delete_personagem(PERSONAGEM ** personagem, int id)
{
    int qnt = count();

    char string[80], string2[80];

    for (int i = id; i < qnt; i++)
    {
        sprintf(string, "data/personagens/p%d.txt", i);
        sprintf(string2,  "data/personagens/p%d.txt", i + 1);
        
        remove(string);
        rename(string2, string);

        FILE * file = fopen(string, "w");

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

// LOAD PERSONAGENS

PERSONAGEM ** load_personagens()
{
    int i, qnt = count();

    PERSONAGEM ** personagem = (PERSONAGEM **) malloc(qnt * sizeof(PERSONAGEM *));
    
    for (i = 0; i < qnt; i++)
    {
        personagem[i] = (PERSONAGEM *) malloc(sizeof(PERSONAGEM));
    }

    char sFile[20];

    for (i = 0; i < qnt; i++)
    {
        sprintf(sFile, "data/personagens/p%d.txt", i+1);

        FILE * file = fopen(sFile, "r");

        if (file == NULL)
        {
            perror("Falha ao abrir o arquivo");
            exit(0);
        }

        fscanf(file, " Id: %d",        &personagem[i]->identidade.id);
        fscanf(file, " Nome: %[^\n]",   personagem[i]->identidade.nome);
        fscanf(file, " Raca: %d",      &personagem[i]->identidade.raca);
        fscanf(file, " Profissao: %d", &personagem[i]->identidade.profissao);

        fscanf(file, " Influencia IA: %d\n",     &personagem[i]->influencia.ia);
        fscanf(file, " Influencia Humano: %d\n", &personagem[i]->influencia.humano);

        fscanf(file, " Forca: %d\n",        &personagem[i]->atributos.forca);
        fscanf(file, " Agilidade: %d\n",    &personagem[i]->atributos.agilidade);
        fscanf(file, " Inteligencia: %d\n", &personagem[i]->atributos.inteligencia);

        fclose(file);
    }

    return personagem;
}

// PRINT PERSONAGEM

void print_personagem(PERSONAGEM * personagem)
{
    clrscr();

    printf("\nNome: %s\n",    personagem->identidade.nome);
    
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

    printf("Influencia IA: %d\n",     personagem->influencia.ia);
    printf("Influencia Humano: %d\n", personagem->influencia.humano);

    printf("Forca: %d\n",        personagem->atributos.forca);
    printf("Agilidade: %d\n",    personagem->atributos.agilidade);
    printf("Inteligencia: %d\n", personagem->atributos.inteligencia);
}

// SET PERSONAGEM IDENTIDADE

void set_personagem_identidade_id(PERSONAGEM * personagem, int id)
{
    personagem->identidade.id = id;
}

void set_personagem_identidade_nome(PERSONAGEM * personagem, char * nome)
{
    strcpy(personagem->identidade.nome, nome);
}

void set_personagem_identidade_raca(PERSONAGEM * personagem, int raca)
{
    personagem->identidade.raca = raca;
}

void set_personagem_identidade_profissao(PERSONAGEM * personagem, int profissao)
{
    personagem->identidade.profissao = profissao;
}

void set_personagem_identidade(PERSONAGEM * personagem)
{
    set_personagem_identidade_id(personagem, count() + 1);

    char nome[20];
    printf("Nome: ");
    scanf(" %[^\n]", nome);
    set_personagem_identidade_nome(personagem, nome);

    int raca;
    printf("Raca:\n");
    printf("[1] IA\t[2]HUMANO: ");
    scanf(" %d", &raca);
    set_personagem_identidade_raca(personagem, raca);

    int profissao;
    printf("Profissao:\n");
    printf("[1] SOLDADO\t[2]CIENTISTA\t[3]OPERARIO\t[4]ATIRADOR: ");
    scanf(" %d", &profissao);
    set_personagem_identidade_profissao(personagem, profissao);
}

// SET PERSONAGEM INFLUENCIA

void set_personagem_influencia(PERSONAGEM * personagem)
{
    srand(time(NULL));
    
    switch(personagem->identidade.raca)
    {
        case ia:
            personagem->influencia.humano = rand()%20;
            personagem->influencia.ia     = 30 + rand()%20;
            break;
        
        case humano:
            personagem->influencia.humano = 30 + rand()%20;
            personagem->influencia.ia     = rand()%20;
            break;
    }
}

// SET PERSONAGEM ATRIBUTOS

void set_personagem_atributos(PERSONAGEM * personagem)
{
    srand(time(NULL));
    
    switch (personagem->identidade.profissao)
    {
        case operario:
            personagem->atributos.inteligencia =      rand()%20;
            personagem->atributos.forca        = 10 + rand()%20;
            personagem->atributos.agilidade    = 10 + rand()%20;
        break;
        
        case atirador:
            personagem->atributos.forca        =      rand()%20;
            personagem->atributos.inteligencia = 10 + rand()%20;
            personagem->atributos.agilidade    = 10 + rand()%20;
            break;
    
        case soldado:
            personagem->atributos.inteligencia =      rand()%20;
            personagem->atributos.forca        = 10 + rand()%20;
            personagem->atributos.agilidade    = 10 + rand()%20;
            break;
    
        case cientista:
            personagem->atributos.forca        =      rand()%20;
            personagem->atributos.agilidade    =      rand()%20;
            personagem->atributos.inteligencia = 15 + rand()%20;
    }
}

// SET PERSONAGEM

void set_personagem()
{
    clrscr();

    PERSONAGEM * personagem = (PERSONAGEM *) malloc(sizeof(PERSONAGEM));

    set_personagem_identidade(personagem);
    set_personagem_influencia(personagem);
    set_personagem_atributos(personagem);

    int qnt = count();
    
    char sFile[20];

    sprintf(sFile, "data/personagens/p%d.txt", qnt + 1);

    FILE * file = fopen(sFile, "w+");

    if (file == NULL)
    {
        perror("Falha ao criar o arquivotnagens.txt");
        exit(0);
    }

    create_personagem(file, personagem, personagem->identidade.id);

    fclose(file);

    upcount(qnt + 1);
}

void personagens()
{
    clrscr();

    printf("\n\nPERSONAGENS:\n\n");

    PERSONAGEM ** personagem = load_personagens();

    int i, qnt = count();

    if (!qnt)
        printf("NAO HA PERSONAGENS CADASTRADOS");

    else
        for (i = 0; i < qnt; i++)
            printf("[%d] %s\n\n", personagem[i]->identidade.id, personagem[i]->identidade.nome);

    int id;
    int opcao;

    printf("\n\n[0] CRIAR NOVO PERSONAGEM");
    printf("\n\nOPCAO: ");
    scanf(" %d", &id);

    switch(id)
    {
        case 0:
            set_personagem();
            personagens();
            break;

        default:
            if (id > qnt)
            {
                personagens();
            }

            print_personagem(personagem[id-1]);
            printf("\n[1]EDITAR\t[2]APAGAR\t[3]VOLTAR\n");
            printf("\nOpcao: ");
            scanf("%d", &opcao);

            switch(opcao)
            {
                case 1:
                    edit_personagem(personagem[id-1]);
                    personagens();
                    break;

                case 2:
                    delete_personagem(personagem, id);
                    personagens();
                    break;

                default:
                    personagens();
            }
        }

    free(personagem);
}

void menu()
{
    int opcao;

    printf("\n\nMENU\n\n1 - Personagens\n\nOpcao: ");
    scanf("%d", &opcao);

    switch (opcao)
    {
        case 1:
            personagens();
            break;
        default:
            menu();
    }
}

int main()
{   
    setlocale(LC_ALL,"Portuguese");

    menu();
    
    printf("\n\n");
    return 1;
}
