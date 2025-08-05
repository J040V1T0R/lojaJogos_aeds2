#include "mergeSortExternoJogo.h"
#include "jogo.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <io.h> 

#define TAM_BLOCO_REGISTROS 1000

int compara_jogos_qsort(const void *a, const void *b) {
    TJogo *j1 = (TJogo *)a;
    TJogo *j2 = (TJogo *)b;
    return j1->cod - j2->cod;
}

int le_bloco_jogos(FILE *arq, TJogo *bloco, int tam_bloco) {
    int i = 0;
    for (i = 0; i < tam_bloco; i++) {
        TJogo *j = le_jogo(arq);
        if (j == NULL) {
            break;
        }
        memcpy(&bloco[i], j, sizeof(TJogo));
        free(j);
    }
    return i;
}

void salva_bloco_jogos(FILE *arq, TJogo *bloco, int num_jogos) {
    for (int i = 0; i < num_jogos; i++) {
        salva_jogo(&bloco[i], arq);
    }
    fflush(arq);
}

void intercala_arquivos_corrigido(FILE *entrada1, FILE *entrada2, FILE *saida) {
    rewind(entrada1);
    rewind(entrada2);

    TJogo *j1 = le_jogo(entrada1);
    TJogo *j2 = le_jogo(entrada2);

    while (j1 != NULL && j2 != NULL) {
        if (j1->cod < j2->cod) {
            salva_jogo(j1, saida);
            free(j1);
            j1 = le_jogo(entrada1);
        } else {
            salva_jogo(j2, saida);
            free(j2);
            j2 = le_jogo(entrada2);
        }
    }

    while (j1 != NULL) {
        salva_jogo(j1, saida);
        free(j1);
        j1 = le_jogo(entrada1);
    }
    while (j2 != NULL) {
        salva_jogo(j2, saida);
        free(j2);
        j2 = le_jogo(entrada2);
    }
    fflush(saida);
}


void mergeSortExternoJogo(FILE *arq, FILE *log) {
    clock_t inicioT, fimT;
    double total;
    inicioT = clock();

    int num_jogos = tamanho_arquivo_jogo(arq);
    if (num_jogos <= 1) {
        printf("   - Base de dados ja ordenada ou vazia.\n");
        return;
    }

    // cria as partições iniciais ordenadas (runs)
    rewind(arq);
    int num_particoes = 0;
    TJogo *bloco_memoria = (TJogo *)malloc(TAM_BLOCO_REGISTROS * sizeof(TJogo));

    while (!feof(arq)) {
        int lidos = le_bloco_jogos(arq, bloco_memoria, TAM_BLOCO_REGISTROS);
        if (lidos == 0) break;

        qsort(bloco_memoria, lidos, sizeof(TJogo), compara_jogos_qsort);

        char nome_particao[30];
        sprintf(nome_particao, "particao_%d.dat", num_particoes);
        FILE *p_saida = fopen(nome_particao, "wb");
        salva_bloco_jogos(p_saida, bloco_memoria, lidos);
        fclose(p_saida);
        num_particoes++;
    }
    free(bloco_memoria);


    // intercala as partições em passadas sucessivas
    int k = 0; 
    while (num_particoes > 1) {
        int idx_saida = 0;
        for (int i = 0; i < num_particoes; i += 2) {
            char nome_p1[30], nome_p2[30], nome_saida[30];
            sprintf(nome_p1, "particao_%d.dat", i);

        
            if (i + 1 >= num_particoes) {
                sprintf(nome_saida, "temp_part_%d_%d.dat", k, idx_saida);
                rename(nome_p1, nome_saida);
            }
        
            else {
                sprintf(nome_p2, "particao_%d.dat", i + 1);
                sprintf(nome_saida, "temp_part_%d_%d.dat", k, idx_saida);

                FILE *f_p1 = fopen(nome_p1, "rb");
                FILE *f_p2 = fopen(nome_p2, "rb");
                FILE *f_saida = fopen(nome_saida, "wb");

                intercala_arquivos_corrigido(f_p1, f_p2, f_saida); 

                fclose(f_p1);
                fclose(f_p2);
                fclose(f_saida);

                remove(nome_p1);
                remove(nome_p2);
            }
            idx_saida++;
        }
        
        // renomeia os arquivos temporários para serem as novas "particao_N.dat" da próxima passada
        for(int j = 0; j < idx_saida; j++){
            char nome_antigo[30], nome_novo[30];
            sprintf(nome_antigo, "temp_part_%d_%d.dat", k, j);
            sprintf(nome_novo, "particao_%d.dat", j);
            rename(nome_antigo, nome_novo);
        }
        num_particoes = idx_saida; // O número de partições dimin.
        k++;
    }


    // copia o arquivo final (agora "particao_0.dat") de volta para o original
    rewind(arq);
    _chsize(fileno(arq), 0); 
    
    FILE *arq_final_ordenado = fopen("particao_0.dat", "rb");
    TJogo *jogo_final;
    while ((jogo_final = le_jogo(arq_final_ordenado)) != NULL) {
        salva_jogo(jogo_final, arq);
        free(jogo_final);
    }
    
    fclose(arq_final_ordenado);
    remove("particao_0.dat"); // limpa o último arquivo temporário

    fimT = clock();
    total = (double)(fimT - inicioT) / CLOCKS_PER_SEC;

    printf("   - Base ordenada em %.4f segundos.\n", total);
    fprintf(log, "Tempo total (Merge Sort Externo): %f segundos\n", total);
    fflush(log);
}
