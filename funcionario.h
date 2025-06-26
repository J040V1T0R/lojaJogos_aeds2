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
    char dataContratacao[11]; // Formato "DD/MM/AAAA"
} TFuncionario;

// Retorna o tamanho em bytes de um registro de funcionario
int tamanho_registro_funcionario();

// Cria um novo funcionario
TFuncionario *funcionario(int id, char *nome, char *cargo, double salario, char *dataContratacao);

// Salva um funcionario no arquivo
void salva_funcionario(TFuncionario *f, FILE *out);

// Lê um funcionario do arquivo
TFuncionario *le_funcionario(FILE *in);

// Imprime os detalhes de um funcionario
void imprime_funcionario(TFuncionario *f);

// Adiciona um novo funcionario ao arquivo
void cadastra_novo_funcionario(TFuncionario *f, FILE *arq_funcionarios);

#endif // FUNCIONARIO_H_INCLUDED