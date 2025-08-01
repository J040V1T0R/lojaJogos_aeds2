#include "arvoreVencedores.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

typedef struct {
    TJogo *jogo;
    int id_particao;
} No;

// Função auxiliar para "jogar" a partida na árvore e definir o vencedor no nó pai
// 'i' é o índice do nó filho da DIREITA no par que está competindo
void competir(No *arvore, int i) {
    int pai = i / 2;
    int irmao = i - 1; // Irmão da esquerda
    if (pai == 0) return;

    // Compara os dois filhos para definir o pai
    if (arvore[i].jogo == NULL && arvore[irmao].jogo == NULL) {
        arvore[pai].jogo = NULL; // Ambos os filhos estão vazios
    } else if (arvore[i].jogo == NULL) {
        arvore[pai] = arvore[irmao]; // Apenas o da direita está vazio, irmão da esquerda vence
    } else if (arvore[irmao].jogo == NULL) {
        arvore[pai] = arvore[i]; // Apenas o da esquerda está vazio, irmão da direita vence
    } else {
        // Ambos têm jogos, o menor vence
        if (arvore[irmao].jogo->cod < arvore[i].jogo->cod) {
            arvore[pai] = arvore[irmao];
        } else {
            arvore[pai] = arvore[i];
        }
    }
}

void arvore_de_vencedores(const char* nome_base_particoes, int num_particoes, FILE* arq_saida) {
    rewind(arq_saida);

    FILE *particoes[num_particoes];
    for (int i = 0; i < num_particoes; ++i) {
        char nome_particao[100];
        sprintf(nome_particao, "%s%d.dat", nome_base_particoes, i + 1);
        particoes[i] = fopen(nome_particao, "rb");
        if (!particoes[i]) {
            perror("Erro ao abrir arquivo de particao");
            return;
        }
    }

    // A árvore precisa de 2 * num_particoes nós no total
    No *arvore = (No*) malloc(2 * num_particoes * sizeof(No));

    // CORREÇÃO: Inicializa toda a árvore para um estado seguro (NULL)
    for(int i = 0; i < 2 * num_particoes; i++){
        arvore[i].jogo = NULL;
        arvore[i].id_particao = -1;
    }

    // Carrega as folhas da árvore com o primeiro elemento de cada partição
    for (int i = 0; i < num_particoes; ++i) {
        arvore[num_particoes + i].jogo = le_jogo(particoes[i]);
        arvore[num_particoes + i].id_particao = i;
    }

    // Constrói a árvore de vencedores inicial, de baixo para cima
    for (int i = 2 * num_particoes - 1; i > 1; i -= 2) {
        competir(arvore, i);
    }

    // Processo de intercalação
    while (arvore[1].jogo != NULL) {
        // 1. Salva o vencedor (que está na raiz, índice 1) no arquivo de saída
        salva_jogo(arvore[1].jogo, arq_saida);

        // 2. Pega o ID da partição de onde o vencedor veio
        int particao_vencedora = arvore[1].id_particao;
        free(arvore[1].jogo);

        // 3. Lê o próximo registro daquela partição para substituir o que foi salvo
        TJogo *proximo_jogo = le_jogo(particoes[particao_vencedora]);

        // 4. Coloca o novo registro na folha correspondente da árvore
        int folha_idx = num_particoes + particao_vencedora;
        arvore[folha_idx].jogo = proximo_jogo;

        // 5. CORREÇÃO: Sobe na árvore a partir da folha atualizada, refazendo as competições
        while (folha_idx > 1) {
            // Garante que 'i' seja sempre o filho da direita para a função competir
            int i = (folha_idx % 2 == 0) ? folha_idx + 1 : folha_idx;
            competir(arvore, i);
            folha_idx /= 2; // Move para o pai para a próxima iteração
        }
    }

    // Libera recursos
    free(arvore);
    for (int i = 0; i < num_particoes; ++i) {
        fclose(particoes[i]);
        char nome_particao[100];
        sprintf(nome_particao, "%s%d.dat", nome_base_particoes, i + 1);
        remove(nome_particao);
    }
}