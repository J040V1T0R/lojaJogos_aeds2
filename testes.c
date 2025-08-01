#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "testes.h"

// Implementação da função que automatiza os testes
void rodar_testes_de_desempenho(FILE **p_arq_jogos, FILE *log) {
    // Bases de dados a serem testadas, conforme solicitado
    int tamanhos[] = {10, 100, 1000, 5000};
    int num_tamanhos = sizeof(tamanhos) / sizeof(tamanhos[0]);

    printf("\nINICIANDO TESTES DE DESEMPENHO AUTOMATIZADOS...\n");
    fprintf(log, "--- INICIO DOS TESTES DE DESEMPENHO ---\n\n");

    for (int i = 0; i < num_tamanhos; i++) {
        int tam_base = tamanhos[i];
        printf("\n======================================================\n");
        printf("Testando com base de dados de %d registros...\n", tam_base);
        fprintf(log, "======================================================\n");
        fprintf(log, "BASE DE DADOS: %d REGISTROS\n", tam_base);

        // --- Teste 1: Merge Sort Externo (Método Antigo) ---
        printf("\n1. Testando Metodo Antigo (Merge Sort Basico)...\n");
        fprintf(log, "\n--- Metodo Antigo (Merge Sort Basico) ---\n");

        // Recria a base desordenada para o teste
        fclose(*p_arq_jogos);
        *p_arq_jogos = fopen("jogos.dat", "w+b");
        if (!*p_arq_jogos) exit(1);
        criarBaseJogos(*p_arq_jogos, tam_base);

        mergeSortExternoJogo(*p_arq_jogos, log);
        fflush(log);

        // --- Teste 2: Seleção por Substituição + Árvore de Vencedores (Método Novo) ---
        printf("\n2. Testando Metodo Novo (Selecao por Substituicao + Arvore de Vencedores)...\n");
        fprintf(log, "\n--- Metodo Novo (Selecao + Arvore) ---\n");

        // Recria a mesma base desordenada para um teste justo
        fclose(*p_arq_jogos);
        *p_arq_jogos = fopen("jogos.dat", "w+b");
        if (!*p_arq_jogos) exit(1);
        criarBaseJogos(*p_arq_jogos, tam_base);

        clock_t inicio_total = clock();

        // Usamos um valor de memória (M) maior para bases maiores para ser mais eficiente
        int M = (tam_base < 1000) ? 10 : 100;
        int num_particoes = selecao_por_substituicao(*p_arq_jogos, "particao_sel", M);
        printf("   - %d particoes criadas (M=%d).\n", num_particoes, M);

        fclose(*p_arq_jogos);
        *p_arq_jogos = fopen("jogos.dat", "wb");
        if (!*p_arq_jogos) exit(1);

        arvore_de_vencedores("particao_sel", num_particoes, *p_arq_jogos);

        clock_t fim_total = clock();
        double tempo_total = (double)(fim_total - inicio_total) / CLOCKS_PER_SEC;

        printf("   - Base ordenada em %.4f segundos.\n", tempo_total);
        fprintf(log, "Tempo total (Selecao por Substituicao + Arvore de Vencedores): %f segundos\n\n", tempo_total);
        fflush(log);
    }

    printf("\n======================================================\n");
    printf("TESTES FINALIZADOS! Resultados salvos em 'log_jogos.txt'.\n");
    fprintf(log, "--- FIM DOS TESTES DE DESEMPENHO ---\n");

    // Reabre o arquivo no modo r+b para o programa poder continuar funcionando
    fclose(*p_arq_jogos);
    *p_arq_jogos = fopen("jogos.dat", "r+b");
    if (!*p_arq_jogos) exit(1);
}