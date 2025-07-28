#ifndef BUSCABINARIAJOGO_H_INCLUDED
#define BUSCABINARIAJOGO_H_INCLUDED

#include "jogo.h"

//realiza uma busca binária por um jogo na base de dados.
//retorna um ponteiro para o TJogo encontrado, ou NULL se o jogo não for encontrado.
TJogo *busca_binaria_jogo(int chave, FILE *in, int inicio, int fim, FILE *log);

#endif 