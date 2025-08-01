#include "SelecaoSubs.h"
#include <stdlib.h>
#include <string.h>

#define MAX_PARTICOES 100 // Define um máximo de partições que podem ser criadas

// Função auxiliar para encontrar o menor jogo no reservatório que não está congelado
int encontrar_menor(TJogo* reservatorio, int* congelado, int M) {
    int menor_idx = -1;
    TJogo* menor_jogo = NULL;

    for (int i = 0; i < M; ++i) {
        if (!congelado[i]) {
            if (menor_idx == -1 || reservatorio[i].cod < menor_jogo->cod) {
                menor_idx = i;
                menor_jogo = &reservatorio[i];
            }
        }
    }
    return menor_idx;
}

int selecao_por_substituicao(FILE *arq_entrada, const char* nome_base_particoes, int M) {
    rewind(arq_entrada);

    TJogo* reservatorio = (TJogo*) malloc(M * sizeof(TJogo));
    int* congelado = (int*) calloc(M, sizeof(int)); // 0 = não congelado, 1 = congelado

    // Carrega o reservatório inicial
    int pos_reservatorio = 0;
    while(pos_reservatorio < M) {
        TJogo *j = le_jogo(arq_entrada);
        if (j == NULL) break;
        reservatorio[pos_reservatorio++] = *j;
        free(j);
    }

    int num_particoes = 0;
    FILE* arq_saida = NULL;
    char nome_particao[100];

    while (pos_reservatorio > 0) {
        num_particoes++;
        sprintf(nome_particao, "%s%d.dat", nome_base_particoes, num_particoes);
        arq_saida = fopen(nome_particao, "wb");
        if (!arq_saida) {
            perror("Erro ao criar arquivo de particao");
            exit(1);
        }

        // Descongela todos os registros para o início de uma nova partição
        for(int i = 0; i < M; i++) {
            congelado[i] = 0;
        }

        while (1) {
            int menor_idx = encontrar_menor(reservatorio, congelado, pos_reservatorio);

            // Se todos no reservatório estão congelados, a partição atual termina
            if (menor_idx == -1) {
                break;
            }

            TJogo ultimo_salvo = reservatorio[menor_idx];
            salva_jogo(&ultimo_salvo, arq_saida);

            TJogo* proximo_jogo = le_jogo(arq_entrada);

            if (proximo_jogo) {
                // Se o próximo registro for menor que o último salvo, congela
                if (proximo_jogo->cod < ultimo_salvo.cod) {
                    reservatorio[menor_idx] = *proximo_jogo;
                    congelado[menor_idx] = 1;
                } else {
                    reservatorio[menor_idx] = *proximo_jogo;
                }
                free(proximo_jogo);
            } else {
                // Se não há mais registros de entrada, remove o menor do reservatório
                for(int i = menor_idx; i < pos_reservatorio - 1; i++){
                    reservatorio[i] = reservatorio[i+1];
                    congelado[i] = congelado[i+1];
                }
                pos_reservatorio--;
            }
        }
        fclose(arq_saida);
    }

    free(reservatorio);
    free(congelado);

    return num_particoes;
}