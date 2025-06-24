#include "buscaSequencialJogo.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h> // Necessário para clock_t e CLOCKS_PER_SEC

//Realiza uma busca sequencial por um jogo na base de dados
TJogo *buscaSequencialJogo(int chave, FILE *in, FILE *log){

    TJogo *j;
    int achou = 0;
    int cont = 0; // Contador de comparações
    clock_t inicio, fim;
    double total;

    rewind(in); // Garante que a leitura começa do início do arquivo

    inicio = clock(); // Marca o tempo de início da busca

    // Loop que lê cada jogo do arquivo até o final ou até encontrar a chave
    while ((j = le_jogo(in)) != NULL){
        cont ++; // Incrementa o contador de comparações

        if(j->cod == chave){ // Se o código do jogo atual é igual à chave
           achou = 1; // Marca que o jogo foi encontrado
           break;     // Sai do loop
        }
        free(j); // Libera a memória do jogo lido se não for o procurado
    }

    fim = clock(); // Marca o tempo de fim da busca
    total = (double)(fim - inicio)/CLOCKS_PER_SEC; // Calcula o tempo total em segundos

    // Registra as informações no arquivo de log
    fprintf(log, "\nComparacoes Sequencial Jogo: %d ", cont);
    fprintf(log, "\nTempo Sequencial Jogo: %f ", total);

    if(achou == 1){
        return j; // Retorna o ponteiro para o jogo encontrado
    } else {
        printf("Jogo nao encontrado na busca sequencial.\n");
        return NULL; // Retorna NULL se o jogo não foi encontrado
    }
}