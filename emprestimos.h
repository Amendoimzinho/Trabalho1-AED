#ifndef EMPRESTIMOS_H
#define EMPRESTIMOS_H

#define TAM 11

typedef struct noemprestimo {
    int idLivro;
    int idUsuario;
    char dataEmprestimo[TAM];
    char dataDevolucao[TAM];
    int Devolvido;

    int Prox;
    int Pos;
}Emprestimo;

                /* AUXILIARES */

// Pega a data atua no formato dd/mm/yyyy
// Pré-condição: 'data' ponteiro para vetor de tamanho minimo 11
// Pós-condição: 'data' preenchida com a data atual
void pegarDataAtual(char* data);

char* tiraEspacos(char* str);

                /* EMPRESTIMOS */

// Aloca e inicia um Emprestimo
// Pré-condição: nenhuma(cuidado para nao perder o retorno)
// Pós-condição: Retorna o ponteiro para o Emprestimo alocado
Emprestimo* novoEmprestimo();

// Busca um Empretimo no arquivo usando os IDs
// Pré-condição: 'f' aberto para leitura
//               'pos' eh onde ele vvai comecar a procurar
//               'IDL' e 'IDU' sao inteiros(?)
// Pós-condição: Retorna o ponteiro para o Emprestimo alocado encontrado
//               ou NULL se nao encontrar
Emprestimo* buscarEmprestimoID(FILE* f, int pos, int IDL, int IDU);


// Verifica se o Emprestimo esta no arquivo
// Pré-condição:'f' aberto para leitura
//               'pos' eh onde ele vvai comecar a procurar
//               'IDL' e 'IDU' sao inteiros(?)
// Pós-condição: Retorna 1 se encontrar ou 0 se nao encontrar
int procurarEmprestimo(FILE* f, int pos,  int IDL,  int IDU);

// Pega e aloca um Emprestimo do arquivo
// Pré-condição: 'f' aberto para leitura
//               'pos' eh um valor que esteja na lista do arquivo
//               (nao sei o q acontece se ele for maior)
// Pós-condição: Retorna o ponteiro para o Emprestimo alocado
//               ou NULL se nao Conseguir
Emprestimo* pegarEmprestimo(FILE* f, int pos);

// Escreve um Emprestimo no arquivo 'f' na posicao 'pos'
// Pré-condição: 'f' aberto para leitura
//               'e' um ponteiro valido
//               'pos' um inteiro maior igual a 0
//               (nao testei extremos para pos mas acho que)
// Pós-condição: Emprestimo escrito no arquivo na posicao enviada 
void escreverEmprestimo(Emprestimo* e, int pos, FILE* f);


// Imprime no Terminal um Emprestimo
// Pré-condição: 'e' ponteiro valido
// Pós-condição: Conteudo de 'e' impresso no terminal
void imprimirEmprestimo_T(Emprestimo* e);

                 /* USUÁRIO */

// Comeca o cadastro de um novo Emprestimo pelo User
// Pré-condição: nenhuma(?)
// Pós-condição: Novo Emprestimo cadastrado
void novoEmprestimo_User();

// Cadastra a devolucao de um Emprestimo
// Pré-condição: nenhuma(?)
// Pós-condição: Devolucao realizada 
void novaDevolucao();

// Imprime TODOS os Emprestimos
// Pré-condição: nenhuma(?)
// Pós-condição: Lista completa de Emprestimos impressa no terminal
void imprimirListaEmprestimoCompleta();

// Imprime os Emprestimos sem devolucao
// Pré-condição: nenhuma(?)
// Pós-condição: Lista de Emprestimos nao devolvidos impressa no terminal
void imprimirListaEmprestimo();

                /* CABECALHO */

// Inicia o Arquivo 'emprestimos.bin'
// Pré-condição: nenhuma
// Pós-condição: 'emprestimos.bin' iniciado com o cabecalho criado
void criarCabecalhoE();

                /* ARQUIVO */

void iniciarArquivos();

// Carrega o arquivo dado pelo User
// Pré-condição: arquivo dado formatado corretamente
// Pós-condição: Informacoes lidas e carregadas nas listas
void carregarArquivo();

// Quando chamada le um Livro do arquivo 'A'
// Pré-condição: 'A' aberto para leitura e esta na posicao correta
// Pós-condição: Livro eh lido e colocado na lista
void pegarARQLivro (FILE* A);

// Quando chamada le um Usuario do arquivo 'A'
// Pré-condição: 'A' aberto para leitura e esta na posicao correta
// Pós-condição: Usuario eh lido e colocado na lista
void pegarARQUsuario (FILE* A);

// Quando chamada le um Emprestimo do arquivo 'A'
// Pré-condição: 'A' aberto para leitura e esta na posicao correta
// Pós-condição: Emprestimo eh lido e colocado na lista
void pegarARQEmprestimo (FILE* A);
#endif
