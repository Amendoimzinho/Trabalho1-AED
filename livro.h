#ifndef LIVRO_H
#define LIVRO_H

#include <stdio.h>

typedef struct{
    int Codigo;
    char Titulo[151];
    char Autor[201];
    char Editora[51];
    int Edicao;
    int Ano;
    int Exemplares;

    int Prox;
    int Pos; // Guarda a posição atual do livro (para edicao)
}Livro;

typedef struct {
    int Inicio;
    int Topo;
    int QuantLivros;
}CabLivro;

            /* AUXILIARES */

// Limpa o buffer de entrada até encontrar '\n' ou EOF
// Pré:  Nenhuma
// Pós:  Buffer stdin vazio até a próxima linha
void limparBuffer();

// Nao eh usada :(
void strToupper(char *str);

            /*LIVRO*/

// Aloca e inicia um livro
// Pré-condição: nenhuma
// Pós-condição: Retorna o ponteiro do livro alocado
Livro* novoLivro();

// Pega o livro da posição 'pos' no arquivo 'f'
// Pré: 'f' um arquivo valido
//      'pos' um inteiro maior ou igual a 0
// Pós: Retorna um ponteiro alocado com o livro encontrado
Livro* pegarLivro(FILE* f, int pos);

// Não usada :(
void copiarLivro(Livro* destino, const Livro* origem);

// Adiciona um livro no arquivo
// Pré: 'newLivro' deve ser um ponteiro válido
//      'f' eh um arquivo valido
//      'cab' eh um cabeçalho valido
// Pós: Livro adicionado no arquivo
void escreverLivro(Livro* newLivro, int pos, FILE* f);

// Busca um livro pelo ID
// Pré: 'f' arquivo binario aberto
//      'pos' inteiro maior ou igual a 0 (corresponde a cabeca da lista)
// Pós: Retorna ponteiro para o livro encontrado ou NULL se não encontrar
Livro* procurarLivroId(FILE* f, int id, int pos);

// Busca um livro pelo Título
// Pré: 'f' arquivo binario aberto
//      'titulo' um ponteiro para 'char' correto e não NULL
//      'pos' inteiro maior ou igual a 0 (corresponde a cabeca da lista)
// Pós: Retorna ponteiro para o livro encontrado ou NULL se não encontrar
Livro* procurarLivroTitulo(FILE* f, const char* titulo, int pos);

// Imprime o livro no terminal
// Pré-condição: 'l' eh valido
// Pós-condição: livro impresso no terminal
// (Não usar para usuário);
void imprimirLivro_T(Livro* l);

// Verifica de um ID ja esta sendo usado
// Pré-condição: nenhuma (ID eh maior ou igual a 0)
// Pós-condição: retorna 1 se encontrar um ID igual
//               e 0 se nao encontrar
int procurarID(FILE* f, int pos, int ID);

            /* CABEÇALHO */

// Le o cabeçalho 'Cablivros'
// Pré: 'cab' não nulo
//      'f' aberto par leitura
// Pós: 'cab' tera as informacoes do cabeçalho
void pegarCabecalhoL(FILE* f, CabLivro* cab);

// Cria o arquivo binário de livros com cabeçalho inicializado.
// Pré: Nenhuma
// Pós: Arquivo "livros.bin" é criado
//      Cabeçalho é escrito com valores iniciais
void criarCabecalhoL();

// Escreve o cabeçalho no arquivo
// Pré: 'f' aberto para escrita
//      'cab' devidamente alocado
// Pós: Informacoes de 'cab' escritas no inicio do arquivo 'f'
void escreverCabecalhoL(FILE* f, const CabLivro *cab);

// Nao eh usada :(
int qntLivros();

            /* USUÁRIO */

// Cadastra um livro via entrada do usuário
// Pré: 'newLivro' eh válido
// Pós: Campos do 'newLivro' preenchidos com valores digitados
void novoLivro_User();

// Abre o menu de edicao para um livro a partir de um ID
// Pré-condição: nenhuma (?)
// Pós-condição: Menu de edicao impresso no terminal
//               ou Erro se ID não existe
void editarLivroID();

// Imprime no terminal as informacoes do livro com ID enviado
// Pré-condição: nenhuma (?)
// Pós-condição: livro de codigo == 'id' imprimido ou mensagem de erro se nao encontrar o id
void imprimirLivroId();

// Imprime um livro a partir do titulo
// Pré-condição: 'titulo' valido
// Pós-condição: Imprime o livro ou erro se nao conseguir
void imprimirLivroTitulo();

// Imprime no terminal a lista completa de livros
// Pré-condição: nenhuma (?)
// Pós-condição: Lista impressa no terminal
void imprimirListaLivro();

// Imprime no terminal a quantidade de Livros e quantidade Total de exemplares
// Pré-condição: nenhuma (?)
// Pós-condição: Informacoes impressas no terminal
void imprimirQuantidade();

#endif
