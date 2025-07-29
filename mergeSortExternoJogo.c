#include "mergeSortExternoJogo.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <io.h>
#include <stdio.h>

#define TAM_BLOCO_REGISTROS 1000 // O TAM_BLOCO_REGISTROS também será o 'M' para a Seleção por Substituição

// ... (compara_jogos_qsort, le_bloco_jogos, salva_bloco_jogos) ...
// Mantenha estas funções como estão.

// Função intercala_arquivos (mantida como está)
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


// Função principal para Merge Sort Externo para jogos.
// Agora recebe um parâmetro para escolher o método de geração de runs.
void mergeSortExternoJogo(FILE *arq, FILE *log, RunGenerationMethod method_run_gen) {
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

    FILE *temp1 = NULL, *temp2 = NULL; // Arquivos temporários para runs


    if ((temp1 = fopen("temp1.dat", "w+b")) == NULL) {
        fprintf(log, "Erro ao criar arquivo temporario temp1.dat\n");
        exit(1);
    }
    if ((temp2 = fopen("temp2.dat", "w+b")) == NULL) {
        fprintf(log, "Erro ao criar arquivo temporario temp2.dat\n");
        fclose(temp1);
        exit(1);
    }

    int num_runs_geradas = 0;

    // =========================================================================
    // FASE 1: Geração de Runs - Escolha do Método
    // =========================================================================
    rewind(arq); // Garante que a leitura do arquivo de entrada começa do início

    if (method_run_gen == RUN_GENERATION_REPLACEMENT_SELECTION) {
        printf("\nUsando Selecao por Substituicao para gerar runs...\n");
        num_runs_geradas = gera_runs_selecao_substituicao(arq, temp1, temp2, TAM_BLOCO_REGISTROS);
        // Apos gera_runs_selecao_substituicao, temp1 e temp2 podem precisar ser reabertos
        // pois a função pode ter fechado ou deixado o cursor no fim.
        fclose(temp1);
        fclose(temp2);
        // Reabre para a fase de intercalação
        if ((temp1 = fopen("temp1.dat", "r+b")) == NULL) {
            fprintf(log, "Erro ao reabrir temp1.dat para intercalacao apos Selecao por Substituicao.\n");
            exit(1);
        }
        if ((temp2 = fopen("temp2.dat", "r+b")) == NULL) {
            fprintf(log, "Erro ao reabrir temp2.dat para intercalacao apos Selecao por Substituicao.\n");
            fclose(temp1);
            exit(1);
        }

    } else { // RUN_GENERATION_OLD_WAY (Método antigo)
        printf("\nUsando metodo antigo (qsort em blocos) para gerar runs...\n");
        TJogo *bloco_memoria_para_runs = (TJogo *)malloc(TAM_BLOCO_REGISTROS * sizeof(TJogo));
        if (bloco_memoria_para_runs == NULL) {
            fprintf(log, "Erro de alocacao de memoria para o bloco de runs (metodo antigo).\n");
            fclose(temp1);
            fclose(temp2);
            exit(1);
        }
        int i_old = 0;
        int current_num_runs_old = 0;
        while (i_old < num_jogos) {
            int lidos = le_bloco_jogos(arq, bloco_memoria_para_runs, TAM_BLOCO_REGISTROS);
            if (lidos == 0) break;
            qsort(bloco_memoria_para_runs, lidos, sizeof(TJogo), compara_jogos_qsort);
            if (current_num_runs_old % 2 == 0) {
                salva_bloco_jogos(temp1, bloco_memoria_para_runs, lidos);
            } else {
                salva_bloco_jogos(temp2, bloco_memoria_para_runs, lidos);
            }
            i_old += lidos;
            current_num_runs_old++;
        }
        free(bloco_memoria_para_runs);
        num_runs_geradas = current_num_runs_old;
    }

    // Se a geração de runs resultou em 0 ou 1 run, o arquivo já está ordenado ou é trivial.
    if (num_runs_geradas <= 1) {
        rewind(temp1); // A única run está em temp1.dat
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
        if (temp1) fclose(temp1);
        if (temp2) fclose(temp2); // Pode estar fechado ou não usado se a outra função gerou 1 run
        remove("temp1.dat");
        remove("temp2.dat");
        remove("temp_out.dat");
        fimT = clock();
        total = (double)(fimT - inicioT)/CLOCKS_PER_SEC;
        fprintf(log, "\nTempo Merge Sort Externo Jogo: %f ", total);
        fprintf(log, "\nBase de dados de jogos ordenada com sucesso (uma unica run gerada)!\n");
        return;
    }

    // =========================================================================
    // FASE 2: Intercalação das Runs (Multi-passagem, Intercalação Básica 2-vias)
    // =========================================================================
    // O tam_run aqui deve ser o tamanho da run gerada pela fase 1.
    // Se usou Selecao por Substituicao, runs sao em media 2*TAM_BLOCO_REGISTROS.
    // Se usou metodo antigo, tam_run = TAM_BLOCO_REGISTROS.
    int tam_run_intercalacao = TAM_BLOCO_REGISTROS; // Começa com o tamanho base da run

    // O loop de intercalação continua enquanto houver mais de uma run para mesclar
    int runs_restantes_na_passagem = num_runs_geradas;
    char nome_temp1_atual[20] = "temp1.dat"; // A entrada para a primeira passagem

    while (runs_restantes_na_passagem > 1) {
        // Define os nomes dos arquivos de entrada e saída para a rodada atual de intercalação.
        char nome_temp_entrada_1_passagem[20];
        char nome_temp_entrada_2_passagem[20];
        char nome_temp_saida_passagem[20];

        if (strcmp(nome_temp1_atual, "temp1.dat") == 0) {
            strcpy(nome_temp_entrada_1_passagem, "temp1.dat");
            strcpy(nome_temp_entrada_2_passagem, "temp2.dat");
            strcpy(nome_temp_saida_passagem, "temp_out.dat");
        } else {
            strcpy(nome_temp_entrada_1_passagem, "temp_out.dat");
            strcpy(nome_temp_entrada_2_passagem, "temp1.dat");
            strcpy(nome_temp_saida_passagem, "temp2.dat");
        }

        FILE *current_temp1_leitura = fopen(nome_temp_entrada_1_passagem, "r+b");
        FILE *current_temp2_leitura = fopen(nome_temp_entrada_2_passagem, "r+b");
        FILE *current_temp_saida_escrita = fopen(nome_temp_saida_passagem, "w+b");

        if (!current_temp1_leitura || !current_temp_saida_escrita) { // current_temp2_leitura pode ser NULL se impar
            fprintf(log, "Erro ao reabrir arquivos temporarios para intercalacao de passagem.\n");
            if (current_temp1_leitura) fclose(current_temp1_leitura);
            if (current_temp2_leitura) fclose(current_temp2_leitura);
            if (current_temp_saida_escrita) fclose(current_temp_saida_escrita);
            exit(1);
        }
        // Se current_temp2_leitura é NULL (quando num_runs_geradas é ímpar na fase inicial)
        // Ou se o arquivo existe mas está vazio (no meio das passagens)
        // A lógica de intercala_arquivos deve ser robusta para lidar com um arquivo de entrada vazio.

        // Realiza a intercalação dos dois arquivos de entrada para o arquivo de saída
        // O tam_run passado aqui é o tamanho da run que o intercala_arquivos espera
        // para AGREGAR em cada intercalação.
        intercala_arquivos(current_temp1_leitura, current_temp2_leitura, current_temp_saida_escrita, tam_run_intercalacao);


        fclose(current_temp1_leitura);
        if (current_temp2_leitura) fclose(current_temp2_leitura); // Fecha apenas se aberto
        fclose(current_temp_saida_escrita);

        strcpy(nome_temp1_atual, nome_temp_saida_passagem); // A saída desta passagem é a entrada principal da próxima

        // Limpa os arquivos temporários usados como entrada para a próxima rodada de intercalação
        remove("temp1.dat");
        remove("temp2.dat");
        // E renomeia a saída para temp1.dat
        rename(nome_temp_saida_passagem, "temp1.dat");

        // Reabre temp1 e temp2 para a próxima iteração (temp1 para leitura, temp2 para escrita para proxima distribuição)
        if ((temp1 = fopen("temp1.dat", "r+b")) == NULL) { // temp1 é sempre o arquivo que contém as runs mescladas
             fprintf(log, "Erro ao reabrir temp1.dat para proxima passagem.\n");
             exit(1);
        }
        if ((temp2 = fopen("temp2.dat", "w+b")) == NULL) { // temp2 é o arquivo "vazio" para próxima distribuição/saida
             fprintf(log, "Erro ao criar/reabrir temp2.dat para proxima passagem.\n");
             fclose(temp1);
             exit(1);
        }

        tam_run_intercalacao *= 2; // O tamanho das runs mescladas dobra a cada passagem
        runs_restantes_na_passagem = (runs_restantes_na_passagem + 1) / 2; // Aprox. metade das runs a cada passagem
    }

    // --- Fase Final: Copiar o Resultado para o Arquivo Original ---
    // O arquivo final ordenado estará em temp1.dat (o último arquivo de saída renomeado).
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

    // --- Limpeza de Recursos ---
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