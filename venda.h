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
    char dataVenda[11]; // Formato "DD/MM/AAAA"
    TItemVenda itens[MAX_ITENS_VENDA]; // Lista de jogos vendidos
    int num_itens; // Quantidade real de itens nesta venda
    double valorTotal;
} TVenda;

// Retorna o tamanho em bytes de um registro de venda
int tamanho_registro_venda();

// Cria uma nova venda
TVenda *venda(int id, int id_cliente, char *dataVenda, double valorTotal);

// Adiciona um item a uma venda
void adiciona_item_venda(TVenda *v, int cod_jogo, int quantidade, double preco_unitario);

// Salva uma venda no arquivo
void salva_venda(TVenda *v, FILE *out);

// Lê uma venda do arquivo
TVenda *le_venda(FILE *in);

// Imprime os detalhes de uma venda
void imprime_venda(TVenda *v);

// Registra uma nova venda no arquivo
void registra_nova_venda(TVenda *v, FILE *arq_vendas);

#endif // VENDA_H_INCLUDED