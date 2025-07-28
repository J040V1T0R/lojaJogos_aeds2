#include "mergeSortExternoJogo.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <io.h> 
#include <stdio.h> 

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
}

void intercala_arquivos(FILE *arq_entrada_1, FILE *arq_entrada_2, FILE *arq_saida, int tam_run) {
    TJogo *j1 = NULL;
    TJogo *j2 = NULL;
    int count1 = 0;
    int count2 = 0;

    rewind(arq_entrada_1);
    rewind(arq_entrada_2);
    rewind(arq_saida);

    j1 = le_jogo(arq_entrada_1);
    j2 = le_jogo(arq_entrada_2);

    while (j1 != NULL || j2 != NULL) {
        if ((j1 == NULL || count1 == tam_run) && (j2 != NULL && count2 < tam_run)) {
            salva_jogo(j2, arq_saida);
            free(j2);
            j2 = le_jogo(arq_entrada_2);
            count2++;
        }
        else if ((j2 == NULL || count2 == tam_run) && (j1 != NULL && count1 < tam_run)) {
            salva_jogo(j1, arq_saida);
            free(j1);
            j1 = le_jogo(arq_entrada_1);
            count1++;
        }
        else if (j1 != NULL && j2 != NULL && count1 < tam_run && count2 < tam_run) {
            if (j1->cod < j2->cod) {
                salva_jogo(j1, arq_saida);
                free(j1);
                j1 = le_jogo(arq_entrada_1);
                count1++;
            } else {
                salva_jogo(j2, arq_saida);
                free(j2);
                j2 = le_jogo(arq_entrada_2);
                count2++;
            }
        }
    
        if ((count1 == tam_run && count2 == tam_run) && (j1 != NULL || j2 != NULL)) {
            count1 = 0;
            count2 = 0;
        }
    }
    
    if (j1) free(j1);
    if (j2) free(j2);
}


void mergeSortExternoJogo(FILE *arq, FILE *log) {
    clock_t inicioT, fimT;
    double total;
    inicioT = clock();

    int num_jogos = tamanho_arquivo_jogo(arq);
    if (num_jogos <= 1) {
        fprintf(log, "\nMerge Sort Externo Jogo: Arquivo ja ordenado ou vazio.\n");
        fimT = clock();
        total = (double)(fimT - inicioT)/CLOCKS_PER_SEC;
        fprintf(log, "\nTempo Merge Sort Externo Jogo: %f ", total);
        return;
    }

    FILE *temp1 = NULL, *temp2 = NULL; 
    TJogo *bloco_memoria = NULL; 


    if ((temp1 = fopen("temp1.dat", "w+b")) == NULL) {
        fprintf(log, "Erro ao criar arquivo temporario temp1.dat\n");
        exit(1);
    }
    if ((temp2 = fopen("temp2.dat", "w+b")) == NULL) {
        fprintf(log, "Erro ao criar arquivo temporario temp2.dat\n");
        fclose(temp1); 
        exit(1);
    }

    bloco_memoria = (TJogo *)malloc(TAM_BLOCO_REGISTROS * sizeof(TJogo));
    if (bloco_memoria == NULL) {
        fprintf(log, "Erro de alocacao de memoria para o bloco.\n");
        fclose(temp1);
        fclose(temp2);
        exit(1);
    }

    
    rewind(arq);
    int i = 0;
    int num_runs = 0;
    while (i < num_jogos) {
        int lidos = le_bloco_jogos(arq, bloco_memoria, TAM_BLOCO_REGISTROS);
        if (lidos == 0) break;

        qsort(bloco_memoria, lidos, sizeof(TJogo), compara_jogos_qsort);

        if (num_runs % 2 == 0) {
            salva_bloco_jogos(temp1, bloco_memoria, lidos);
        } else {
            salva_bloco_jogos(temp2, bloco_memoria, lidos);
        }
        i += lidos;
        num_runs++;
    }

    //intercala as runs até ter um único arquivo ordenado
    int tam_run = TAM_BLOCO_REGISTROS;
    char nome_temp1_atual[20] = "temp1.dat";
    
    char nome_temp_saida_prox[20];
    char nome_temp_entrada_1[20];
    char nome_temp_entrada_2[20];

    while (tam_run < num_jogos) {
    
        if (strcmp(nome_temp1_atual, "temp1.dat") == 0) {
            strcpy(nome_temp_entrada_1, "temp1.dat");
            strcpy(nome_temp_entrada_2, "temp2.dat");
            strcpy(nome_temp_saida_prox, "temp_out.dat");
        } else {
            strcpy(nome_temp_entrada_1, "temp_out.dat"); 
            strcpy(nome_temp_entrada_2, "temp1.dat"); 
            strcpy(nome_temp_saida_prox, "temp2.dat"); 
        }



        FILE *current_temp1 = fopen(nome_temp_entrada_1, "r+b");
        FILE *current_temp2 = fopen(nome_temp_entrada_2, "r+b"); 
        FILE *current_temp_saida = fopen(nome_temp_saida_prox, "w+b");

        if (!current_temp1 || !current_temp2 || !current_temp_saida) {
            fprintf(log, "Erro ao reabrir arquivos temporarios para intercalacao.\n");
            if (current_temp1) fclose(current_temp1);
            if (current_temp2) fclose(current_temp2);
            if (current_temp_saida) fclose(current_temp_saida);
            free(bloco_memoria);
            exit(1);
        }

        intercala_arquivos(current_temp1, current_temp2, current_temp_saida, tam_run);

        fclose(current_temp1);
        fclose(current_temp2);
        fclose(current_temp_saida);

        //prepara para a próxima iteração
        
        strcpy(nome_temp1_atual, nome_temp_saida_prox);
        
        
        remove("temp1.dat"); 
        remove("temp2.dat");
        rename(nome_temp_saida_prox, "temp1.dat"); 

    
        if ((temp1 = fopen("temp1.dat", "r+b")) == NULL) {
             fprintf(log, "Erro ao reabrir arquivo temporario temp1.dat\n");
             free(bloco_memoria);
             exit(1);
        }
        if ((temp2 = fopen("temp2.dat", "w+b")) == NULL) { 
             fprintf(log, "Erro ao criar/reabrir arquivo temporario temp2.dat\n");
             free(bloco_memoria);
             fclose(temp1);
             exit(1);
        }

        tam_run *= 2;
    }

    rewind(temp1);
    rewind(arq);
    TJogo *final_j;
    while ((final_j = le_jogo(temp1)) != NULL) {
        salva_jogo(final_j, arq);
        free(final_j);
    }
    fflush(arq);


    if (_chsize(fileno(arq), (long)num_jogos * tamanho_registro_jogo()) != 0) {
        fprintf(log, "Aviso: Nao foi possivel truncar o arquivo para o tamanho correto.\n");
    }

    
    free(bloco_memoria);
    if (temp1) fclose(temp1);
    if (temp2) fclose(temp2);
    
    remove("temp1.dat");
    remove("temp2.dat");
    remove("temp_out.dat"); 


    fimT = clock();
    total = (double)(fimT - inicioT)/CLOCKS_PER_SEC;
    fprintf(log, "\nTempo Merge Sort Externo Jogo: %f ", total);
    fprintf(log, "\nBase de dados de jogos ordenada com sucesso!\n");
}