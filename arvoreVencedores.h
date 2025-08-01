#ifndef ARVOREVENCEDORES_H_INCLUDED
#define ARVOREVENCEDORES_H_INCLUDED

#include <stdio.h>
#include "jogo.h"

// Função que implementa a intercalação de partições usando uma Árvore de Vencedores
// nome_base_particoes: prefixo dos nomes dos arquivos de partição
// num_particoes: número de partições a serem intercaladas
// arq_saida: arquivo final ordenado
void arvore_de_vencedores(const char* nome_base_particoes, int num_particoes, FILE* arq_saida);

#endif // ARVOREVENCEDORES_H_INCLUDED