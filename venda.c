#include "venda.h"
#include <string.h>
#include <time.h> // Para time_t, struct tm, localtime
#include <stdio.h> // Para sprintf, printf, FILE
#include <stdlib.h> // Para free

#include "jogo.h" // Para busca_binaria_jogo, tamanho_arquivo_jogo, atualiza_quantidade_estoque_jogo, imprime_jogo
#include "cliente.h" // Embora nao use funcoes do cliente.c, cliente.h pode ser util para referencias futuras.


// Retorna o tamanho em bytes de um registro de venda
int tamanho_registro_venda() {
    return sizeof(int)      // id
           + sizeof(int)      // id_cliente
           + sizeof(char) * 11  // dataVenda
           + (sizeof(TItemVenda) * MAX_ITENS_VENDA) // itens (fixo)
           + sizeof(int)      // num_itens
           + sizeof(double);    // valorTotal
}

// Cria uma nova venda
TVenda *venda(int id, int id_cliente, char *dataVenda, double valorTotal) {
    TVenda *v = (TVenda *) malloc(sizeof(TVenda));
    if (v) memset(v, 0, sizeof(TVenda));
    v->id = id;
    v->id_cliente = id_cliente;
    strcpy(v->dataVenda, dataVenda);
    v->num_itens = 0; // Inicializa sem itens
    v->valorTotal = valorTotal;
    return v;
}

// Adiciona um item a uma venda
void adiciona_item_venda(TVenda *v, int cod_jogo, int quantidade, double preco_unitario) {
    if (v->num_itens < MAX_ITENS_VENDA) {
        v->itens[v->num_itens].cod_jogo = cod_jogo;
        v->itens[v->num_itens].quantidade = quantidade;
        v->itens[v->num_itens].preco_unitario = preco_unitario;
        v->num_itens++;
        v->valorTotal += (quantidade * preco_unitario);
    } else {
        printf("Limite de itens por venda atingido (%d).\n", MAX_ITENS_VENDA);
    }
}

// Salva uma venda no arquivo
void salva_venda(TVenda *v, FILE *out) {
    fwrite(&v->id, sizeof(int), 1, out);
    fwrite(&v->id_cliente, sizeof(int), 1, out);
    fwrite(v->dataVenda, sizeof(char), sizeof(v->dataVenda), out);
    fwrite(v->itens, sizeof(TItemVenda), MAX_ITENS_VENDA, out); // Salva o array completo
    fwrite(&v->num_itens, sizeof(int), 1, out);
    fwrite(&v->valorTotal, sizeof(double), 1, out);
}

// Lê uma venda do arquivo
TVenda *le_venda(FILE *in) {
    TVenda *v = (TVenda *) malloc(sizeof(TVenda));
    if (0 >= fread(&v->id, sizeof(int), 1, in)) {
        free(v);
        return NULL;
    }
    fread(&v->id_cliente, sizeof(int), 1, in);
    fread(v->dataVenda, sizeof(char), sizeof(v->dataVenda), in);
    fread(v->itens, sizeof(TItemVenda), MAX_ITENS_VENDA, in);
    fread(&v->num_itens, sizeof(int), 1, in);
    fread(&v->valorTotal, sizeof(double), 1, in);
    return v;
}

// Imprime os detalhes de uma venda
void imprime_venda(TVenda *v) {
    printf("--- Venda ---\n");
    printf("ID da Venda: %d\n", v->id);
    printf("ID do Cliente: %d\n", v->id_cliente);
    printf("Data: %s\n", v->dataVenda);
    printf("Itens Vendidos (%d):\n", v->num_itens);
    for (int i = 0; i < v->num_itens; i++) {
        printf("  - Jogo ID: %d, Qtd: %d, Preco Unitario: %.2f\n",
               v->itens[i].cod_jogo, v->itens[i].quantidade, v->itens[i].preco_unitario);
    }
    printf("Valor Total: %.2f\n", v->valorTotal);
    printf("-------------\n");
}

// Registra uma nova venda no arquivo
void registra_nova_venda(TVenda *v, FILE *arq_vendas) {
    fseek(arq_vendas, 0, SEEK_END);
    salva_venda(v, arq_vendas);
    fflush(arq_vendas);
    printf("Venda %d registrada com sucesso.\n", v->id);
}

// Retorna a quantidade de registros no arquivo de vendas
int tamanho_arquivo_venda(FILE *arq) {
    long current_pos = ftell(arq);
    fseek(arq, 0, SEEK_END);
    int tam = (int)(ftell(arq) / tamanho_registro_venda());
    fseek(arq, current_pos, SEEK_SET);
    return tam;
}

// A função realizar_compra, movida da main.c
void realizar_compra(int id_cliente, int cod_jogo_desejado, int quantidade_comprada,
                      FILE *arq_jogos, FILE *arq_clientes, FILE *arq_vendas, FILE *log) {

    printf("\n--- Realizando compra (Cliente %d, Jogo %d, Qtd %d) ---\n",
           id_cliente, cod_jogo_desejado, quantidade_comprada);

    // Necessita de busca_binaria_jogo, tamanho_arquivo_jogo, atualiza_quantidade_estoque_jogo, imprime_jogo
    // que são definidas em jogo.h e implementadas em jogo.c
    TJogo *jogo_no_estoque = busca_binaria_jogo(cod_jogo_desejado, arq_jogos, 0, tamanho_arquivo_jogo(arq_jogos) - 1, log);

    if (jogo_no_estoque == NULL) {
        printf("ERRO: Jogo %d nao encontrado no estoque (ou base nao ordenada para busca binaria).\n", cod_jogo_desejado);
        return;
    }

    if (jogo_no_estoque->quantidadeEmEstoque < quantidade_comprada) {
        printf("ERRO: Estoque insuficiente para o Jogo '%s'. Disponivel: %d, Solicitado: %d.\n",
               jogo_no_estoque->titulo, jogo_no_estoque->quantidadeEmEstoque, quantidade_comprada);
        free(jogo_no_estoque);
        return;
    }

    printf("Jogo '%s' disponivel. Estoque atual: %d. Preco: %.2f.\n",
           jogo_no_estoque->titulo, jogo_no_estoque->quantidadeEmEstoque, jogo_no_estoque->preco);

    int nova_quantidade_estoque = jogo_no_estoque->quantidadeEmEstoque - quantidade_comprada;
    atualiza_quantidade_estoque_jogo(cod_jogo_desejado, nova_quantidade_estoque, arq_jogos);

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char data_venda[11];
    sprintf(data_venda, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    int proximo_id_venda = tamanho_arquivo_venda(arq_vendas) + 1;

    TVenda *nova_venda = venda(proximo_id_venda, id_cliente, data_venda, 0.0);
    adiciona_item_venda(nova_venda, cod_jogo_desejado, quantidade_comprada, jogo_no_estoque->preco);

    registra_nova_venda(nova_venda, arq_vendas);

    imprime_venda(nova_venda);

    free(nova_venda);
    free(jogo_no_estoque);
    printf("Compra realizada com sucesso!\n");
}