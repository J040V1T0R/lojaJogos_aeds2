#ifndef VENDA_H_INCLUDED
#define VENDA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "jogo.h" // Necessário pois a função de compra lida com jogos
#include "cliente.h" // Necessário pois a função de compra lida com clientes

#define MAX_ITENS 10

typedef struct ItemVenda {
    int cod_jogo;
    int quantidade;
    double preco_unitario;
} TItemVenda;

typedef struct Venda {
    int id_venda;
    int id_cliente;
    char data[11];
    double valor_total;
    TItemVenda itens[MAX_ITENS];
    int num_itens;
} TVenda;

TVenda *venda(int id_venda, int id_cliente, const char *data, double valor_total);
void imprime_venda(TVenda *venda);
void adiciona_item_venda(TVenda *venda, int cod_jogo, int quantidade, double preco_unitario);
void registra_nova_venda(TVenda *venda, FILE *out);
int tamanho_arquivo_venda(FILE *arq);

// Protótipo da função movida para cá
void realizar_compra(int id_cliente, int cod_jogo_desejado, int quantidade_comprada,
                      FILE *arq_jogos, FILE *arq_clientes, FILE *arq_vendas, FILE *log);

#endif // VENDA_H_INCLUDED