#ifndef JOGO_H_INCLUDED
#define JOGO_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <time.h> // Adicionado para a função shuffle

typedef struct Jogo {
    int cod;
    char titulo[100];
    char genero[50];
    char plataforma[50];
    double preco;
    int quantidadeEmEstoque; // NOVO ATRIBUTO
} TJogo;


// Retorna tamanho do jogo em bytes
int tamanho_registro_jogo();

// Cria jogo.
TJogo *jogo(int cod, char *titulo, char *genero, char *plataforma, double preco, int quantidadeEmEstoque); // NOVO PARAMETRO


// Salva jogo no arquivo out, na posicao atual do cursor
void salva_jogo(TJogo *j, FILE *out);


// retorna a quantidade de registros no arquivo
int tamanho_arquivo_jogo(FILE *arq);


// Le um jogo do arquivo in na posicao atual do cursor
// Retorna um ponteiro para jogo lido do arquivo
TJogo *le_jogo(FILE *in);


// Imprime jogo
void imprime_jogo(TJogo *j);


// Cria a base de dados de jogos
void criarBaseJogos(FILE *out, int tam);

// Embaralha base de dados de jogos
void embaralha_jogos(int *vet, int tam);

// Imprime a base de dados de jogos
void imprimirBaseJogos(FILE *out);


// Compara dois jogos
// Retorna 1 se os valores dos atributos de ambos forem iguais
// e 0 caso contrario
int compara_jogos(TJogo *j1, TJogo *j2);

// NOVO: Adiciona um jogo individualmente ao estoque (apenas ao final do arquivo)
void adiciona_jogo_ao_estoque(TJogo *j, FILE *out);

// NOVO: Atualiza a quantidade em estoque de um jogo existente no arquivo
void atualiza_quantidade_estoque_jogo(int cod_jogo, int nova_quantidade, FILE *arq_jogos);


#endif // JOGO_H_INCLUDED