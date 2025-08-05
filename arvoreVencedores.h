#ifndef ARVOREVENCEDORES_H_INCLUDED
#define ARVOREVENCEDORES_H_INCLUDED

#include <stdio.h>
#include "jogo.h"

// função que implementa a intercalação de partições usando uma Árvore de Vencedores
void arvore_de_vencedores(const char* nome_base_particoes, int num_particoes, FILE* arq_saida);

#endif // ARVOREVENCEDORES_H_INCLUDED
