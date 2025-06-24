#include "buscaBinariaJogo.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <time.h> // Adicionado para clock_t

//Realiza uma busca binaria por um jogo na base de dados
TJogo *busca_binaria_jogo(int chave, FILE *in, int inicio, int fim, FILE *log) {

    TJogo *j = NULL; // Inicializa j como NULL
    int cod = -1;    // Inicializa cod com um valor que não será a chave
    int cont = 0;    // Contador de comparações
    clock_t inicioT, fimT;
    double total;

    inicioT = clock(); // Marca o tempo de início da busca

    // Loop da busca binária: continua enquanto o início for menor ou igual ao fim e a chave não for encontrada
    while (inicio <= fim && cod != chave) {
        int meio = (inicio + fim) / 2; // Calcula o índice do meio (usando divisão inteira)

        // Posiciona o cursor do arquivo no registro do meio
        fseek(in, (long int)meio * tamanho_registro_jogo(), SEEK_SET);
        j = le_jogo(in); // Lê o jogo na posição do meio

        if (j) { // Verifica se a leitura foi bem sucedida antes de acessar cod
            cod = j->cod; // Obtém o código do jogo lido
        } else {
            // Se não conseguir ler o jogo, algo está errado com o arquivo ou offset.
            // Para evitar loop infinito, sai do loop.
            break;
        }

        cont ++; // Incrementa o contador de comparações

        if (cod > chave) {
            fim = meio - 1; // A chave está na primeira metade
        } else if (cod < chave) {
            inicio = meio + 1; // A chave está na segunda metade
        }
        // Se cod == chave, o loop será encerrado na próxima iteração pela condição 'cod != chave'

        // Libera a memória do jogo lido em cada iteração, pois um novo jogo será lido na próxima.
        // Se o jogo for encontrado, ele será lido novamente no final para ser retornado.
        if (j && cod != chave) { // Só libera se não for o jogo procurado
            free(j);
            j = NULL; // Evita ponteiro dangling
        }
    }

    fimT = clock(); // Marca o tempo de fim da busca
    total = (double)(fimT - inicioT)/CLOCKS_PER_SEC; // Calcula o tempo total em segundos

    if (cod == chave) { // Se o jogo foi encontrado
        // Precisamos ler novamente o jogo correto, pois o 'j' pode ter sido liberado
        // ou pode estar apontando para o último 'j' antes de sair do loop,
        // mas a posição do cursor pode ter se movido.
        // A posição correta para o jogo encontrado é no `meio` da última iteração
        // onde `cod == chave`. No entanto, como o loop termina quando `cod == chave`,
        // a última posição válida do meio é aquela onde o jogo foi encontrado.
        // O `inicio` ou `fim` após o loop também podem apontar para essa posição.
        // Para simplificar e garantir, podemos reposicionar o cursor no meio correto.
        int meio_final = (inicio + fim) / 2; // Calcula o meio final (onde a chave foi encontrada)
        fseek(in, (long int)meio_final * tamanho_registro_jogo(), SEEK_SET);
        TJogo *found_j = le_jogo(in); // Lê o jogo final para retorno

        fprintf(log, "\nComparacoes Binaria Jogo: %d ", cont);
        fprintf(log, "\nTempo Binaria Jogo: %f ", total);
        return found_j; // Retorna o jogo encontrado
    } else { // Se o jogo não foi encontrado
        fprintf(log, "\nComparacoes Binaria Jogo: %d ", cont);
        fprintf(log, "\nTempo Binaria Jogo: %f ", total);
        if (j) free(j); // Garante que o último 'j' lido seja liberado se não for o procurado
        return NULL;
    }
}