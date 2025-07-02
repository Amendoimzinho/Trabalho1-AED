#include "livro.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ENTER printf("\nPressione ENTER para voltar...");scanf("%*c")

/* AUXILIARES */

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void strToupper(char *str) { // Nao eh usada
    while (*str) {
        *str = toupper((*str));
        str++;
    }
}

        /* LIVRO */

Livro* pegarLivro(FILE* f, const int pos) {
    Livro* livro = malloc(sizeof(Livro)); // Aloca novo livro
    if (!livro) return NULL; // Retorna NULL se der Erro

    fseek(f,sizeof(CabLivro) + pos * sizeof(Livro),SEEK_SET); // Localiza o Livro no arquivo
    fread(livro, sizeof(Livro), 1, f); // Le o livro

    return livro; // Retorna o que encontrou
}

Livro* novoLivro() {
    Livro* l = malloc(sizeof(Livro)); if (!l) return NULL; // Aloca novo livro

    // inicializa todas as areas
    l->Codigo = -1;
    memset(l->Titulo, 0, sizeof(l->Titulo));
    memset(l->Autor, 0, sizeof(l->Autor));
    memset(l->Editora, 0, sizeof(l->Editora));
    l->Edicao = 0;
    l->Ano = 0;
    l->Exemplares = 0;
    l->Prox = -1;
    l->Pos = -1;

    return l; // Retorna o Livro inicializado
}

void copiarLivro(Livro* destino, const Livro* origem) { // Nao eh usada
    destino->Ano = origem->Ano;
    strcpy(destino->Autor, origem->Autor);
    strcpy(destino->Titulo, origem->Titulo);
    strcpy(destino->Editora, origem->Editora);
    destino->Edicao = origem->Edicao;
    destino->Codigo = origem->Codigo;
    destino->Exemplares = origem->Exemplares;
}

void escreverLivro(Livro* newLivro, const int pos, FILE* f) {
    newLivro->Pos = pos; // Atualiza a localizacao
    newLivro->Prox = pos - 1;
    fseek(f, sizeof(CabLivro) + sizeof(Livro) * pos, SEEK_SET); // Localiza a posicao no arquivo
    fwrite(newLivro, sizeof(Livro), 1, f); // Escreve o novo livro
}

Livro* procurarLivroId(FILE* f , const int id, int pos) {
    Livro* l = NULL;
    while (pos >= 0) { // Enquanto a posicao eh valida
        l = pegarLivro(f, pos); // Pega o Livro da posicao
        if (!l) break;
        if (l->Codigo == id) return l; // Se encontrar retorna o ponteiro
        pos = l->Prox;
        free(l); // Senao libera e repete
    }
    return NULL; // Se nao encontrar retorna NULL
}

Livro* procurarLivroTitulo(FILE* f , const char* titulo, int pos) {
    Livro* l = NULL;
    while (pos >= 0) {  // mesma coisa que procura por ID
        l = pegarLivro(f, pos);
        if (!l) continue;
        if (!strcmp(titulo, l->Titulo)) return l; // So muda a comparacao
        pos = l->Prox;
        free(l);
    }
    return NULL;
}

void imprimirLivro_T(Livro* l) {
    printf("\n== %s(%04d) ==\n", l->Titulo, l->Ano);
    printf("Edicao: %d\n", l->Edicao);
    printf("Autor: %s\n", l->Autor);
    printf("Editora: %s\n", l->Editora);
    printf("ID: %d\n", l->Codigo);
    printf("Exemplares: %d\n", l->Exemplares);
}

int procurarID(FILE* f, int pos, const int ID) {
    Livro* l = NULL;
    int res = 0;
    while (pos >= 0 && !res) {
        l = pegarLivro(f, pos); if (!l) break;
        if (l->Codigo == ID) res = 1;
        pos = l->Prox;
        free(l);
    }
    return res;
}

            /* CABEÇALHO */

void pegarCabecalhoL(FILE * f, CabLivro* cab) {
    fseek(f,0,SEEK_SET); // Vai para o inicio do arquivo
    fread(cab,sizeof(CabLivro),1,f); // Le o cabeçalho
}

