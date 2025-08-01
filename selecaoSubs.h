#ifndef SELECAOSUBS_H_INCLUDED
#define SELECAOSUBS_H_INCLUDED

#include <stdio.h>
#include "jogo.h"

// Função que implementa a Seleção por Substituição para criar partições ordenadas
// arq_entrada: arquivo de dados original
// nome_base_particoes: prefixo para os nomes dos arquivos de partição (ex: "part")
// M: tamanho do reservatório em memória
// retorna o número de partições criadas
int selecao_por_substituicao(FILE *arq_entrada, const char* nome_base_particoes, int M);

#endif // SELECAOSUBS_H_INCLUDED