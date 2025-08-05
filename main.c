#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "jogo.h"
#include "cliente.h"
#include "funcionario.h"
#include "venda.h"

#include "buscaSequencialJogo.h"
#include "buscaBinariaJogo.h"
#include "mergeSortExternoJogo.h"
#include "SelecaoSubs.h"
#include "arvoreVencedores.h"

#include "testes.h"


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


    // abre o log no modo "w" para limpar o conteúdo a cada nova execução dos testes
    if ((log = fopen("log_jogos.txt", "w")) == NULL) {
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


    printf("\n--- Gerando a base inicial de 10 jogos desordenados (automatico) ---\n");
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
        printf("4. Ordenar Base de Jogos\n");
        printf("5. Adicionar Novo Jogo (Manual)\n");
        printf("6. Cadastrar Novo Cliente (Manual)\n");
        printf("7. Cadastrar Novo Funcionario (Manual)\n");
        printf("8. Buscar Jogo (Sequencial ou Binaria)\n");
        printf("9. Realizar Compra\n");
        printf("10. Rodar Testes de Desempenho (Questao 3)\n");
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
                printf("\n--- Metodos de Ordenacao Externa ---\n");
                printf("1. Merge Sort Externo (Intercalacao Basica)\n");
                printf("2. Selecao por Substituicao + Arvore de Vencedores\n");
                printf("Escolha o metodo de ordenacao: ");
                int escolha_ord;
                scanf("%d", &escolha_ord);
                getchar();

                if (escolha_ord == 1) {
                    printf("\n--- Ordenando com Merge Sort Externo ---\n");
                    mergeSortExternoJogo(arq_jogos, log);
                    fflush(arq_jogos);
                } else if (escolha_ord == 2) {
                    printf("\n--- Ordenando com Selecao por Substituicao e Arvore de Vencedores ---\n");
                    clock_t inicio_total = clock();

                    printf("Gerando particoes com Selecao por Substituicao...\n");
                    int num_particoes = selecao_por_substituicao(arq_jogos, "particao_sel", 5);
                    printf("%d particoes criadas.\n", num_particoes);

                    printf("Intercalando particoes com Arvore de Vencedores...\n");

                    fclose(arq_jogos);
                    arq_jogos = fopen("jogos.dat", "wb");
                    if (!arq_jogos) {
                        perror("Nao foi possivel reabrir o arquivo de jogos para a saida.");
                        exit(1);
                    }

                    arvore_de_vencedores("particao_sel", num_particoes, arq_jogos);

                    clock_t fim_total = clock();
                    double tempo_total = (double)(fim_total - inicio_total) / CLOCKS_PER_SEC;

                    printf("Base de dados ordenada com sucesso em %.4f segundos.\n", tempo_total);
                    fprintf(log, "Tempo total (Selecao por Substituicao + Arvore de Vencedores): %f segundos\n", tempo_total);

                    fclose(arq_jogos);
                    if ((arq_jogos = fopen("jogos.dat", "r+b")) == NULL) {
                        printf("Erro ao reabrir arquivo de jogos para uso continuo.\n");
                        exit(1);
                    }

                } else {
                    printf("Opcao invalida.\n");
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

            case 10:
                rodar_testes_de_desempenho(&arq_jogos, log);
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
