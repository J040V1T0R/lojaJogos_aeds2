#include "selecaoSubs.h"
#include <string.h> // Para memcpy

// Função auxiliar para trocar dois nós do heap
void swap_heap_nodes(HeapNode *a, HeapNode *b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

// Restaura a propriedade de min-heap para o subárvore com raiz no índice 'idx'
void min_heapify(HeapNode **heap_array, int heap_size, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    // Se o filho esquerdo existe e é menor que o pai
    if (left < heap_size && heap_array[left]->item->cod < heap_array[smallest]->item->cod) {
        smallest = left;
    }

    // Se o filho direito existe e é menor que o menor até agora
    if (right < heap_size && heap_array[right]->item->cod < heap_array[smallest]->item->cod) {
        smallest = right;
    }

    // Se o menor não é o pai, troca e continua heapify
    if (smallest != idx) {
        swap_heap_nodes(heap_array[idx], heap_array[smallest]); // Troca os ponteiros para os nós
        min_heapify(heap_array, heap_size, smallest);
    }
}

// Constrói um min-heap a partir de um array de HeapNode*
void constroi_min_heap(HeapNode **heap_array, int heap_size) {
    for (int i = heap_size / 2 - 1; i >= 0; i--) {
        min_heapify(heap_array, heap_size, i);
    }
}

// Função principal que gera as runs usando Seleção por Substituição
// Retorna o número total de runs geradas.
int gera_runs_selecao_substituicao(FILE *arq_entrada, FILE *temp1, FILE *temp2, int tam_buffer_memoria) {
    HeapNode **heap_array = (HeapNode **)malloc(tam_buffer_memoria * sizeof(HeapNode *));
    if (!heap_array) {
        perror("Erro de alocacao para o heap (selecaoSubs)");
        exit(1);
    }

    // Alocar os nós do heap
    for (int i = 0; i < tam_buffer_memoria; i++) {
        heap_array[i] = (HeapNode *)malloc(sizeof(HeapNode));
        if (!heap_array[i]) {
            perror("Erro de alocacao para HeapNode");
            // Liberar o que ja foi alocado
            for (int j = 0; j < i; j++) free(heap_array[j]);
            free(heap_array);
            exit(1);
        }
        heap_array[i]->item = NULL; // Inicializa com NULL
    }

    FILE *current_temp_out_file = temp1; // Começa salvando runs no temp1
    int num_runs_geradas = 0;
    int heap_size = 0; // Elementos ativos no heap
    int elementos_proxima_run_buffer = 0; // Elementos "congelados" para a próxima run

    // Array auxiliar para os elementos "congelados" para a próxima run
    HeapNode **next_run_elements = (HeapNode **)malloc(tam_buffer_memoria * sizeof(HeapNode *));
    if (!next_run_elements) {
        perror("Erro de alocacao para next_run_elements");
        // Liberar heap_array
        for (int j = 0; j < tam_buffer_memoria; j++) free(heap_array[j]);
        free(heap_array);
        exit(1);
    }

    // 1. Fase de preenchimento inicial do heap
    for (int i = 0; i < tam_buffer_memoria; i++) {
        TJogo *j = le_jogo(arq_entrada);
        if (j) {
            heap_array[i]->item = j;
            heap_size++;
        } else {
            break; // Fim do arquivo de entrada antes de preencher o heap
        }
    }
    constroi_min_heap(heap_array, heap_size); // Constrói o heap com os elementos iniciais

    TJogo *ultimo_escrito_na_run = NULL; // Último jogo escrito na run atual

    // Loop principal da Seleção por Substituição
    while (heap_size > 0 || elementos_proxima_run_buffer > 0) {
        if (heap_size == 0) { // A run atual terminou
            // Inicia uma nova run
            num_runs_geradas++;
            ultimo_escrito_na_run = NULL; // Reseta o último escrito

            // Troca o arquivo de saída temporário
            if (current_temp_out_file == temp1) {
                current_temp_out_file = temp2;
            } else {
                current_temp_out_file = temp1;
            }

            // Move os elementos "congelados" para o heap ativo
            for (int i = 0; i < elementos_proxima_run_buffer; i++) {
                heap_array[heap_size++] = next_run_elements[i];
            }
            elementos_proxima_run_buffer = 0; // Limpa o buffer de congelados
            constroi_min_heap(heap_array, heap_size); // Reconstrói o heap
            if (heap_size == 0) break; // Se não há mais elementos em nenhuma parte, sai
        }

        // Extrai o menor elemento (raiz) do heap ativo
        HeapNode *min_node = heap_array[0];
        salva_jogo(min_node->item, current_temp_out_file); // Salva na run atual
        ultimo_escrito_na_run = min_node->item; // Atualiza o último escrito

        // Tenta ler um novo elemento do arquivo de entrada
        TJogo *novo_jogo_lido = le_jogo(arq_entrada);

        if (novo_jogo_lido == NULL) { // Fim do arquivo de entrada
            // Se o arquivo de entrada acabou, o heap só tem elementos da run atual.
            // Apenas removemos o elemento e re-heapificamos.
            free(min_node->item); // Libera o jogo que foi salvo
            min_node->item = NULL; // Marca como vazio
            swap_heap_nodes(heap_array[0], heap_array[heap_size - 1]); // Troca com o último
            heap_size--; // Reduz o tamanho do heap ativo
            min_heapify(heap_array, heap_size, 0); // Re-heapify
        } else {
            // Regra da Seleção por Substituição:
            // Se o novo_jogo_lido pode fazer parte da run atual (é >= ao último escrito)
            if (novo_jogo_lido->cod >= ultimo_escrito_na_run->cod) {
                free(min_node->item); // Libera o jogo que foi salvo
                min_node->item = novo_jogo_lido; // Substitui o elemento extraído pelo novo
                min_heapify(heap_array, heap_size, 0); // Re-heapify
            } else { // O novo_jogo_lido não pode fazer parte da run atual (congelar)
                free(min_node->item); // Libera o jogo que foi salvo
                min_node->item = NULL; // Marca como vazio
                next_run_elements[elementos_proxima_run_buffer++] = min_node; // Move o nó para o buffer de congelados
                
                swap_heap_nodes(heap_array[0], heap_array[heap_size - 1]); // Troca com o último ativo
                heap_array[heap_size - 1] = NULL; // Ultima posição agora esta livre
                heap_size--; // Reduz o tamanho do heap ativo
                min_heapify(heap_array, heap_size, 0); // Re-heapify
            }
        }
    } // Fim do while principal

    // Libera a memória alocada para os nós do heap
    for (int i = 0; i < tam_buffer_memoria; i++) {
        if (heap_array[i] != NULL && heap_array[i]->item != NULL) {
             free(heap_array[i]->item);
        }
        if (heap_array[i] != NULL) {
            free(heap_array[i]);
        }
    }
    free(heap_array);
    free(next_run_elements);

    // Garante que a última run também seja contada se houver elementos congelados não processados
    if (elementos_proxima_run_buffer > 0) {
        num_runs_geradas++;
    }

    return num_runs_geradas;
}