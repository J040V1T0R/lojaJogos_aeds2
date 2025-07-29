#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> // Para strcmp

#include "jogo.h"
#include "buscaSequencialJogo.h"
#include "buscaBinariaJogo.h"
#include "mergeSortExternoJogo.h"
#include "cliente.h"
#include "funcionario.h"
#include "venda.h" // AGORA CONTÉM O PROTÓTIPO DE realizar_compra


// A função realizar_compra foi movida para venda.c e seu protótipo está em venda.h
// Não é mais necessária aqui no main.c


int main()
{
    FILE *arq_jogos;
    FILE *arq_clientes;
    FILE *arq_funcionarios;
    FILE *arq_vendas;
    FILE *log;

    int opcao;
    char resposta[5];
    TJogo *j;
    TCliente *c;
    TFuncionario *f;
    int id_cliente_compra, cod_jogo_compra, qtd_compra;
    int cod_busca;
    TJogo *jogo_encontrado;


    //abertura e criação de arquivos de dados

    if ((log = fopen("log_jogos.txt", "a+")) == NULL) {
            printf("Erro ao abrir arquivo de log\n");
            exit(1);
        }

    if ((arq_jogos = fopen("jogos.dat", "w+b")) == NULL) {
        printf("Erro ao abrir arquivo de jogos\n");
        exit(1);
    }
    if ((arq_clientes = fopen("clientes.dat", "w+b")) == NULL) {
        printf("Erro ao abrir arquivo de clientes\n");
        fclose(arq_jogos); exit(1);
    }
    if ((arq_funcionarios = fopen("funcionarios.dat", "w+b")) == NULL) {
        printf("Erro ao abrir arquivo de funcionarios\n");
        fclose(arq_jogos); fclose(arq_clientes); exit(1);
    }
    if ((arq_vendas = fopen("vendas.dat", "w+b")) == NULL) {
        printf("Erro ao abrir arquivo de vendas\n");
        fclose(arq_jogos); fclose(arq_clientes); fclose(arq_funcionarios); exit(1);
    }


    printf("\n--- Gerando a base inicial de 10 jogos desordenados (automático) ---\n");
    criarBaseJogos(arq_jogos, 10);
    fflush(arq_jogos);
    printf("Base inicial de jogos desordenados criada com sucesso.\n");


    do {
        printf("\n============================================\n");
        printf("           MENU DE GERENCIAMENTO DE JOGOS   \n");
        printf("============================================\n");
        printf("1. Recriar Base de Jogos (10 jogos desordenados)\n");
        printf("2. Imprimir Base de Jogos (Desordenada)\n");
        printf("3. Imprimir Base de Jogos (Ordenada)\n");
        printf("4. Ordenar Base de Jogos (Escolher Metodo)\n");
        printf("5. Adicionar Novo Jogo (Manual)\n");
        printf("6. Cadastrar Novo Cliente (Manual)\n");
        printf("7. Cadastrar Novo Funcionario (Manual)\n");
        printf("8. Buscar Jogo (Sequencial ou Binaria)\n");
        printf("9. Realizar Compra\n");
        printf("0. Sair\n");
        printf("============================================\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                printf("\n--- Recriando a base de dados de 10 jogos desordenados ---\n");
                criarBaseJogos(arq_jogos, 10);
                fflush(arq_jogos);
                printf("Base de jogos recriada com sucesso.\n");
                break;

            case 2:
                printf("\n--- Imprimindo a base de dados de jogos (Desordenada) ---\n");
                imprimirBaseJogos(arq_jogos);
                break;

            case 3:
                printf("\n--- Imprimindo a base de dados de jogos (Ordenada) ---\n");
                imprimirBaseJogos(arq_jogos);
                break;

            case 4:
                printf("\n--- Escolha o Metodo de Geracao de Runs ---\n");
                printf("1. Metodo Antigo (qsort em blocos)\n");
                printf("2. Selecao por Substituicao\n");
                printf("Escolha uma opcao para ordenar: ");
                int sub_opcao_ordenacao;
                scanf("%d", &sub_opcao_ordenacao);
                getchar();

                RunGenerationMethod metodo_escolhido;
                if (sub_opcao_ordenacao == 1) {
                    metodo_escolhido = RUN_GENERATION_OLD_WAY;
                } else if (sub_opcao_ordenacao == 2) {
                    metodo_escolhido = RUN_GENERATION_REPLACEMENT_SELECTION;
                } else {
                    printf("Opcao invalida. Ordenacao cancelada.\n");
                    break;
                }

                printf("\nDeseja ordenar a base de jogos com o metodo escolhido? (s/n): ");
                fgets(resposta, sizeof(resposta), stdin);
                resposta[strcspn(resposta, "\n")] = 0;
                if (strcmp(resposta, "s") == 0 || strcmp(resposta, "S") == 0) {
                    printf("\n--- Ordenando a base de dados de jogos com Merge Sort Externo ---\n");
                    // Passa o método escolhido para a função mergeSortExternoJogo
                    mergeSortExternoJogo(arq_jogos, log, metodo_escolhido);
                    fflush(arq_jogos);
                    printf("Base de dados de jogos ordenada com sucesso.\n");
                } else {
                    printf("Ordenacao cancelada.\n");
                }
                break;

            case 5:
                do {
                    printf("\nDeseja adicionar um novo jogo manualmente? (s/n): ");
                    fgets(resposta, sizeof(resposta), stdin);
                    resposta[strcspn(resposta, "\n")] = 0;
                    if (strcmp(resposta, "s") == 0 || strcmp(resposta, "S") == 0) {
                        j = cria_jogo_manual();
                        if (j) {
                            adiciona_jogo_ao_estoque(j, arq_jogos);
                            free(j);
                            printf("Novo jogo adicionado. Deseja adicionar outro? (s/n): ");
                            fgets(resposta, sizeof(resposta), stdin);
                            resposta[strcspn(resposta, "\n")] = 0;
                        } else {
                            printf("Erro ao criar jogo (memoria ou input).\n");
                            break;
                        }
                    } else {
                        printf("Adicao de jogos finalizada.\n");
                        break;
                    }
                } while (strcmp(resposta, "s") == 0 || strcmp(resposta, "S") == 0);
                break;

            case 6:
                do {
                    printf("\nDeseja cadastrar um novo cliente manualmente? (s/n): ");
                    fgets(resposta, sizeof(resposta), stdin);
                    resposta[strcspn(resposta, "\n")] = 0;
                    if (strcmp(resposta, "s") == 0 || strcmp(resposta, "S") == 0) {
                        c = cria_cliente_manual();
                        if (c) {
                            cadastra_novo_cliente(c, arq_clientes);
                            free(c);
                            printf("Novo cliente cadastrado. Deseja cadastrar outro? (s/n): ");
                            fgets(resposta, sizeof(resposta), stdin);
                            resposta[strcspn(resposta, "\n")] = 0;
                        } else {
                            printf("Erro ao criar cliente (memoria ou input).\n");
                            break;
                        }
                    } else {
                        printf("Cadastro de clientes finalizado.\n");
                        break;
                    }
                } while (strcmp(resposta, "s") == 0 || strcmp(resposta, "S") == 0);
                break;

            case 7:
                do {
                    printf("\nDeseja cadastrar um novo funcionario manualmente? (s/n): ");
                    fgets(resposta, sizeof(resposta), stdin);
                    resposta[strcspn(resposta, "\n")] = 0;
                    if (strcmp(resposta, "s") == 0 || strcmp(resposta, "S") == 0) {
                        f = cria_funcionario_manual();
                        if (f) {
                            cadastra_novo_funcionario(f, arq_funcionarios);
                            free(f);
                            printf("Novo funcionario cadastrado. Deseja cadastrar outro? (s/n): ");
                            fgets(resposta, sizeof(resposta), stdin);
                            resposta[strcspn(resposta, "\n")] = 0;
                        } else {
                            printf("Erro ao criar funcionario (memoria ou input).\n");
                            break;
                        }
                    } else {
                        printf("Cadastro de funcionarios finalizado.\n");
                        break;
                    }
                } while (strcmp(resposta, "s") == 0 || strcmp(resposta, "S") == 0);
                break;

            case 8:
                printf("\n--- Buscar Jogo ---\n");
                printf("Qual o codigo do jogo que deseja buscar? ");
                scanf("%d", &cod_busca);
                getchar();

                printf("Deseja usar busca binaria (b) ou sequencial (s)? ");
                fgets(resposta, sizeof(resposta), stdin);
                resposta[strcspn(resposta, "\n")] = 0;

                if (strcmp(resposta, "b") == 0 || strcmp(resposta, "B") == 0) {
                    printf("Realizando busca binaria para o jogo %d...\n", cod_busca);
                    jogo_encontrado = busca_binaria_jogo(cod_busca, arq_jogos, 0, tamanho_arquivo_jogo(arq_jogos) - 1, log);
                } else if (strcmp(resposta, "s") == 0 || strcmp(resposta, "S") == 0) {
                    printf("Realizando busca sequencial para o jogo %d...\n", cod_busca);
                    jogo_encontrado = buscaSequencialJogo(cod_busca, arq_jogos, log);
                } else {
                    printf("Opcao de busca invalida. Retornando ao menu.\n");
                    jogo_encontrado = NULL;
                }

                if (jogo_encontrado) {
                    imprime_jogo(jogo_encontrado);
                    free(jogo_encontrado);
                } else {
                    printf("Jogo %d nao encontrado na base de dados.\n", cod_busca);
                }
                break;

            case 9:
                printf("\n--- Realizar Compra ---\n");
                printf("ID do Cliente para a compra: ");
                scanf("%d", &id_cliente_compra);
                getchar();

                printf("Codigo do Jogo para a compra: ");
                scanf("%d", &cod_jogo_compra);
                getchar();

                printf("Quantidade a comprar: ");
                scanf("%d", &qtd_compra);
                getchar();

                realizar_compra(id_cliente_compra, cod_jogo_compra, qtd_compra, arq_jogos, arq_clientes, arq_vendas, log);
                break;

            case 0:
                printf("\nSaindo do programa. Ate mais!\n");
                break;

            default:
                printf("\nOpcao invalida. Por favor, escolha uma opcao valida.\n");
                break;
        }

    } while (opcao != 0);


    fclose(arq_jogos);
    fclose(arq_clientes);
    fclose(arq_funcionarios);
    fclose(arq_vendas);
    fclose(log);

    return 0;
}