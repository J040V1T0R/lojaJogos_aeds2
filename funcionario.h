#ifndef FUNCIONARIO_H_INCLUDED
#define FUNCIONARIO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Funcionario {
    int id;
    char nome[100];
    char cargo[50];
    double salario;
    char dataContratacao[11]; 
} TFuncionario;

int tamanho_registro_funcionario();

TFuncionario *funcionario(int id, char *nome, char *cargo, double salario, char *dataContratacao);

void salva_funcionario(TFuncionario *f, FILE *out);

TFuncionario *le_funcionario(FILE *in);

void imprime_funcionario(TFuncionario *f);

void cadastra_novo_funcionario(TFuncionario *f, FILE *arq_funcionarios);

TFuncionario *cria_funcionario_manual();

#endif