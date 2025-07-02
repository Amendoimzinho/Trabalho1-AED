#include "livro.h"
#include "usuario.h"
#include "emprestimos.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define ENTER printf("\nPressione ENTER para voltar...");scanf("%*c")

        /* auxiliares */

void pegarDataAtual(char* data) {
    time_t rawtime;
    time(&rawtime);

    struct tm *timeinfo = localtime(&rawtime);
    if (!timeinfo) {
        strcpy(data, "01,01,1970");
        return;
    }

    strftime(data, TAM, "%d/%m/%Y", timeinfo);
}

char* tiraEspacos(char* str){
    char *ini;
    ini = str;
    while(*ini == ' ') ini++;
    memmove(str, ini, strlen(ini) + 1);

    char* end;
    end = str + strlen(str) - 1;
    while(end >= str && *end == ' ')end--;
    *(end + 1) = '\0';

    return str; 
}
 
        /* EMPRESTIMOS */

Emprestimo* novoEmprestimo() {
    Emprestimo* e = malloc(sizeof(Emprestimo));
    if (!e) return NULL;

    strcpy(e->dataEmprestimo, "01/01/1970");
    strcpy(e->dataDevolucao, "01/01/1970");
    e->Devolvido = 0;
    e->idLivro = -1;
    e->idUsuario = -1;
    e->Prox = -1;
    e->Pos = -1;

    return e;
}

Emprestimo* buscarEmprestimoID(FILE* f, int pos, const int IDL, const int IDU) {
    Emprestimo* e = NULL;
    while (pos >= 0) {
        e = pegarEmprestimo(f, pos);
        if (!e) continue;
        pos = e->Prox;
        if (e->idLivro == IDL && e->idUsuario == IDU) return e;
        free(e);
    }
    return NULL;
}

int procurarEmprestimo(FILE* f, int pos, const int IDL, const int IDU) {
    Emprestimo* e = NULL;
    while (pos >= 0) {
        e = pegarEmprestimo(f, pos);
        if (!e) continue;
        pos = e->Prox;
        if (e->idLivro == IDL && e->idUsuario == IDU) {free(e); return 1;}
        free(e);
    }
    return 0;
}

Emprestimo* pegarEmprestimo(FILE* f, const int pos) {
    Emprestimo* e = novoEmprestimo(); if (!e) return NULL;
    fseek(f, sizeof(Cabecalho) + sizeof(Emprestimo) * pos, SEEK_SET);
    fread(e, sizeof(Emprestimo), 1, f);
    return e;
}

void escreverEmprestimo(Emprestimo* e, const int pos, FILE* f) {
    e->Pos = pos;
    e->Prox = pos - 1;
    fseek(f, sizeof(Cabecalho) + sizeof(Emprestimo) * pos, SEEK_SET);
    fwrite(e, sizeof(Emprestimo), 1, f);
}

void imprimirEmprestimo_T(Emprestimo* e) {
    FILE* l = fopen("livros.bin", "rb");
    if (!l) {printf("Erro ao abrir 'livros.bin'\n"); return;}
    FILE* u = fopen("usuarios.bin", "rb");
    if (!u) {fclose(l) ;printf("Erro ao abrir 'usuarios.bin'\n"); return;}

    CabLivro cabL; pegarCabecalhoL(l, &cabL);
    Cabecalho cabU; pegarCabecalho(u, &cabU);

    Usuario* U = buscarUsuarioID(u,e->idUsuario,cabU.Inicio);
    if(!U) U = novoUsuario();
    Livro* L = procurarLivroId(l,e->idLivro,cabL.Inicio);
    if(!L) L = novoLivro();

    printf("\n====== %s(%d) - %s(%d) ======\n", U->Nome, e->idUsuario, L->Titulo, e->idLivro);
    printf("Data de Emprestimo: %s\n", e->dataEmprestimo);
    if (e->Devolvido)
        printf("Data de Devolucao: %s\n", e->dataDevolucao);
    else
        printf("Livro nao Devolvido!\n");

    free(U); free(L);
    fclose(u); fclose(l);
}

void criarCabecalhoE() {
    FILE* f = fopen("emprestimos.bin", "wb");
    if (!f) {printf("Erro ao abrir 'emprestimos.bin'"); return;}

    Cabecalho cab;
    cab.Inicio = -1;
    cab.Topo = 0;
    escreverCabecalho(f, &cab);
    fclose(f);
}

        /* USER */

