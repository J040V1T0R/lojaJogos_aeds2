#ifndef MERGESORTEXTERNOJOGO_H_INCLUDED
#define MERGESORTEXTERNOJOGO_H_INCLUDED

#include "jogo.h"
#include <stdio.h> // Necessário para FILE

// Inclui o cabeçalho para Seleção por Substituição (necessário para usar gera_runs_selecao_substituicao)
#include "selecaoSubs.h"

// NOVO: Enum para definir o método de geração de runs
typedef enum {
    RUN_GENERATION_OLD_WAY, // Método antigo (qsort em blocos)
    RUN_GENERATION_REPLACEMENT_SELECTION // Seleção por Substituição
} RunGenerationMethod;

// Função para ordenar a base de dados de jogos usando Merge Sort Externo.
// NOVO PARÂMETRO: method_run_gen para escolher o método de geração de runs.
void mergeSortExternoJogo(FILE *arq, FILE *log, RunGenerationMethod method_run_gen);

// ... (outros protótipos de funções auxiliares se necessário) ...

#endif // MERGESORTEXTERNOJOGO_H_INCLUDED