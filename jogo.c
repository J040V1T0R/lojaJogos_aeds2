#include "jogo.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

int tamanho_registro_jogo() {
    return sizeof(int)  
           + sizeof(char) * 100 
           + sizeof(char) * 50 
           + sizeof(char) * 50 
           + sizeof(double) 
           + sizeof(int); 
}


TJogo *jogo(int cod, char *titulo, char *genero, char *plataforma, double preco, int quantidadeEmEstoque) {
    TJogo *j = (TJogo *) malloc(sizeof(TJogo));
    if (j) memset(j, 0, sizeof(TJogo));
    j->cod = cod;
    strcpy(j->titulo, titulo);
    strcpy(j->genero, genero);
    strcpy(j->plataforma, plataforma);
    j->preco = preco;
    j->quantidadeEmEstoque = quantidadeEmEstoque;
    return j;
}

void salva_jogo(TJogo *j, FILE *out) {
    fwrite(&j->cod, sizeof(int), 1, out);
    fwrite(j->titulo, sizeof(char), sizeof(j->titulo), out);
    fwrite(j->genero, sizeof(char), sizeof(j->genero), out);
    fwrite(j->plataforma, sizeof(char), sizeof(j->plataforma), out);
    fwrite(&j->preco, sizeof(double), 1, out);
    fwrite(&j->quantidadeEmEstoque, sizeof(int), 1, out);
}

int tamanho_arquivo_jogo(FILE *arq) {
    long current_pos = ftell(arq);
    fseek(arq, 0, SEEK_END);
    int tam = trunc((double)ftell(arq) / tamanho_registro_jogo());
    fseek(arq, current_pos, SEEK_SET);
    return tam;
}

TJogo *le_jogo(FILE *in) {
    TJogo *j = (TJogo *) malloc(sizeof(TJogo));
    if (0 >= fread(&j->cod, sizeof(int), 1, in)) {
        free(j);
        return NULL;
    }
    fread(j->titulo, sizeof(char), sizeof(j->titulo), in);
    fread(j->genero, sizeof(char), sizeof(j->genero), in);
    fread(j->plataforma, sizeof(char), sizeof(j->plataforma), in);
    fread(&j->preco, sizeof(double), 1, in);
    fread(&j->quantidadeEmEstoque, sizeof(int), 1, in);
    return j;
}

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
    printf("\nEstoque: ");
    printf("%d", j->quantidadeEmEstoque);
    printf("\n**********************************************");
}


void criarBaseJogos(FILE *out, int tam){

    int vet[tam];
    TJogo *j;

    char *nomes_jogos[] = {
        "The Witcher 3: Wild Hunt",
        "Cyberpunk 2077",
        "Red Dead Redemption 2",
        "Grand Theft Auto V",
        "Minecraft",
        "The Legend of Zelda: Breath of the Wild",
        "Elden Ring",
        "God of War (2018)",
        "Stardew Valley",
        "Hollow Knight",
        "Among Us",
        "Valorant",
        "Apex Legends",
        "Fortnite",
        "Call of Duty: Warzone"
    };
    int num_nomes = sizeof(nomes_jogos) / sizeof(nomes_jogos[0]);

    char *generos[] = {
        "RPG", "Aventura", "Mundo Aberto", "Tiro", "Estratégia", "Simulação"
    };
    int num_generos = sizeof(generos) / sizeof(generos[0]);

    char *plataformas[] = {
        "PC", "PlayStation", "Xbox", "Nintendo Switch"
    };
    int num_plataformas = sizeof(plataformas) / sizeof(plataformas[0]);


    for(int i=0;i<tam;i++)
        vet[i] = i + 1;

    embaralha_jogos(vet,tam);

    printf("\nGerando a base de dados de jogos...\n");

    for (int i=0;i<tam;i++){
        char titulo[100];
        char genero[50];
        char plataforma[50];

        strcpy(titulo, nomes_jogos[i % num_nomes]);
        strcpy(genero, generos[i % num_generos]);
        strcpy(plataforma, plataformas[i % num_plataformas]);

        j = jogo(vet[i], titulo, genero, plataforma, 59.90 + (i * 0.1), 100);
        salva_jogo(j, out);
        free(j);
    }
}

void embaralha_jogos(int *vet, int tam) {
    int tmp;
    srand(time(NULL));
    int trocas = (tam * 60) / 100;
    for (int t = 0; t < trocas; t++) {
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
        free(j);
    }
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

void adiciona_jogo_ao_estoque(TJogo *j, FILE *out) {
    fseek(out, 0, SEEK_END);
    salva_jogo(j, out);
    fflush(out);
    printf("\nJogo '%s' adicionado ao estoque.\n", j->titulo);
}

void atualiza_quantidade_estoque_jogo(int cod_jogo, int nova_quantidade, FILE *arq_jogos) {
    TJogo *j;
    int pos_encontrada = -1;
    long current_pos = ftell(arq_jogos);

    rewind(arq_jogos);

    int i = 0;
    while ((j = le_jogo(arq_jogos)) != NULL) {
        if (j->cod == cod_jogo) {
            pos_encontrada = i;
            j->quantidadeEmEstoque = nova_quantidade;
            fseek(arq_jogos, (long)pos_encontrada * tamanho_registro_jogo(), SEEK_SET);
            salva_jogo(j, arq_jogos);
            fflush(arq_jogos);
            printf("\nEstoque do Jogo %d atualizado para %d.\n", cod_jogo, nova_quantidade);
            free(j);
            fseek(arq_jogos, current_pos, SEEK_SET);
            return;
        }
        free(j);
        i++;
    }
    printf("\nJogo de codigo %d nao encontrado para atualizar estoque.\n", cod_jogo);
    fseek(arq_jogos, current_pos, SEEK_SET);
}

TJogo *cria_jogo_manual() {
    int cod;
    char titulo[100];
    char genero[50];
    char plataforma[50];
    double preco;
    int quantidadeEmEstoque;

    printf("\n--- Criar Novo Jogo (Entrada Manual) ---\n");
    printf("Codigo: ");
    scanf("%d", &cod);
    getchar(); 

    printf("Titulo: ");
    fgets(titulo, sizeof(titulo), stdin);
    titulo[strcspn(titulo, "\n")] = 0; 

    printf("Genero: ");
    fgets(genero, sizeof(genero), stdin);
    genero[strcspn(genero, "\n")] = 0;

    printf("Plataforma: ");
    fgets(plataforma, sizeof(plataforma), stdin);
    plataforma[strcspn(plataforma, "\n")] = 0;

    printf("Preco: ");
    scanf("%lf", &preco);
    getchar(); 

    printf("Quantidade em Estoque: ");
    scanf("%d", &quantidadeEmEstoque);
    getchar(); 

    return jogo(cod, titulo, genero, plataforma, preco, quantidadeEmEstoque);
}