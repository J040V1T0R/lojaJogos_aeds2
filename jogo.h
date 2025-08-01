#ifndef JOGO_H_INCLUDED
#define JOGO_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct Jogo {
    int cod;
    char titulo[100];
    char genero[50];
    char plataforma[50];
    double preco;
    int quantidadeEmEstoque;
} TJogo;


int tamanho_registro_jogo();

TJogo *jogo(int cod, char *titulo, char *genero, char *plataforma, double preco, int quantidadeEmEstoque);

void salva_jogo(TJogo *j, FILE *out);

int tamanho_arquivo_jogo(FILE *arq);


TJogo *le_jogo(FILE *in);

void imprime_jogo(TJogo *j);

void criarBaseJogos(FILE *out, int tam);

void embaralha_jogos(int *vet, int tam);

void imprimirBaseJogos(FILE *out);


int compara_jogos(TJogo *j1, TJogo *j2);

void adiciona_jogo_ao_estoque(TJogo *j, FILE *out);


void atualiza_quantidade_estoque_jogo(int cod_jogo, int nova_quantidade, FILE *arq_jogos);

TJogo *cria_jogo_manual();

#endif 