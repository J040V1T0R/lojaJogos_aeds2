#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "jogo.h"
#include "buscaSequencialJogo.h"
#include "buscaBinariaJogo.h"
#include "mergeSortExternoJogo.h"
#include "cliente.h" // NOVO
#include "funcionario.h" // NOVO
#include "venda.h" // NOVO

// Funcao auxiliar para simular uma compra
void realizar_compra(int id_cliente, int cod_jogo_desejado, int quantidade_comprada,
                      FILE *arq_jogos, FILE *arq_clientes, FILE *arq_vendas, FILE *log) {

    printf("\n--- Realizando compra (Cliente %d, Jogo %d, Qtd %d) ---\n",
           id_cliente, cod_jogo_desejado, quantidade_comprada);

    // 1. Consultar a disponibilidade do jogo no estoque (precisa da base ordenada para busca binária eficiente)
    // Assumimos que 'arq_jogos' ja esta ordenado para usar busca binaria.
    // Ou usar busca sequencial se a ordenacao nao for garantida.
    TJogo *jogo_no_estoque = busca_binaria_jogo(cod_jogo_desejado, arq_jogos, 0, tamanho_arquivo_jogo(arq_jogos) - 1, log);

    if (jogo_no_estoque == NULL) {
        printf("ERRO: Jogo %d nao encontrado no estoque.\n", cod_jogo_desejado);
        return;
    }

    if (jogo_no_estoque->quantidadeEmEstoque < quantidade_comprada) {
        printf("ERRO: Estoque insuficiente para o Jogo %d. Disponivel: %d, Solicitado: %d.\n",
               cod_jogo_desejado, jogo_no_estoque->quantidadeEmEstoque, quantidade_comprada);
        free(jogo_no_estoque);
        return;
    }

    printf("Jogo '%s' disponivel. Estoque atual: %d. Preco: %.2f.\n",
           jogo_no_estoque->titulo, jogo_no_estoque->quantidadeEmEstoque, jogo_no_estoque->preco);

    // 2. Atualizar a quantidade em estoque do(s) jogo(s) comprado(s).
    int nova_quantidade_estoque = jogo_no_estoque->quantidadeEmEstoque - quantidade_comprada;
    atualiza_quantidade_estoque_jogo(cod_jogo_desejado, nova_quantidade_estoque, arq_jogos);

    // Re-lê o jogo para confirmar a atualização do estoque na memória, se quiser imprimir logo apos
    // TJogo *jogo_atualizado = busca_binaria_jogo(cod_jogo_desejado, arq_jogos, 0, tamanho_arquivo_jogo(arq_jogos) - 1, log);
    // if(jogo_atualizado) { imprime_jogo(jogo_atualizado); free(jogo_atualizado); }


    // 3. Criar um registro de venda associando o cliente e os jogos.
    // Para simplificar, vamos gerar um ID de venda simples e uma data atual.
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char data_venda[11];
    sprintf(data_venda, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    // Pega o proximo ID de venda
    // Isso seria melhor gerenciado em um arquivo de metadados ou com um contador global persistente.
    // Por enquanto, apenas um ID fixo ou um contador simples se o arquivo estiver vazio
    int proximo_id_venda = 1;
    // Aqui voce poderia ler o ultimo ID de venda do arquivo de vendas e incrementar
    // proximo_id_venda = tamanho_arquivo_venda(arq_vendas) + 1; // se tiver uma função tamanho_arquivo_venda

    TVenda *nova_venda = venda(proximo_id_venda, id_cliente, data_venda, 0.0); // Valor total sera calculado na adicao de itens
    adiciona_item_venda(nova_venda, cod_jogo_desejado, quantidade_comprada, jogo_no_estoque->preco);

    registra_nova_venda(nova_venda, arq_vendas);

    imprime_venda(nova_venda);

    free(nova_venda);
    free(jogo_no_estoque); // Libera o jogo que foi lido para consulta
    printf("Compra realizada com sucesso!\n");
}


int main()
{
    FILE *arq_jogos;
    FILE *arq_clientes; // NOVO
    FILE *arq_funcionarios; // NOVO
    FILE *arq_vendas; // NOVO
    FILE *log;

    TJogo *j;
    TCliente *c; // NOVO
    TFuncionario *f; // NOVO
    TVenda *v; // NOVO

    // --- Abertura e Criação de Arquivos de Dados ---

    if ((log = fopen("log_jogos.txt", "a+")) == NULL) {
            printf("Erro ao abrir arquivo de log\n");
            exit(1);
        }

    if ((arq_jogos = fopen("jogos.dat", "w+b")) == NULL) {
        printf("Erro ao abrir arquivo de jogos\n");
        exit(1);
    }
    if ((arq_clientes = fopen("clientes.dat", "w+b")) == NULL) { // NOVO
        printf("Erro ao abrir arquivo de clientes\n");
        fclose(arq_jogos); exit(1);
    }
    if ((arq_funcionarios = fopen("funcionarios.dat", "w+b")) == NULL) { // NOVO
        printf("Erro ao abrir arquivo de funcionarios\n");
        fclose(arq_jogos); fclose(arq_clientes); exit(1);
    }
    if ((arq_vendas = fopen("vendas.dat", "w+b")) == NULL) { // NOVO
        printf("Erro ao abrir arquivo de vendas\n");
        fclose(arq_jogos); fclose(arq_clientes); fclose(arq_funcionarios); exit(1);
    }
    else { // Bloco else para o caso de todos os arquivos abrirem com sucesso

        // --- Demonstração de Gerenciamento de Jogos ---
        printf("\n--- Criando a base de dados de 10 jogos desordenados (com estoque) ---\n");
        criarBaseJogos(arq_jogos, 10); // Criando uma base de 10 jogos (agora com estoque)
        fflush(arq_jogos);

        printf("\n--- Imprimindo a base de dados desordenada ---\n");
        imprimirBaseJogos(arq_jogos);

        printf("\n--- Ordenando a base de dados de jogos ---\n");
        mergeSortExternoJogo(arq_jogos, log);
        fflush(arq_jogos);

        printf("\n--- Imprimindo a base de dados ordenada ---\n");
        imprimirBaseJogos(arq_jogos);

        // Adicionando um novo jogo ao estoque (Gerenciamento de Jogos)
        printf("\n--- Adicionando um novo jogo ao estoque ---\n");
        j = jogo(99, "Novo Jogo Incrivel", "Acao", "Multi", 79.99, 50);
        adiciona_jogo_ao_estoque(j, arq_jogos);
        free(j); // Libera o jogo recem-adicionado
        imprimirBaseJogos(arq_jogos); // Imprime para ver o novo jogo no final

        // --- Demonstração de Gerenciamento de Clientes ---
        printf("\n--- Cadastrando um novo cliente ---\n");
        c = cliente(1, "Maria Silva", "111.222.333-44", "98765-4321");
        cadastra_novo_cliente(c, arq_clientes);
        free(c);

        printf("\n--- Cadastrando outro cliente ---\n");
        c = cliente(2, "Joao Souza", "555.666.777-88", "12345-6789");
        cadastra_novo_cliente(c, arq_clientes);
        free(c);


        // --- Demonstração de Gerenciamento de Funcionários (Exemplo Simples) ---
        printf("\n--- Cadastrando um novo funcionario ---\n");
        f = funcionario(101, "Carlos Admin", "Gerente", 5000.00, "01/01/2023");
        cadastra_novo_funcionario(f, arq_funcionarios);
        free(f);

        // --- Demonstração da Interação: Realizar uma Compra ---
        // A busca binária para 'realizar_compra' exige que jogos.dat esteja ordenado.
        // Já ordenamos acima.

        realizar_compra(1, 3, 2, arq_jogos, arq_clientes, arq_vendas, log); // Cliente 1 compra 2 unidades do Jogo 3
        realizar_compra(2, 5, 1, arq_jogos, arq_clientes, arq_vendas, log); // Cliente 2 compra 1 unidade do Jogo 5
        realizar_compra(1, 3, 100, arq_jogos, arq_clientes, arq_vendas, log); // Cliente 1 tenta comprar muito do Jogo 3 (deve falhar)
        realizar_compra(1, 999, 1, arq_jogos, arq_clientes, arq_vendas, log); // Cliente 1 tenta comprar jogo inexistente

        // Você pode imprimir o arquivo de vendas completo se quiser ver todos os registros
        // printf("\n--- Imprimindo todas as vendas registradas ---\n");
        // rewind(arq_vendas);
        // while ((v = le_venda(arq_vendas)) != NULL) {
        //     imprime_venda(v);
        //     free(v);
        // }
    }

    // --- Fechamento de Arquivos ---
    fclose(arq_jogos);
    fclose(arq_clientes);
    fclose(arq_funcionarios);
    fclose(arq_vendas);
    fclose(log);

    return 0;
}