#include "funcionario.h"
#include <string.h>

//retorna o tamanho em bytes de um registro de funcionario
int tamanho_registro_funcionario() {
    return sizeof(int)      
           + sizeof(char) * 100 
           + sizeof(char) * 50  
           + sizeof(double)   
           + sizeof(char) * 11; 
}

//cria um novo funcionario
TFuncionario *funcionario(int id, char *nome, char *cargo, double salario, char *dataContratacao) {
    TFuncionario *f = (TFuncionario *) malloc(sizeof(TFuncionario));
    if (f) memset(f, 0, sizeof(TFuncionario));
    f->id = id;
    strcpy(f->nome, nome);
    strcpy(f->cargo, cargo);
    f->salario = salario;
    strcpy(f->dataContratacao, dataContratacao);
    return f;
}

void salva_funcionario(TFuncionario *f, FILE *out) {
    fwrite(&f->id, sizeof(int), 1, out);
    fwrite(f->nome, sizeof(char), sizeof(f->nome), out);
    fwrite(f->cargo, sizeof(char), sizeof(f->cargo), out);
    fwrite(&f->salario, sizeof(double), 1, out);
    fwrite(f->dataContratacao, sizeof(char), sizeof(f->dataContratacao), out);
}

TFuncionario *le_funcionario(FILE *in) {
    TFuncionario *f = (TFuncionario *) malloc(sizeof(TFuncionario));
    if (0 >= fread(&f->id, sizeof(int), 1, in)) {
        free(f);
        return NULL;
    }
    fread(f->nome, sizeof(char), sizeof(f->nome), in);
    fread(f->cargo, sizeof(char), sizeof(f->cargo), in);
    fread(&f->salario, sizeof(double), 1, in);
    fread(f->dataContratacao, sizeof(char), sizeof(f->dataContratacao), in);
    return f;
}

void imprime_funcionario(TFuncionario *f) {
    printf("--- Funcionario ---\n");
    printf("ID: %d\n", f->id);
    printf("Nome: %s\n", f->nome);
    printf("Cargo: %s\n", f->cargo);
    printf("Salario: %.2f\n", f->salario);
    printf("Data Contratacao: %s\n", f->dataContratacao);
    printf("-------------------\n");
}

void cadastra_novo_funcionario(TFuncionario *f, FILE *arq_funcionarios) {
    fseek(arq_funcionarios, 0, SEEK_END);
    salva_funcionario(f, arq_funcionarios);
    fflush(arq_funcionarios);
    printf("Funcionario '%s' cadastrado com sucesso.\n", f->nome);
}

TFuncionario *cria_funcionario_manual() {
    int id;
    char nome[100];
    char cargo[50];
    double salario;
    char dataContratacao[11];

    printf("\n--- Cadastrar Novo Funcionario (Entrada Manual) ---\n");
    printf("ID do Funcionario: ");
    scanf("%d", &id);
    getchar(); 

    printf("Nome: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;

    printf("Cargo: ");
    fgets(cargo, sizeof(cargo), stdin);
    cargo[strcspn(cargo, "\n")] = 0;

    printf("Salario: ");
    scanf("%lf", &salario);
    getchar(); 

    printf("Data de Contratacao (DD/MM/AAAA): ");
    fgets(dataContratacao, sizeof(dataContratacao), stdin);
    dataContratacao[strcspn(dataContratacao, "\n")] = 0;

    return funcionario(id, nome, cargo, salario, dataContratacao);
}