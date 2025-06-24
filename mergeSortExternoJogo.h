#ifndef MERGESORTEXTERNOJOGO_H_INCLUDED
#define MERGESORTEXTERNOJOGO_H_INCLUDED

#include "jogo.h" // Inclui o cabeçalho do jogo

// Função para ordenar a base de dados de jogos usando Merge Sort Externo.
// Parâmetros:
//   arq: ponteiro para o arquivo da base de dados de jogos (já aberto).
//   log: ponteiro para o arquivo de log para registrar informações.
void mergeSortExternoJogo(FILE *arq, FILE *log);

#endif // MERGESORTEXTERNOJOGO_H_INCLUDED