#ifndef BUSCASEQUENCIALJOGO_H_INCLUDED
#define BUSCASEQUENCIALJOGO_H_INCLUDED

#include "jogo.h" 

//realiza uma busca sequencial por um jogo na base de dados.
//retorna um ponteiro para o TJogo encontrado, ou NULL se o jogo não for encontrado.
TJogo *buscaSequencialJogo(int chave, FILE *in, FILE *log);

#endif 