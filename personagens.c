#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

typedef struct identidade
{
    int  id;
    char nome[20];
    char raca[20];
    char profissao[20];
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

PERSONAGEM personagem;

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

void identidade()
{
    printf("\n\nIDENTIDADE:\n\n");

    personagem.identidade.id = count()+1;
    printf("Id: %d\n", personagem.identidade.id);

    printf("Nome: ");
    scanf(" %[^\n]", personagem.identidade.nome);

    printf("Raca: ");
    scanf(" %[^\n]", personagem.identidade.raca);

    printf("Profissao: ");
    scanf(" %[^\n]", personagem.identidade.profissao);
}

void influencia()
{
    printf("\n\nINFLUENCIA:\n\n");

    srand(time(NULL));

    if (!strcmp(personagem.identidade.raca, "humano"))
    {
        personagem.influencia.humano = 30 + rand()%20;
        personagem.influencia.ia     = rand()%20;
    }

    else if(!strcmp(personagem.identidade.raca, "ia"))
    {
        personagem.influencia.humano = rand()%20;
        personagem.influencia.ia     = 30 + rand()%20;
    }

    printf("Influencia sobre as IA: %d\n",      personagem.influencia.ia);
    printf("Influencia sobre os humanos: %d\n", personagem.influencia.humano);
}

void atributos()
{
    srand(time(NULL));

    printf("\n\nATRIBUTOS:\n\n");

    if (!strcmp(personagem.identidade.raca, "humano"))
    {
        if (!strcmp(personagem.identidade.profissao, "soldado"))
        {
            personagem.atributos.inteligencia =      rand()%20;
            personagem.atributos.forca        = 10 + rand()%20;
            personagem.atributos.agilidade    = 10 + rand()%20;
        }

        else if (!strcmp(personagem.identidade.profissao, "cientista"))
        {
            personagem.atributos.forca        =      rand()%20;
            personagem.atributos.agilidade    =      rand()%20;
            personagem.atributos.inteligencia = 15 + rand()%20;
        }
    }

    if (!strcmp(personagem.identidade.raca, "ia"))
    {
        if (!strcmp(personagem.identidade.profissao, "operario"))
        {
            personagem.atributos.inteligencia =      rand()%20;
            personagem.atributos.forca        = 10 + rand()%20;
            personagem.atributos.agilidade    = 10 + rand()%20;
        }

        else if (!strcmp(personagem.identidade.profissao, "atirador"))
        {
            personagem.atributos.forca        =      rand()%20;
            personagem.atributos.inteligencia = 10 + rand()%20;
            personagem.atributos.agilidade    = 10 + rand()%20;
        }
    }

    printf("Forca: %d\n",        personagem.atributos.forca);
    printf("Agilidade: %d\n",    personagem.atributos.agilidade);
    printf("Inteligencia: %d\n", personagem.atributos.inteligencia);
}

void guardar()
{
    int qnt = count();

    char sFile[20];

    sprintf(sFile, "data/personagens/p%d.txt", qnt + 1);

    FILE * file = fopen(sFile, "w+");

    if (file == NULL)
    {
        perror("Falha ao criar o arquivotnagens.txt");
        exit(0);
    }

    fprintf(file, "Id: %d\n",        personagem.identidade.id);
    fprintf(file, "Nome: %s\n",      personagem.identidade.nome);
    fprintf(file, "Raca: %s\n",      personagem.identidade.raca);
    fprintf(file, "Profissao: %s\n", personagem.identidade.profissao);

    fprintf(file, "Influencia IA: %d\n",     personagem.influencia.ia);
    fprintf(file, "Influencia Humano: %d\n", personagem.influencia.humano);

    fprintf(file, "Forca: %d\n",        personagem.atributos.forca);
    fprintf(file, "Agilidade: %d\n",    personagem.atributos.agilidade);
    fprintf(file, "Inteligencia: %d\n", personagem.atributos.inteligencia);

    fclose(file);

    upcount(qnt + 1);
}

PERSONAGEM * load()
{
    int i, qnt = count();

    PERSONAGEM * personagem = (PERSONAGEM *) malloc(qnt * sizeof(PERSONAGEM));

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

        fscanf(file, " Id: %d",           &personagem[i].identidade.id);
        fscanf(file, " Nome: %[^\n]",      personagem[i].identidade.nome);
        fscanf(file, " Raca: %[^\n]",      personagem[i].identidade.raca);
        fscanf(file, " Profissao: %[^\n]", personagem[i].identidade.profissao);

        fscanf(file, " Influencia IA: %d\n",     &personagem[i].influencia.ia);
        fscanf(file, " Influencia Humano: %d\n", &personagem[i].influencia.humano);

        fscanf(file, " Forca: %d\n",        &personagem[i].atributos.forca);
        fscanf(file, " Agilidade: %d\n",    &personagem[i].atributos.agilidade);
        fscanf(file, " Inteligencia: %d\n", &personagem[i].atributos.inteligencia);

        fclose(file);
    }

    return personagem;
}

