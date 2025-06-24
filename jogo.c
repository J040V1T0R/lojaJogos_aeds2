#include "jogo.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h> // Para trunc, se necessário, embora não esteja sendo usado diretamente agora.

// Retorna tamanho do jogo em bytes
int tamanho_registro_jogo() {
    return sizeof(int)  //cod
           + sizeof(char) * 100 //titulo
           + sizeof(char) * 50 //genero
           + sizeof(char) * 50 //plataforma
           + sizeof(double); //preco
}


// Cria jogo.
TJogo *jogo(int cod, char *titulo, char *genero, char *plataforma, double preco) {
    TJogo *j = (TJogo *) malloc(sizeof(TJogo));
    //inicializa espaco de memoria com ZEROS
    if (j) memset(j, 0, sizeof(TJogo));
    //copia valores para os campos de j
    j->cod = cod;
    strcpy(j->titulo, titulo);
    strcpy(j->genero, genero);
    strcpy(j->plataforma, plataforma);
    j->preco = preco;
    return j;
}


// Salva jogo no arquivo out, na posicao atual do cursor
void salva_jogo(TJogo *j, FILE *out) {
    fwrite(&j->cod, sizeof(int), 1, out);
    fwrite(j->titulo, sizeof(char), sizeof(j->titulo), out);
    fwrite(j->genero, sizeof(char), sizeof(j->genero), out);
    fwrite(j->plataforma, sizeof(char), sizeof(j->plataforma), out);
    fwrite(&j->preco, sizeof(double), 1, out);
}


// retorna a quantidade de registros no arquivo
int tamanho_arquivo_jogo(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    int tam = trunc((double)ftell(arq) / tamanho_registro_jogo()); // Explicit cast to double
    return tam;
}



// Le um jogo do arquivo in na posicao atual do cursor
// Retorna um ponteiro para jogo lido do arquivo
TJogo *le_jogo(FILE *in) {
    TJogo *j = (TJogo *) malloc(sizeof(TJogo));
    if (0 >= fread(&j->cod, sizeof(int), 1, in)) { // Se não conseguir ler o código, libera e retorna NULL
        free(j);
        return NULL;
    }
    fread(j->titulo, sizeof(char), sizeof(j->titulo), in);
    fread(j->genero, sizeof(char), sizeof(j->genero), in);
    fread(j->plataforma, sizeof(char), sizeof(j->plataforma), in);
    fread(&j->preco, sizeof(double), 1, in);
    return j;
}


// Imprime jogo
void imprime_jogo(TJogo *j) {
    printf("**********************************************");
    printf("\nJogo de codigo ");
    printf("%d", j->cod);
    printf("\nTitulo: ");
    printf("%s", j->titulo);
    printf("\nGenero: ");
    printf("%s", j->genero);
    printf("\nPlataforma: ");
    printf("%s", j->plataforma);
    printf("\nPreco: ");
    printf("%4.2f", j->preco);
    printf("\n**********************************************");
}


// Cria a base de dados de jogos
void criarBaseJogos(FILE *out, int tam){

    int vet[tam];
    TJogo *j;

    for(int i=0;i<tam;i++)
        vet[i] = i + 1;

    embaralha_jogos(vet,tam); // Descomentar para embaralhar e criar base desordenada

    printf("\nGerando a base de dados de jogos...\n");

    for (int i=0;i<tam;i++){
        // Exemplo de dados para jogos
        char titulo[100];
        char genero[50];
        char plataforma[50];
        sprintf(titulo, "Jogo %d", vet[i]);
        sprintf(genero, "Aventura");
        sprintf(plataforma, "PC");
        j = jogo(vet[i], titulo, genero, plataforma, 59.90 + (i * 0.1));
        salva_jogo(j, out);
        free(j); // Libera a memória após salvar cada jogo
    }

    // O free(j) no final do loop original estava liberando apenas o último ponteiro.
    // É importante liberar cada 'j' após seu uso dentro do loop.

}

//embaralha base de dados
void embaralha_jogos(int *vet, int tam) {

    int tmp;

    srand(time(NULL));

    int trocas = (tam * 60) / 100; // 60% dos elementos serão trocados para desordenar

    for (int t = 0; t < trocas; t++) { // O loop deve ir de 0 a trocas-1
        int i = rand() % tam;
        int j = rand() % tam;
        tmp = vet[i];
        vet[i] = vet[j];
        vet[j] = tmp;
    }
}


void imprimirBaseJogos(FILE *out){

printf("\nImprimindo a base de dados de jogos...\n");

    rewind(out);
    TJogo *j;

    while ((j = le_jogo(out)) != NULL) {
        imprime_jogo(j);
        free(j); // Libera a memória após imprimir cada jogo
    }

    // O free(j) no final do loop original estava liberando um ponteiro NULL
    // ou o último jogo que foi lido (e possivelmente já liberado).

}


int compara_jogos(TJogo *j1, TJogo *j2)
{
	if (j1 == NULL) {
		return (j2 == NULL);
	}
	if (j1->cod != j2->cod) {
		return 0;
	}
	if (strcmp(j1->titulo, j2->titulo) != 0) {
		return 0;
	}
	return 1;
}