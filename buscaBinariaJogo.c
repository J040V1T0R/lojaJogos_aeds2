#include "buscaBinariaJogo.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <time.h> 

//realiza uma busca binaria por um jogo na base de dados
TJogo *busca_binaria_jogo(int chave, FILE *in, int inicio, int fim, FILE *log) {

    TJogo *j = NULL;
    int cod = -1;
    int cont = 0;
    clock_t inicioT, fimT;
    double total;

    inicioT = clock();

    //loop da b. binaria: continua enquanto o início for menor ou igual ao fim e a chave não for encontrada
    while (inicio <= fim && cod != chave) {
        int meio = (inicio + fim) / 2;

        fseek(in, (long int)meio * tamanho_registro_jogo(), SEEK_SET);
        j = le_jogo(in);

        if (j) {
            cod = j->cod;
        } else {
            break;
        }

        cont ++;

        if (cod > chave) {
            fim = meio - 1;
        } else if (cod < chave) {
            inicio = meio + 1;
        }

        if (j && cod != chave) {
            free(j);
            j = NULL;
        }
    }

    fimT = clock();
    total = (double)(fimT - inicioT)/CLOCKS_PER_SEC;

    //registra as informações no arquivo de log
    fprintf(log, "\nComparacoes Binaria Jogo: %d ", cont);
    fprintf(log, "\nTempo Binaria Jogo: %f ", total);

    printf("Busca Binaria: Comparacoes = %d, Tempo = %f segundos.\n", cont, total);

    if (cod == chave) {
        int meio_final = (inicio + fim) / 2;
        fseek(in, (long int)meio_final * tamanho_registro_jogo(), SEEK_SET);
        TJogo *found_j = le_jogo(in);

        return found_j;
    } else {
        printf("Jogo nao encontrado na busca binaria.\n"); 
        if (j) free(j);
        return NULL;
    }
}
