#ifndef CLIENTE_H_INCLUDED
#define CLIENTE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Cliente {
    int id;
    char nome[100];
    char cpf[15]; 
    char telefone[20];
} TCliente;

int tamanho_registro_cliente();

//cria um novo cliente
TCliente *cliente(int id, char *nome, char *cpf, char *telefone);

void salva_cliente(TCliente *c, FILE *out);

TCliente *le_cliente(FILE *in);

void imprime_cliente(TCliente *c);

void cadastra_novo_cliente(TCliente *c, FILE *arq_clientes);

int tamanho_arquivo_cliente(FILE *arq);

TCliente *cria_cliente_manual();

#endif 