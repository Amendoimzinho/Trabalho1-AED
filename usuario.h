#ifndef USUARIO_H
#define USUARIO_H

#include <stdio.h>

typedef struct noUser {
    int Codigo;
    char Nome[51];

    int Prox;
    int Pos;
}Usuario;


typedef struct {
    int Inicio;
    int Topo;
}Cabecalho;

            /* USUÁRIO */

// Aloca e Inicializa um Usuario
// Pré-condição: nenhuma
// Pós-condição: Retorna o ponteiro alocado
//               ou NULL se nao conseguir
Usuario* novoUsuario();

// Pega um Usuario do arquivo 'f' da posicao 'pos'
// Pré-condição: 'f' aberto para leitura
//               'pos' eh um valor que esteja na lista do arquivo
// Pós-condição: Retorna o ponteiro para o Usuario alocado
//               ou NULL se nao Conseguir
Usuario* pegarUsuario(FILE* f, int pos);

// Escreve um Usuario no arquivo 'f' na posicao 'pos'
// Pré-condição: 'f' aberto para leitura
//               'usuario' um ponteiro valido
//               'pos' um inteiro maior igual a 0
// Pós-condição: Usuario escrito no arquivo 'f' na posicao enviada 
void escreverUsuario(FILE* f, Usuario *usuario, int pos);

// Comeca o cadastro de um novo Usuario pelo User
// Pré-condição: nenhuma(?)
// Pós-condição: Novo Usuario cadastrado
void novoUsuario_User();

// Busca um Usuario no arquivo usando o ID
// Pré-condição: 'f' aberto para leitura
//               'pos' eh onde ele vvai comecar a procurar
//               'ID' um inteiro(?)
// Pós-condição: Retorna o ponteiro para o Usuario alocado encontrado
//               ou NULL se nao encontrar
Usuario* buscarUsuarioID(FILE* f, int ID, int pos);

// Imprime a lista de Usuarios
// Pré-condição: nenhuma(?)
// Pós-condição: Lista de Usuarios impressa no terminal
void imprimirListaUsuario();

            /* CABEÇALHO */

// Inicia o Arquivo 'usuarios.bin'
// Pré-condição: nenhuma
// Pós-condição: 'usuarios.bin' iniciado com o cabecalho criado
void criarCabecalhoU();

// Pega o Cabecalho do arquivo 'f'
// Pré-condição: 'f' aberto para leitura
//               'cab' um ponteiro valido
// Pós-condição: '*cab' com o Cabecalho do arquivo 'f' 
void pegarCabecalho(FILE* f, Cabecalho* cab);

// Escreve o Cabecalho 'cab' no arquivo 'f'
// Pré-condição: 'f' aberto para escrita
//               'cab' um ponteiro valido
// Pós-condição: Cabecalho d 'cab' escrito no arquivo 'f'
void escreverCabecalho(FILE* f, const Cabecalho* cab);

// Verifica se existe um Usuario com ID == 'ID' existe
// Pré-condição: 'f' aberto para leitura 
//               'pos' eh a posicao onde inicia a busca
// Pós-condição: Retorna 1 se encontrar e 0 se nao encontrar
int procurarIDU(FILE* f, int pos, int ID);

#endif
