#include "venda.h"
#include <string.h>

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