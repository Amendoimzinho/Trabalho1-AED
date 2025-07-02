#include "usuario.h"
#include "livro.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ENTER printf("\nPressione ENTER para voltar...");scanf("%*c")

        /* USUÁRIO */

Usuario* novoUsuario() {
    Usuario* u = malloc(sizeof(Usuario)); if (!u) return NULL;

    u->Codigo = -1;
    memset(u->Nome, 0, sizeof(u->Nome));
    u->Prox = -1;
    u->Pos = -1;

    return u;
}

Usuario* pegarUsuario(FILE* f, const int pos) {
    Usuario* u = novoUsuario(); if (!u) return NULL;
    fseek(f, sizeof(Cabecalho) + sizeof(Usuario) * pos, SEEK_SET);
    fread(u, sizeof(Usuario), 1, f);
    return u;
}

void escreverUsuario(FILE* f, Usuario *usuario, const int pos) {
    usuario->Pos = pos;
    usuario->Prox = pos - 1;
    fseek(f, sizeof(Cabecalho) + sizeof(Usuario) * pos, SEEK_SET);
    fwrite(usuario, sizeof(Usuario), 1, f);
}

void imprimirUsuario_T(Usuario* u) {
    printf("\nID:%d - %s \n", u->Codigo, u->Nome);
}

Usuario* buscarUsuarioID(FILE* f, const int ID, int pos) {
    Usuario* u = NULL;
    while (pos >= 0) {
        u = pegarUsuario(f, pos);
        if (!u) break;
        pos = u->Prox;
        if (u->Codigo == ID) return u;
        free(u);
    }
    return NULL;
}

int procurarIDU(FILE* f, int pos, const int ID) {
    Usuario* u = NULL;
    int res = 0;
    while (pos >= 0 && !res) {
        u = pegarUsuario(f, pos); if (!u) break;
        if (u->Codigo == ID) res = 1;
        pos = u->Prox;
        free(u);
    }
    return res;
}

        /* CABECALHO */

void criarCabecalhoU() {
    FILE* f = fopen("usuarios.bin", "wb");
    if (!f) {printf("Erro ao abrir 'usuarios.bin'"); return;}

    Cabecalho cab;
    cab.Inicio = -1;
    cab.Topo = 0;
    escreverCabecalho(f, &cab);
    fclose(f);
}

void pegarCabecalho(FILE* f, Cabecalho* cab) {
    fseek(f, 0, SEEK_SET);
    fread(cab, sizeof(Cabecalho), 1, f);
}

void escreverCabecalho(FILE* f, const Cabecalho* cab) {
    fseek(f, 0, SEEK_SET);
    fwrite(cab, sizeof(Cabecalho), 1, f);
}

        /* USER */

void novoUsuario_User() {
    FILE* f = fopen("usuarios.bin", "r+b");
    if (!f) {printf("Erro ao abrir 'usuarios.bin'"); return;}

    Cabecalho cab; pegarCabecalho(f, &cab);
    Usuario* u = novoUsuario();
    if (!u) {fclose(f); printf("Erro ao criar Usuario\n"); return;}

    printf("===== Cadastrando Usuario =====\n"
                 "(digite apenas o pedido e cuidado com quebras de linha)\n\n");

    printf("Digite o Nome do usuario:\n=> ");
    scanf("%50[^\n]", u->Nome); limparBuffer();

    printf("Digite o Codigo do usuario:\n=> ");
    scanf("%d", &u->Codigo); limparBuffer();
    while (u->Codigo < 0 || procurarIDU(f, cab.Inicio, u->Codigo)) {
        printf("Codigo invalido ou ja existente, digite outro:\n=> ");
        scanf("%d", &(u->Codigo)); limparBuffer();
    }

    escreverUsuario(f, u, cab.Topo);
    free(u);

    cab.Inicio = cab.Topo++;
    escreverCabecalho(f, &cab);

    fclose(f);

    ENTER;
}

void imprimirListaUsuario() {
    FILE* f = fopen("usuarios.bin", "rb");
    if (!f) {printf("Erro ao abrir usuarios.bin\n"); return;}

    Cabecalho cab; pegarCabecalho(f, &cab);
    int n = cab.Inicio;
    Usuario* u = NULL;

    printf("\n====== Lista de Usuarios ======\n");

    while (n >= 0) {
        u = pegarUsuario(f, n);
        if (!u) break;
        n = u->Prox;
        imprimirUsuario_T(u);
        free(u);
    }
    fclose(f);
    printf("\n====== Fim da Lista ======\n");

    ENTER;
}
