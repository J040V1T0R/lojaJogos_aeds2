#ifndef JOGO_H_INCLUDED
#define JOGO_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <time.h> // Adicionado para a função shuffle

// Definição da estrutura TJogo para representar um jogo
typedef struct Jogo {
    int cod;           // Código único do jogo
    char titulo[100];  // Título do jogo
    char genero[50];   // Gênero do jogo
    char plataforma[50]; // Plataforma(s) em que o jogo está disponível
    double preco;      // Preço do jogo
} TJogo;


// Funções para manipulação de jogos e da base de dados
// Retorna o tamanho em bytes de um registro de jogo.
int tamanho_registro_jogo();

// Cria uma nova instância de TJogo com os dados fornecidos.
// Parâmetros: cod (código), titulo, genero, plataforma, preco.
// Retorna um ponteiro para o TJogo recém-criado.
TJogo *jogo(int cod, char *titulo, char *genero, char *plataforma, double preco);


// Salva um jogo em um arquivo.
// Parâmetros: j (ponteiro para o jogo a ser salvo), out (ponteiro para o arquivo).
void salva_jogo(TJogo *j, FILE *out);


// Retorna a quantidade total de registros de jogos no arquivo.
// Parâmetro: arq (ponteiro para o arquivo).
int tamanho_arquivo_jogo(FILE *arq);


// Lê um jogo de um arquivo na posição atual do cursor.
// Parâmetro: in (ponteiro para o arquivo).
// Retorna um ponteiro para o jogo lido, ou NULL se não houver mais jogos para ler.
TJogo *le_jogo(FILE *in);


// Imprime os detalhes de um jogo no console.
// Parâmetro: j (ponteiro para o jogo a ser impresso).
void imprime_jogo(TJogo *j);


// Cria uma base de dados de jogos em um arquivo, com um número especificado de jogos.
// Os jogos são inicialmente criados com códigos sequenciais.
// Parâmetros: out (ponteiro para o arquivo de saída), tam (quantidade de jogos a serem criados).
void criarBaseJogos(FILE *out, int tam);

// Embaralha os códigos de um vetor de inteiros.
// Utilizado para criar uma base de dados desordenada.
// Parâmetros: vet (ponteiro para o array de inteiros), tam (tamanho do array).
void embaralha_jogos(int *vet, int tam);

// Imprime todos os jogos presentes na base de dados.
// Parâmetro: out (ponteiro para o arquivo da base de dados).
void imprimirBaseJogos(FILE *out);


// Compara dois jogos.
// Retorna 1 se os códigos e títulos dos jogos forem iguais, e 0 caso contrário.
// Parâmetros: j1 (ponteiro para o primeiro jogo), j2 (ponteiro para o segundo jogo).
int compara_jogos(TJogo *j1, TJogo *j2);


#endif // JOGO_H_INCLUDED