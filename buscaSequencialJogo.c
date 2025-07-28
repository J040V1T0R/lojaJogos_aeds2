#include "buscaSequencialJogo.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h> 

//realiza uma busca sequencial por um jogo
TJogo *buscaSequencialJogo(int chave, FILE *in, FILE *log){

    TJogo *j;
    int achou = 0;
    int cont = 0; 
    clock_t inicio, fim;
    double total;

    rewind(in); 

    inicio = clock(); //marca o tempo de início da busca

    //loop que lê cada jogo do arquivo até o final ou até encontrar a chave
    while ((j = le_jogo(in)) != NULL){
        cont ++; //

        if(j->cod == chave){ 
           achou = 1; 
           break;     
        }
        free(j); 
    }

    fim = clock(); 
    total = (double)(fim - inicio)/CLOCKS_PER_SEC; 

    //registra as informações no arquivo de log
    fprintf(log, "\nComparacoes Sequencial Jogo: %d ", cont);
    fprintf(log, "\nTempo Sequencial Jogo: %f ", total);

    printf("Busca Sequencial: Comparacoes = %d, Tempo = %f segundos.\n", cont, total);

    if(achou == 1){
        return j; 
    } else {
        printf("Jogo nao encontrado na busca sequencial.\n");
        return NULL; 
    }
}