void novoEmprestimo_User() {
    FILE* f = fopen("emprestimos.bin", "r+b");
    if (!f) {printf("Erro ao abrir 'emprestimos.bin'\n");}
    FILE* l = fopen("livros.bin", "r+b");
    if (!l) {fclose(f) ;printf("Erro ao abrir 'livros.bin'\n"); return;}
    FILE* u = fopen("usuarios.bin", "rb");
    if (!u) {fclose(f); fclose(l); printf("Erro ao abrir 'usuarios.bin'"); return;}

    Cabecalho cab; pegarCabecalho(f, &cab);
    CabLivro cabL; pegarCabecalhoL(l, &cabL);
    Cabecalho cabU; pegarCabecalho(u, &cabU);

    Emprestimo* e = novoEmprestimo();
    if (!e){fclose(f); fclose(l); fclose(u);printf("Erro ao criar Emprestimo\n"); return;}

    printf("====== Cadastro de Emprestimo ======\n"
           "(digite apenas o pedido e cuidado com quebras de linha)\n\n");

    printf("Digite o ID do USUARIO:\n""=> ");
    scanf("%d", &e->idUsuario); limparBuffer();

    if (!procurarIDU(u, cabU.Inicio, e->idUsuario))
        printf("Aviso: Usuario Nao Encontrado ou Nao Existente!\n\n");

    printf("Digite o ID do LIVRO:\n""=> ");
    scanf("%d", &e->idLivro); limparBuffer();
    
    Livro* livro = procurarLivroId(l, e->idLivro, cabL.Inicio);

    if (livro) 
        if(livro->Exemplares < 1) {
            printf("Livro Indisponivel!\n");
            fclose(f); fclose(l); fclose(u); free(livro); return;
        }
        else{
            livro->Exemplares -= 1;
            cabL.QuantLivros -= 1;
      
            escreverLivro(livro, livro->Pos, l); free(livro);
            escreverCabecalhoL(l, &cabL); fclose(l);
        }
    else 
        printf("Aviso: Livro Nao Encontrado ou Nao Existente!\n\n");

    printf("\nData de Emprestimo (digite 0 para usar data ATUAL)\n""=> ");
    
    int op = 0;

    scanf("%d", &op); limparBuffer();

    if (op) {
        printf("Digite a data de EMPRESTIMO(dd/mm/yyyy)\n""=> ");
        scanf("%10[^\n]", e->dataEmprestimo); limparBuffer();
    }else {
        pegarDataAtual(e->dataEmprestimo);
    }

    escreverEmprestimo(e, cab.Topo, f);
    cab.Inicio = cab.Topo++;

    free(e);
    escreverCabecalho(f, &cab);
    fclose(f); fclose(u);

    ENTER;
}

void novaDevolucao() {

    FILE* f = fopen("emprestimos.bin", "r+b");
    if (!f) {printf("Erro ao abrir 'emprestimos.bin'\n");}


    FILE* l = fopen("livros.bin", "r+b");
    if (!l) {fclose(f) ;printf("Erro ao abrir 'livros.bin'\n"); return;}

    Cabecalho cab; pegarCabecalho(f, &cab);

    int IDL = 0, IDU = 0;

    printf("\nDigite o ID do Usuario:\n""=> ");
    scanf("%d", &IDU); limparBuffer();
    printf("Digite o ID do Livro:\n""=> ");
    scanf("%d", &IDL); limparBuffer();

    if (!procurarEmprestimo(f, cab.Inicio, IDL, IDU))
        printf("Erro: Emprestimo Nao Encontrado\n");


    Emprestimo* e = buscarEmprestimoID(f,cab.Inicio, IDL, IDU);
    if (!e) { printf("Erro ao Alocar Emprestimo\n"); fclose(f); return;}

    printf("Data de Devolucao(Digite 0 para usar data ATUAL):\n");
    
    int op = 0;
    scanf("%d", &op); limparBuffer();

    if (op) {
        printf("Digite a data de DEVOLUCAO(dd/mm/yyyy):\n""=>");
        scanf("%10s", e->dataDevolucao);
    }else {
        pegarDataAtual(e->dataDevolucao);
    }
    
    CabLivro cabL; pegarCabecalhoL(l, &cabL);
    Livro* L = procurarLivroId(l, IDL, cabL.Inicio);
    if(L) {
        L->Exemplares += 1;
        cabL.QuantLivros += 1;
        escreverLivro(L, L->Pos, l);
        escreverCabecalhoL(l, &cabL);
        free(L);
    }
    
    e->Devolvido = 1;
    
    escreverEmprestimo(e, e->Pos, f);
    free(e); fclose(l); fclose(f);

    ENTER;
}

void imprimirListaEmprestimoCompleta() {
    FILE* f = fopen("emprestimos.bin", "rb");
    if (!f) {printf("Erro ao abrir 'emprestimos.bin'\n");}

    Cabecalho cab; pegarCabecalho(f, &cab);
    int n = cab.Inicio;

    Emprestimo* e = NULL;
    printf("\n====== Lista de Emprestimos (Completa) ======\n");
    while (n >= 0) {
        e = pegarEmprestimo(f,n);
        if (!e) break;
        n = e->Prox;
        imprimirEmprestimo_T(e);
        free(e);
    }
    fclose(f);
    printf("\n====== Fim da Lista ======\n");
    ENTER;
}

