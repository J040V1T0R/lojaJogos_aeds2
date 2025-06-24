#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "jogo.h" // Inclui a implementação do jogo
#include "buscaSequencialJogo.h" // Inclui a implementação da busca sequencial
#include "buscaBinariaJogo.h" // Inclui a implementação da busca binaria
#include "mergeSortExternoJogo.h" // Inclui a implementação do Merge Sort Externo

int main()
{
    FILE *arq;  // Arquivo principal de dados de jogos
    FILE *log;  // Arquivo de log

    TJogo *j;   // Ponteiro para um jogo

    // Abre o arquivo de log no modo de adição (append)
    if ((log = fopen("log_jogos.txt", "a+")) == NULL) {
            printf("Erro ao abrir arquivo de log\n");
            exit(1);
        }

    // Abre o arquivo de dados de jogos no modo de escrita/leitura binária
    if ((arq = fopen("jogos.dat", "w+b")) == NULL) {
        printf("Erro ao abrir arquivo de jogos\n");
        exit(1);
    }
    else{
        // 1. Criar a base de dados de 10 jogos desordenados
        printf("\n--- Criando a base de dados de 10 jogos desordenados ---\n");
        criarBaseJogos(arq, 10); // Alterado para 10 jogos
        fflush(arq); // Garante que os dados são escritos no disco

        // 2. Imprimir a base de dados desordenada
        printf("\n--- Imprimindo a base de dados desordenada ---\n");
        imprimirBaseJogos(arq);

        // 3. Ordenar a base de dados com Merge Sort Externo
        printf("\n--- Ordenando a base de dados com Merge Sort Externo ---\n");
        mergeSortExternoJogo(arq, log);
        fflush(arq); // Garante que a base ordenada está no disco

        // 4. Imprimir a base de dados ordenada
        printf("\n--- Imprimindo a base de dados ordenada ---\n");
        imprimirBaseJogos(arq);


        // As buscas sequencial e binária podem ser mantidas ou comentadas,
        // dependendo se você quer testá-las com a base de 10 jogos.
        // Para a busca binária funcionar, a base precisa estar ordenada, o que já fizemos.

        /*
        printf("\n--- Realizando busca sequencial (jogo 5) ---\n");
        j = buscaSequencialJogo(5, arq, log); // Buscando um jogo de código 5
        if (j) {
            imprime_jogo(j);
            free(j);
        } else {
            printf("Jogo nao encontrado na busca sequencial.\n");
        }


        printf("\n--- Realizando busca binaria (jogo 8) ---\n");
        j = busca_binaria_jogo(8, arq, 0, tamanho_arquivo_jogo(arq) - 1, log); // Buscando um jogo de código 8
        if (j) {
            imprime_jogo(j);
            free(j);
        } else {
            printf("Jogo nao encontrado na busca binaria ou base nao ordenada.\n");
        }
        */
    }

    // Fecha os arquivos
    fclose(arq);
    fclose(log);

    return 0;
}
