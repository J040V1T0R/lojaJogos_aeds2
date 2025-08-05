#ifndef SELECAOSUBS_H_INCLUDED
#define SELECAOSUBS_H_INCLUDED

#include <stdio.h>
#include "jogo.h"

// função que implementa a Seleção por Substituição para criar partições ordenadas
int selecao_por_substituicao(FILE *arq_entrada, const char* nome_base_particoes, int M);

#endif 