void escreverCabecalhoL(FILE* f, const CabLivro* cab) {
    fseek(f,0,SEEK_SET); // Vai para o inicio do arquivo
    fwrite(cab,sizeof(CabLivro),1,f); // Escreve o Cabeçalho
}

void criarCabecalhoL() {
    FILE* f = fopen("livros.bin", "wb");
    if (!f) {printf("Erro ao abrir livros.bin\n"); return;}

    CabLivro cab;

    cab.Topo = 0;
    cab.Inicio = -1;
    cab.QuantLivros = 0;

    escreverCabecalhoL(f, &cab);
    fclose(f);
}

int qntLivros() { // Nao eh usada
    FILE* f = fopen("livros.bin", "r+b");
    if (!f) {printf("Erro ao abrir livros.bin\n"); return 0;}

    CabLivro cab; pegarCabecalhoL(f, &cab);
    return cab.QuantLivros;
}

            /* FUNCOES PARA O USER */

void novoLivro_User() {
    FILE* f = fopen("livros.bin", "r+b"); if (!f) {printf("Erro ao abrir livros.bin\n"); return;}

    Livro* newLivro = novoLivro();
    if (!newLivro){printf("Erro ao criar novo livro"); fclose(f); return;}

    CabLivro cab; pegarCabecalhoL(f, &cab); // Le o Cabeçalho\


    printf("====== Cadastrando Livro ======\n(digite apenas o pedido e cuidado com as quebras de linha!!)\n\n");

    printf("Digite o Codigo do Livro\n=> ");
    scanf("%d", &(newLivro->Codigo)); limparBuffer();
    while (newLivro->Codigo < 0 || procurarID(f, cab.Inicio, newLivro->Codigo)) {
        printf("\nCodigo invalido ou ja existente, digite novament\n=> ");
        scanf("%d", &(newLivro->Codigo)); limparBuffer();
    }

    printf("Digite o Titulo\n=> ");
    scanf("%150[^\n]", newLivro->Titulo);limparBuffer();
    printf("Digite o nome do Autor:\n=> ");
    scanf("%200[^\n]", newLivro->Autor);limparBuffer();
    printf("Digite o nome da Editora:\n=> ");
    scanf("%50[^\n]", newLivro->Editora);limparBuffer();
    printf("Digite a Edicao:\n=> ");
    scanf("%d", &(newLivro->Edicao));limparBuffer();
    printf("Digite o Ano:\n=> ");
    scanf("%d", &(newLivro->Ano));limparBuffer();
    printf("Digite a quantidade de Exemplares:\n=> ");
    scanf("%d", &(newLivro->Exemplares));limparBuffer();


    newLivro->Pos = cab.Topo;
    escreverLivro(newLivro, cab.Topo, f); // Escreve o novo livro

    cab.QuantLivros += newLivro->Exemplares; // Atualiza o total de livros
    cab.Inicio = cab.Topo++; // Atualiza o inicio e o topo

    escreverCabecalhoL(f, &cab); // Atualiza o cabeçalho

    free(newLivro);fclose(f);

    ENTER;
}

void imprimirLivroId() {
    FILE* f = fopen("livros.bin", "rb");
    if (!f) {printf("Erro ao abrir livros.bin\n"); return;}

    int id = -1;
    printf("\nDigite o ID do Livro\n""=> ");
    scanf("%d", &id); limparBuffer();

    CabLivro cab; pegarCabecalhoL(f, &cab);

    Livro* l = procurarLivroId(f, id, cab.Inicio);

    if (l) {
        imprimirLivro_T(l);
        free(l);
    }else printf("\nErro: Livro nao encontrado ou nao existente\n");

    fclose(f);

    ENTER;
}

