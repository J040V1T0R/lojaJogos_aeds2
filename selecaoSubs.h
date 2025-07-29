#ifndef SELECAO_SUBS_H_INCLUDED
#define SELECAO_SUBS_H_INCLUDED

#include "jogo.h" // Para TJogo
#include <stdio.h> // Para FILE
#include <stdlib.h> // Para malloc, free

// Estrutura para um nó do heap (que armazena um ponteiro para TJogo)
typedef struct HeapNode {
    TJogo *item;
    // Pode adicionar um flag 'ativo'/'inativo' aqui se a estratégia for ter um único heap grande
    // mas para simplificar o heap ativo/inativo será gerenciado pela logica da selecao por substituicao
} HeapNode;

// Funções auxiliares para o Min-Heap
void swap_heap_nodes(HeapNode *a, HeapNode *b);
void min_heapify(HeapNode **heap_array, int heap_size, int idx);
void constroi_min_heap(HeapNode **heap_array, int heap_size);

// Função principal que gera as runs usando Seleção por Substituição
// arq_entrada: Arquivo original a ser lido
// temp1, temp2: Arquivos temporários para gravar as runs alternadamente
// tam_buffer_memoria: Tamanho do buffer de memória (número de jogos) para o heap
// Retorna o número total de runs geradas.
int gera_runs_selecao_substituicao(FILE *arq_entrada, FILE *temp1, FILE *temp2, int tam_buffer_memoria);

#endif // SELECAO_SUBS_H_INCLUDED