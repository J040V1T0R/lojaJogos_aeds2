#ifndef VENDA_H_INCLUDED
#define VENDA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jogo.h" // Para TJogo (detalhes do item de venda)
#include "cliente.h" // Para TCliente (associacao com a venda)

// Para simplificar, definiremos um número máximo de itens por venda.
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

// Retorna a quantidade de registros no arquivo de vendas
int tamanho_arquivo_venda(FILE *arq);

// NOVO: Função para realizar uma compra, movida da main.c
// Recebe todos os FILE* necessários para interagir com as bases de dados.
void realizar_compra(int id_cliente, int cod_jogo_desejado, int quantidade_comprada,
                      FILE *arq_jogos, FILE *arq_clientes, FILE *arq_vendas, FILE *log);

#endif // VENDA_H_INCLUDED