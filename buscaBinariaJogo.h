#ifndef BUSCABINARIAJOGO_H_INCLUDED
#define BUSCABINARIAJOGO_H_INCLUDED

#include "jogo.h" // Inclui o cabeçalho do jogo

// Realiza uma busca binária por um jogo na base de dados.
// NOTA: Esta função pressupõe que a base de dados está ORDENADA pelo código do jogo.
// Parâmetros:
//   chave: o código do jogo a ser procurado.
//   in: ponteiro para o arquivo da base de dados de jogos.
//   inicio: índice inicial do segmento de busca.
//   fim: índice final do segmento de busca.
//   log: ponteiro para o arquivo de log para registrar informações de desempenho.
// Retorna um ponteiro para o TJogo encontrado, ou NULL se o jogo não for encontrado.
TJogo *busca_binaria_jogo(int chave, FILE *in, int inicio, int fim, FILE *log);

#endif // BUSCABINARIAJOGO_H_INCLUDED