#ifndef VENDA_H_INCLUDED
#define VENDA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jogo.h" // Para referenciar TJogo, se necessário para detalhes do item
#include "cliente.h" // Para referenciar TCliente

// Para simplificar, definiremos um número máximo de itens por venda.
// Em um sistema real, isso seria uma lista dinâmica ou um arquivo separado.
#define MAX_ITENS_VENDA 5

typedef struct ItemVenda {
    int cod_jogo;
    int quantidade;
    double preco_unitario; // Preço do jogo no momento da venda
} TItemVenda;

typedef struct Venda {
    int id;
    int id_cliente;
    char dataVenda[11]; 
    TItemVenda itens[MAX_ITENS_VENDA]; 
    int num_itens; 
    double valorTotal;
} TVenda;

int tamanho_registro_venda();

TVenda *venda(int id, int id_cliente, char *dataVenda, double valorTotal);

void adiciona_item_venda(TVenda *v, int cod_jogo, int quantidade, double preco_unitario);

void salva_venda(TVenda *v, FILE *out);


TVenda *le_venda(FILE *in);

void imprime_venda(TVenda *v);

void registra_nova_venda(TVenda *v, FILE *arq_vendas);

int tamanho_arquivo_venda(FILE *arq);

#endif 