void imprimirLivroTitulo() {
    FILE* f = fopen("livros.bin", "rb");
    if (!f) {printf("Erro ao abrir livros.bin\n"); return;}

    char* titulo = malloc(sizeof(char) * 151);
    printf("Digite o Titulo:\n");
    scanf("%150[^\n]", titulo);limparBuffer();

    CabLivro cab; pegarCabecalhoL(f, &cab);

    Livro* l = procurarLivroTitulo(f, titulo, cab.Inicio);

    if (l) {
        imprimirLivro_T(l);
        free(l);
    }else printf("\nErro: Livro nao encontrado ou nao existente\n");
    fclose(f); free(titulo);

    ENTER;
}

void imprimirListaLivro() {
    FILE* f = fopen("livros.bin", "rb"); if (!f) {printf("Erro ao abrir livros.bin\n"); return;}

    CabLivro cab; pegarCabecalhoL(f, &cab);
    int n = cab.Inicio;
    Livro* l = NULL;

    printf("\n====== Lista de Livros ======\n");
    while (n >= 0) {
        l = pegarLivro(f, n);
        if (!l) break;
        n = l->Prox;
        imprimirLivro_T(l);
        free(l);
    }
    fclose(f);
    printf("\n====== Fim da Lista ======\n");

    ENTER;
}

void editarLivroID() {
    int ID;
    printf("\nDigite o ID do livro\n=> ");
    scanf("%d", &ID); limparBuffer();

    FILE* f = fopen("livros.bin", "r+b"); if (!f) {printf("Erro ao abrir livros.bin\n"); return;}

    CabLivro cab; pegarCabecalhoL(f,&cab);

    Livro* l = procurarLivroId(f,ID,cab.Inicio);
    if (!l){fclose(f); printf("\nErro: Livro nao encontrado ou nao existente\n"); ENTER;return;}

    printf("====== Menu de Edicao de Livro ======\n"
           "1 - Editar Titulo\n"
           "2 - Editar Autor\n"
           "3 - Editar Editora\n"
           "4 - Editar Ano\n"
           "5 - Editar Edicao do Livro\n"
           "6 - Editar Codigo\n"
           "7 - Editar Quantidade de Exemplares\n"
           "\n=> ");

    int op; scanf("%d", &op); limparBuffer();

    switch (op) {
        case 1 : printf("\nDigite novo Titulo:\n=> ");
                 scanf("%150[^\n]", l->Titulo);limparBuffer(); break;

        case 2 : printf("\nDigite novo Autor:\n=> ");
                 scanf("%200[^\n]", l->Autor);limparBuffer(); break;

        case 3 : printf("\nDigite nova Editora:\n=> ");
                 scanf("%50[^\n]", l->Editora);limparBuffer(); break;

        case 4 : printf("\nDigite novo Ano:\n=> ");
                 scanf("%d", &l->Ano);limparBuffer();break;

        case 5 : printf("\nDigite nova Edicao:\n=> ");
                 scanf("%d", &l->Edicao);limparBuffer();break;

        case 6 : printf("\nDigite novo Codigo:\n=> ");
                 scanf("%d", &l->Codigo);limparBuffer();
                 while (l->Codigo < 0 || procurarID(f, cab.Inicio, l->Codigo)) {
                     printf("\nCodigo invalido ou ja existente, digite novamente:\n=> ");
                     scanf("%d", &l->Codigo); limparBuffer();
                 }break;

        case 7 : printf("\nDigite nova Quantidade(%d):\n=> ", l->Exemplares);
                 cab.QuantLivros -= l->Exemplares;
                 scanf("%d", &l->Exemplares);limparBuffer();
                 cab.QuantLivros += l->Exemplares;
                 escreverCabecalhoL(f, &cab); break;

        default: printf("\nErro: valor nao reconhecido\n\n");
        }
    escreverLivro(l, l->Pos, f);
    free(l); fclose(f);

    ENTER;
}

void imprimirQuantidade() {
    FILE* f = fopen("livros.bin", "r+b");
    if (!f) {printf("Erro ao abrir livros.bin\n"); return;}

    CabLivro cab; pegarCabecalhoL(f, &cab);

    printf("\n=================================\n"
           "(%d) Livros Cadastrados"
           "\n(%d) Exemplares Disponiveis\n"
           "=================================\n", cab.Inicio, cab.QuantLivros);
    fclose(f);
    
    ENTER;
}