void print(PERSONAGEM personagem)
{
    clrscr();

    printf("\nNome: %s\n",    personagem.identidade.nome);
    printf("Raca: %s\n",      personagem.identidade.raca);
    printf("Profissao: %s\n", personagem.identidade.profissao);

    printf("Influencia IA: %d\n",     personagem.influencia.ia);
    printf("Influencia Humano: %d\n", personagem.influencia.humano);

    printf("Forca: %d\n",        personagem.atributos.forca);
    printf("Agilidade: %d\n",    personagem.atributos.agilidade);
    printf("Inteligencia: %d\n", personagem.atributos.inteligencia);
}

void criar()
{
    clrscr();
    identidade();
    influencia();
    atributos();
    guardar();
}

void editar(PERSONAGEM personagem)
{
    printf("\n\nEDITAR:\n\n");

    printf("NOVO NOME: ");
    scanf(" %[^\n]", personagem.identidade.nome);

    char sFile[80];

    sprintf(sFile, "data/personagens/p%d.txt", personagem.identidade.id);
    
    FILE * file = fopen(sFile, "w");

    if (file == NULL)
    {
        perror("Falha ao criar o arquivotnagens.txt");
        exit(0);
    }

    fprintf(file, "Id: %d\n",        personagem.identidade.id);
    fprintf(file, "Nome: %s\n",      personagem.identidade.nome);
    fprintf(file, "Raca: %s\n",      personagem.identidade.raca);
    fprintf(file, "Profissao: %s\n", personagem.identidade.profissao);

    fprintf(file, "Influencia IA: %d\n",     personagem.influencia.ia);
    fprintf(file, "Influencia Humano: %d\n", personagem.influencia.humano);

    fprintf(file, "Forca: %d\n",        personagem.atributos.forca);
    fprintf(file, "Agilidade: %d\n",    personagem.atributos.agilidade);
    fprintf(file, "Inteligencia: %d\n", personagem.atributos.inteligencia);

    fclose(file);
}

void apagar(PERSONAGEM * personagem, int id)
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
            perror("Falha ao criar o arquivotnagens.txt");
            exit(0);
        }
    
        fprintf(file, "Id: %d\n",        personagem[i].identidade.id - 1);
        fprintf(file, "Nome: %s\n",      personagem[i].identidade.nome);
        fprintf(file, "Raca: %s\n",      personagem[i].identidade.raca);
        fprintf(file, "Profissao: %s\n", personagem[i].identidade.profissao);
    
        fprintf(file, "Influencia IA: %d\n",     personagem[i].influencia.ia);
        fprintf(file, "Influencia Humano: %d\n", personagem[i].influencia.humano);
    
        fprintf(file, "Forca: %d\n",        personagem[i].atributos.forca);
        fprintf(file, "Agilidade: %d\n",    personagem[i].atributos.agilidade);
        fprintf(file, "Inteligencia: %d\n", personagem[i].atributos.inteligencia);
        
        fclose(file);
    }

    sprintf(string, "data/personagens/p%d.txt", qnt);
    remove(string);

    upcount(qnt - 1);
}

void personagens()
{
    clrscr();

    printf("\n\nPERSONAGENS:\n\n");

    PERSONAGEM * personagem = load();

    int i, qnt = count();

    if (!qnt)
        printf("NAO HA PERSONAGENS CADASTRADOS");

    else
        for (i = 0; i < qnt; i++)
            printf("[%d] %s\n\n", personagem[i].identidade.id, personagem[i].identidade.nome);

    int id;
    int opcao;
    int lixo;

    printf("\n\n[0] CRIAR NOVO PERSONAGEM");
    printf("\n\nOPCAO: ");
    scanf(" %d", &id);

    switch(id)
    {
        case 0:
            criar();
            personagens();
            break;

        default:
            if (id > qnt)
            {
                personagens();
            }

            print(personagem[id-1]);
            printf("\n[1]EDITAR\t[2]APAGAR\t[3]VOLTAR\n");
            printf("\nOpcao: ");
            scanf("%d", &opcao);

            switch(opcao)
            {
                case 1:
                    editar(personagem[id-1]);
                    personagens();
                    break;

                case 2:
                    apagar(personagem, id);
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

    printf("\n---------------------------------------\n");
    printf("\nMENU\n\n");
    printf("1 - Personagens\n");
    printf("\nOpcao: ");

    scanf("%d", &opcao);

    switch (opcao)
    {
        case 1:
            personagens();
            break;
    }
}

int main()
{   
    setlocale(LC_ALL,"Portuguese");
    //criar();
    menu();

    printf("\n\n");
    return 1;
}
