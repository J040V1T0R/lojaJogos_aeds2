#ifndef TESTES_H_INCLUDED
#define TESTES_H_INCLUDED

#include <stdio.h>
#include "jogo.h"
#include "mergeSortExternoJogo.h"
#include "SelecaoSubs.h"
#include "arvoreVencedores.h"

// Protótipo da função que executa os testes de desempenho
void rodar_testes_de_desempenho(FILE **p_arq_jogos, FILE *log);

#endif // TESTES_H_INCLUDED