#include "venda.h"
#include "jogo.h"
#include "buscaBinariaJogo.h" 
#include <string.h>
#include <stdlib.h>
#include <time.h> 

TVenda *venda(int id_venda, int id_cliente, const char *data, double valor_total) {
    TVenda *v = (TVenda *) malloc(sizeof(TVenda));
    if (v) {
        v->id_venda = id_venda;
        v->id_cliente = id_cliente;
        strcpy(v->data, data);
        v->valor_total = valor_total;
        v->num_itens = 0;
    }
    return v;
}

void imprime_venda(TVenda *venda) {
    printf("\n--- Detalhes da Venda ---\n");
    printf("ID Venda: %d\n", venda->id_venda);
    printf("ID Cliente: %d\n", venda->id_cliente);
    printf("Data: %s\n", venda->data);
    printf("Itens:\n");
    for (int i = 0; i < venda->num_itens; ++i) {
        printf("  - Jogo (Cod: %d), Qtd: %d, Preco Unit.: %.2f\n",
               venda->itens[i].cod_jogo, venda->itens[i].quantidade, venda->itens[i].preco_unitario);
    }
    printf("Valor Total: R$ %.2f\n", venda->valor_total);
}

void adiciona_item_venda(TVenda *venda, int cod_jogo, int quantidade, double preco_unitario) {
    if (venda->num_itens < MAX_ITENS) {
        venda->itens[venda->num_itens].cod_jogo = cod_jogo;
        venda->itens[venda->num_itens].quantidade = quantidade;
        venda->itens[venda->num_itens].preco_unitario = preco_unitario;
        venda->num_itens++;
        venda->valor_total += quantidade * preco_unitario;
    }
}

void registra_nova_venda(TVenda *venda, FILE *out) {
    fseek(out, 0, SEEK_END);
    fwrite(venda, sizeof(TVenda), 1, out);
    fflush(out);
}

int tamanho_arquivo_venda(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    return ftell(arq) / sizeof(TVenda);
}

void realizar_compra(int id_cliente, int cod_jogo_desejado, int quantidade_comprada,
                      FILE *arq_jogos, FILE *arq_clientes, FILE *arq_vendas, FILE *log) {

    printf("\n--- Realizando compra (Cliente %d, Jogo %d, Qtd %d) ---\n",
           id_cliente, cod_jogo_desejado, quantidade_comprada);


    TJogo *jogo_no_estoque = busca_binaria_jogo(cod_jogo_desejado, arq_jogos, 0, tamanho_arquivo_jogo(arq_jogos) - 1, log);

    if (jogo_no_estoque == NULL) {
        printf("ERRO: Jogo %d nao encontrado no estoque (ou base nao ordenada para busca binaria).\n", cod_jogo_desejado);
        return;
    }

    if (jogo_no_estoque->quantidadeEmEstoque < quantidade_comprada) {
        printf("ERRO: Estoque insuficiente para o Jogo '%s'. Disponivel: %d, Solicitado: %d.\n",
               jogo_no_estoque->titulo, jogo_no_estoque->quantidadeEmEstoque, quantidade_comprada);
        free(jogo_no_estoque);
        return;
    }

    printf("Jogo '%s' disponivel. Estoque atual: %d. Preco: %.2f.\n",
           jogo_no_estoque->titulo, jogo_no_estoque->quantidadeEmEstoque, jogo_no_estoque->preco);


    int nova_quantidade_estoque = jogo_no_estoque->quantidadeEmEstoque - quantidade_comprada;
    atualiza_quantidade_estoque_jogo(cod_jogo_desejado, nova_quantidade_estoque, arq_jogos);

    //cria um registro de venda associando o cliente e os jogos
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char data_venda[11];
    sprintf(data_venda, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    int proximo_id_venda = tamanho_arquivo_venda(arq_vendas) + 1;

    TVenda *nova_venda = venda(proximo_id_venda, id_cliente, data_venda, 0.0);
    adiciona_item_venda(nova_venda, cod_jogo_desejado, quantidade_comprada, jogo_no_estoque->preco);

    registra_nova_venda(nova_venda, arq_vendas);

    imprime_venda(nova_venda);

    free(nova_venda);
    free(jogo_no_estoque);
    printf("Compra realizada com sucesso!\n");
}
