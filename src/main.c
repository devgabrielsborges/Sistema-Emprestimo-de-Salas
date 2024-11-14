#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include "database.h"

void limpaTela() {
#ifdef _WIN32
    system("cls");  // Comando para Windows
#else
    system("clear");  // Comando para (Linux/Mac)
#endif
}

void menu(sqlite3 *db, int rc) {
    int opcao;
    char nome_sala[50], bloco[50], novo_nome_sala[50], novo_bloco[50];
    char data[50], horario_inicio[50], horario_fim[50];

    while (1) {
        limpaTela();
        printf("\nMenu:\n");
        printf("1. Inserir dados na tabela salas\n");
        printf("2. Atualizar dados na tabela salas\n");
        printf("3. Deletar dados na tabela salas\n");
        printf("4. Inserir registros\n");
        printf("5. Atualizar registros\n");
        printf("6. Deletar registros\n");
        printf("7. Visualizar dados das salas\n");
        printf("8. Visualizar registros\n");
        printf("9. Deletar tudo\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Nome da sala: ");
                scanf("%s", nome_sala);
                printf("Bloco: ");
                scanf("%s", bloco);
                inserir_dados_sala(db, rc, nome_sala, bloco);
                break;
            case 2:
                printf("Nome da sala a ser atualizada: ");
                scanf("%s", nome_sala);
                printf("Novo nome da sala: ");
                scanf("%s", novo_nome_sala);
                printf("Novo bloco: ");
                scanf("%s", novo_bloco);
                atualizar_dados_sala(db, rc, nome_sala, novo_nome_sala, novo_bloco);
                break;
            case 3:
                printf("Nome da sala a ser deletada: ");
                scanf("%s", nome_sala);
                deletar_sala(db, rc, nome_sala);
                break;
            case 4:
                printf("Data: ");
                scanf("%s", data);
                printf("Horario de inicio: ");
                scanf("%s", horario_inicio);
                printf("Horario de fim: ");
                scanf("%s", horario_fim);
                printf("Nome da sala: ");
                scanf("%s", nome_sala);
                inserir_registros(db, rc, data, horario_inicio, horario_fim, nome_sala);
                break;
            case 5:
                printf("Data: ");
                scanf("%s", data);
                printf("Horario de inicio: ");
                scanf("%s", horario_inicio);
                printf("Horario de fim: ");
                scanf("%s", horario_fim);
                printf("Nome da sala: ");
                scanf("%s", nome_sala);
                atualizar_registro(db, rc, data, horario_inicio, horario_fim, nome_sala);
                break;
            case 6:
                printf("Data: ");
                scanf("%s", data);
                printf("Horario de inicio: ");
                scanf("%s", horario_inicio);
                printf("Horario de fim: ");
                scanf("%s", horario_fim);
                printf("Nome da sala: ");
                scanf("%s", nome_sala);
                deletar_registro(db, rc, data, horario_inicio, horario_fim, nome_sala);
                break;
            case 7:
                visualizar_dados_salas(db, rc);
                break;
            case 8:
                visualizar_registros(db, rc);
                break;
            case 9:
                deletar_tudo(db, rc);
                break;
            case 0:
                sqlite3_close(db);
                exit(0);
            default:
                printf("Opcao invalida!\n");
        }
    }
}

int main() {
    sqlite3 *db;
    int rc = sqlite3_open("registros_salas.db", &db);

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    fprintf(stdout, "Opened database successfully\n");

    criar_tabela_salas(db, rc);
    criar_tabela_registros(db, rc);

    menu(db, rc);

    return 0;
}