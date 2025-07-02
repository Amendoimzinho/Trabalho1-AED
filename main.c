#include "livro.h"
#include "usuario.h"
#include "emprestimos.h"
#include <stdio.h>
#include <stdlib.h>

#define ENTER printf("\nPressione ENTER para voltar...");scanf("%*c");limparBuffer();

void limpar_terminal() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int abrirMenuLivros() {
    limpar_terminal();

    int op = -1;

    printf("\n====== Menu de Livros ======\n"
        " 1 - Adicionar Novo Livro\n"
        " 2 - Imprimir Livro (Titulo)\n"
        " 3 - Imprimir Livro (ID)\n"
        " 4 - Imprimir Lista Completa\n"
        " 5 - Quantidade de Livros\n"
        "-1 - Voltar\n\n"
        "=> ");

    scanf("%d", &op); limparBuffer();

    switch (op) {
        case 1 : novoLivro_User(); break;
        case 2 : imprimirLivroTitulo(); break;
        case 3 : imprimirLivroId(); break;
        case 4 : imprimirListaLivro(); break;
        case 5 : imprimirQuantidade(); break;
        case -1 : return op;
        default : printf("\nErro: Codigo Invalido\n");return op;
    }
    return 0;
}

int abrirMenuUsuarios() {
    limpar_terminal();

    int op = -1;
    printf("\n====== Menu de Usuarios ======\n"
            " 1 - Adicionar Novo Usuario\n"
            " 2 - Imprimir Lista\n"
            "-1 - Voltar\n\n"
            "=> ");

    scanf("%d", &op); limparBuffer();

    switch (op) {
        case 1  : novoUsuario_User(); break;
        case 2  : imprimirListaUsuario(); break;
        case -1 : return op;
        default : printf("Erro: Codigo Invalido\n"); return op;
    }
    return 0;
}

int abrirMenuEmprestimos() {
    limpar_terminal();

    int op = -1;
    printf("\n====== Menu de Emprestimos ======\n"
                " 1 - Criar Novo Emprestimo\n"
                " 2 - Devolver um Livro\n"
                " 3 - Imprimir Lista\n"
                "-1 - Voltar\n\n"
                "=> ");

    scanf("%d", &op); limparBuffer();

    switch (op) {
        case 1  : novoEmprestimo_User(); break;
        case 2  : novaDevolucao(); break;
        case 3  : imprimirListaEmprestimo(); break;
        case -1 : return op;
        default : printf("Erro: Codigo Invalido\n"); return op;
    }
    return 0;
}

int abrirMenuExtras() {
    limpar_terminal();

    int op = -1;
    printf("\n====== Extras ======\n"
            " 1 - Editar Livro(ID)\n"
            " 2 - Imprimir Lista Emprestimos Completa\n"
            "-1 - Voltar\n\n"
            "=> ");

    scanf("%d", &op); limparBuffer();

    switch (op) {
        case 1  : editarLivroID(); break;
        case 2  : imprimirListaEmprestimoCompleta();
        case -1 : return op;
        default : printf("Erro: Codigo Invalido\n"); return op;
    }
    return 0;
}

int abrirMenuOutros() {
    limpar_terminal();

    int op = -1;
    printf("\n====== Outros ======\n"
            " 1 - Iniciar Arquivos\n"
            " 2 - Carregar Arquivo\n"
            " 3 - Extras\n"
            "-1 - Voltar\n\n"
            "=> ");

    scanf("%d", &op); limparBuffer();

    switch (op) {
        case 1  : iniciarArquivos(); break;
        case 2  : carregarArquivo(); break;
        case 3  : while(abrirMenuExtras() != -1){}; break;
        case -1 : return op;
        default : printf("Erro: Codigo Invalido\n"); return op;
    }
    return 0;
}

int abrirMenu() {
    limpar_terminal();

    int op = -1;

    printf("\n====== Menu Inicial ======\n"
            " 1 - Menu de Livros\n"
            " 2 - Menu de Usuarios\n"
            " 3 - Menu de Emprestimos\n"
            " 4 - Outros\n"
            "-1 - Encerrar\n\n"
            "=> ");

    scanf("%d", &op); limparBuffer();

    switch (op) {
        case 1 : while (abrirMenuLivros() != -1){} break;
        case 2 : while (abrirMenuUsuarios() != -1){} break;
        case 3 : while (abrirMenuEmprestimos() != -1){} break;
        case 4 : while (abrirMenuOutros() != -1){} break;
        case -1 : return op;
        default : printf("Erro"); return op;
    }
    return 0;
}

int main() {
    while(abrirMenu() != -1){}
    return 0;
    }