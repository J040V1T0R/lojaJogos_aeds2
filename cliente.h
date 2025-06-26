#ifndef CLIENTE_H_INCLUDED
#define CLIENTE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Cliente {
    int id;
    char nome[100];
    char cpf[15]; // Ex: "123.456.789-00"
    char telefone[20];
} TCliente;

// Retorna o tamanho em bytes de um registro de cliente
int tamanho_registro_cliente();

// Cria um novo cliente
TCliente *cliente(int id, char *nome, char *cpf, char *telefone);

// Salva um cliente no arquivo
void salva_cliente(TCliente *c, FILE *out);

// Lê um cliente do arquivo
TCliente *le_cliente(FILE *in);

// Imprime os detalhes de um cliente
void imprime_cliente(TCliente *c);

// Adiciona um novo cliente ao arquivo
void cadastra_novo_cliente(TCliente *c, FILE *arq_clientes);

// Retorna a quantidade de registros no arquivo de clientes
int tamanho_arquivo_cliente(FILE *arq);

#endif // CLIENTE_H_INCLUDED