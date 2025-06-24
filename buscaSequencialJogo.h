#ifndef BUSCASEQUENCIALJOGO_H_INCLUDED
#define BUSCASEQUENCIALJOGO_H_INCLUDED

#include "jogo.h" // Inclui o cabeçalho do jogo

// Realiza uma busca sequencial por um jogo na base de dados.
// Parâmetros:
//   chave: o código do jogo a ser procurado.
//   in: ponteiro para o arquivo da base de dados de jogos.
//   log: ponteiro para o arquivo de log para registrar informações de desempenho.
// Retorna um ponteiro para o TJogo encontrado, ou NULL se o jogo não for encontrado.
TJogo *buscaSequencialJogo(int chave, FILE *in, FILE *log);


#endif // BUSCASEQUENCIALJOGO_H_INCLUDED