#include "cliente.h"
#include <string.h>

// Retorna o tamanho em bytes de um registro de cliente
int tamanho_registro_cliente() {
    return sizeof(int)      // id
           + sizeof(char) * 100 // nome
           + sizeof(char) * 15  // cpf
           + sizeof(char) * 20; // telefone
}

// Cria um novo cliente
TCliente *cliente(int id, char *nome, char *cpf, char *telefone) {
    TCliente *c = (TCliente *) malloc(sizeof(TCliente));
    if (c) memset(c, 0, sizeof(TCliente));
    c->id = id;
    strcpy(c->nome, nome);
    strcpy(c->cpf, cpf);
    strcpy(c->telefone, telefone);
    return c;
}

// Salva um cliente no arquivo
void salva_cliente(TCliente *c, FILE *out) {
    fwrite(&c->id, sizeof(int), 1, out);
    fwrite(c->nome, sizeof(char), sizeof(c->nome), out);
    fwrite(c->cpf, sizeof(char), sizeof(c->cpf), out);
    fwrite(c->telefone, sizeof(char), sizeof(c->telefone), out);
}

// Lê um cliente do arquivo
TCliente *le_cliente(FILE *in) {
    TCliente *c = (TCliente *) malloc(sizeof(TCliente));
    if (0 >= fread(&c->id, sizeof(int), 1, in)) {
        free(c);
        return NULL;
    }
    fread(c->nome, sizeof(char), sizeof(c->nome), in);
    fread(c->cpf, sizeof(char), sizeof(c->cpf), in);
    fread(c->telefone, sizeof(char), sizeof(c->telefone), in);
    return c;
}

// Imprime os detalhes de um cliente
void imprime_cliente(TCliente *c) {
    printf("--- Cliente ---\n");
    printf("ID: %d\n", c->id);
    printf("Nome: %s\n", c->nome);
    printf("CPF: %s\n", c->cpf);
    printf("Telefone: %s\n", c->telefone);
    printf("---------------\n");
}

// Adiciona um novo cliente ao arquivo (cadastrar novo cliente)
void cadastra_novo_cliente(TCliente *c, FILE *arq_clientes) {
    fseek(arq_clientes, 0, SEEK_END); // Posiciona no final do arquivo
    salva_cliente(c, arq_clientes);
    fflush(arq_clientes);
    printf("Cliente '%s' cadastrado com sucesso.\n", c->nome);
}

// Retorna a quantidade de registros no arquivo de clientes
int tamanho_arquivo_cliente(FILE *arq) {
    long current_pos = ftell(arq);
    fseek(arq, 0, SEEK_END);
    int tam = (int)(ftell(arq) / tamanho_registro_cliente());
    fseek(arq, current_pos, SEEK_SET);
    return tam;
}