void imprimirListaEmprestimo() {
    FILE* f = fopen("emprestimos.bin", "rb");
    if (!f) {printf("Erro ao abrir 'emprestimos.bin'\n");}

    printf("\n====== Lista de Emprestimos ======\n");
    Cabecalho cab; pegarCabecalho(f, &cab);
    int n = cab.Inicio;

    Emprestimo* e = NULL;
    while (n >= 0) {
        e = pegarEmprestimo(f,n);
        if (!e) break;
        n = e->Prox;
        if(!e->Devolvido) imprimirEmprestimo_T(e);
        free(e);
    }
    fclose(f);
    printf("\n====== Fim da Lista ======\n");
    ENTER;
}

        /* ARQUIVO */

void iniciarArquivos(){
    criarCabecalhoU();
    criarCabecalhoL();
    criarCabecalhoE();
    printf("\n====== Arquivos Iniciados ======\n");
    ENTER;
}        

void carregarArquivo(){
    char* arq = malloc(sizeof(char) * 201);
    printf("\n====== Carregar Arquivo ======\n"
           "Digite o Diretorio do Arquivo\n=> ");
    scanf("%[^\n]", arq); limparBuffer();

    FILE* ARQ = fopen(arq, "r"); free(arq);
    if(!ARQ) return;

    char op;

    while(fscanf(ARQ, "%c", &op) != EOF){
        if (op == 'L')
            pegarARQLivro(ARQ);
        else if (op == 'U')
            pegarARQUsuario(ARQ);
        else if (op == 'E')
            pegarARQEmprestimo(ARQ);
    }

    printf("\n====== Fim da Leitura ======\n");

    ENTER;
}

void pegarARQLivro (FILE* A) {
    printf("-> Lendo (Livro)\n");
    FILE* f = fopen("livros.bin", "r+b");
    if(!f){printf("ERRO\n"); return;}

    Livro* l = novoLivro();
    if(!l) {fclose(f); printf("ERRO\n"); return;}

    CabLivro cab; pegarCabecalhoL(f, &cab);

    fscanf(A, ";%d", &l->Codigo);
    fscanf(A, ";%[^;]", l->Titulo); strcpy(l->Titulo, tiraEspacos(l->Titulo));
    fscanf(A, ";%[^;]", l->Autor); strcpy(l->Autor, tiraEspacos(l->Autor));
    fscanf(A, ";%[^;]", l->Editora); strcpy(l->Editora, tiraEspacos(l->Editora));
    fscanf(A, ";%d", &l->Edicao);
    fscanf(A, ";%d", &l->Ano);
    fscanf(A, ";%d", &l->Exemplares);

    cab.QuantLivros += l->Exemplares;

    cab.Inicio = cab.Topo++;
    escreverLivro(l, cab.Inicio, f); free(l);
    escreverCabecalhoL(f, &cab); fclose(f);
    printf("(Livro) Lido\n");
}

void pegarARQUsuario (FILE* A) {
    printf("-> Lendo (Usuario)\n");
    FILE* f = fopen("usuarios.bin", "r+b");
    if(!f){printf("ERRO2\n"); return;}

    Usuario* u = novoUsuario();
    if(!u) {fclose(f); printf("ERRO\n"); return;}

    Cabecalho cab; pegarCabecalho(f, &cab);

    fscanf(A, ";%d", &u->Codigo);
    fscanf(A, ";%[^\n]", u->Nome); strcpy(u->Nome, tiraEspacos(u->Nome));

    cab.Inicio = cab.Topo++;
    escreverUsuario(f, u, cab.Inicio); free(u);
    escreverCabecalho(f, &cab); fclose(f);
    printf("(Usuario) Lido\n");
}

void pegarARQEmprestimo (FILE* A) {
    printf("-> Lendo (Emprestimo)\n");
    FILE* f = fopen("emprestimos.bin", "r+b");
    if(!f){printf("ERRO3\n"); return;}

    Cabecalho cab; pegarCabecalho(f, &cab);
    Emprestimo* e = novoEmprestimo();

    fscanf(A, ";%d", &e->idUsuario);
    fscanf(A, ";%d", &e->idLivro);
    fscanf(A, ";%10s", e->dataEmprestimo);

    if (fscanf(A, ";%10[^\n]", e->dataDevolucao) == 1) e->Devolvido = 1;

    FILE* L  = fopen("livros.bin", "r+b");
    FILE* U = fopen("usuarios.bin", "r+b");
    CabLivro cabL; pegarCabecalhoL(L, &cabL);
    Cabecalho cabU; pegarCabecalho(U, &cabU);

    Livro* l = procurarLivroId(L, e->idLivro, cabL.Inicio);
    if(!l || !procurarIDU(U, cabU.Inicio, e->idUsuario)) {fclose(f); fclose(L); fclose(U); free(e); if(l) free(l) ;return;}

    if(!e->Devolvido){
        cabL.QuantLivros--;
        l->Exemplares--;
        escreverLivro(l,l->Pos,L);
        escreverCabecalhoL(L, &cabL);
    }
    cab.Inicio = cab.Topo++;                // Se troca essas duas linha
    escreverEmprestimo(e, cab.Inicio, f);   // Ele para de funcionar (e trocar Inicio por Topo) ?????????
    escreverCabecalho(f, &cab);
    free(l); free(e); fclose(f); fclose(L); fclose(U);
    printf("(Emprestimo) Lido\n");
}
