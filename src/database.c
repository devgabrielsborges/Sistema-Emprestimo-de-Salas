//
// Created by boreges on 14/11/24.
//

#include <stdio.h>
#include <sqlite3.h>
#include "database.h"


void criar_tabela_salas(sqlite3 *db, int rc) {
    const char *tabela_salas = "CREATE TABLE IF NOT EXISTS salas ("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "nome TEXT UNIQUE NOT NULL,"
                              "bloco TEXT NOT NULL);";


    rc = sqlite3_exec(db, tabela_salas, 0, 0, 0);


    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    } else {
        fprintf(stdout, "Tabela criada com sucesso\n");
    }
}

void criar_tabela_registros(sqlite3 *db, int rc) {
    const char *tabela_registros = "CREATE TABLE IF NOT EXISTS registros("
                                   "data TEXT NOT NULL,"
                                   "horario_inicio TEXT NOT NULL,"
                                   "horario_fim TEXT NOT NULL,"
                                   "sala_id INTEGER,"
                                   "FOREIGN KEY (sala_id) REFERENCES salas(id));";

    rc = sqlite3_exec(db, tabela_registros, 0, 0, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    }
}


void inserir_dados_sala(sqlite3 *db, int rc, const char *nome_sala, const char *bloco) {
    char sql_inserir_salas[100];


    sprintf(
            sql_inserir_salas,
            "INSERT INTO salas (nome, bloco) VALUES ('%s', '%s');",
            nome_sala, bloco
            );


    rc = sqlite3_exec(db, sql_inserir_salas, 0, 0, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    } else {
        fprintf(stdout, "Dados inseridos com sucesso\n");
    }
}

void atualizar_dados_sala(sqlite3 *db, int rc, const char *nome_sala, const char *novo_nome_sala, const char *novo_bloco) {
    char sql_atualizar_salas[100];

    sprintf(
            sql_atualizar_salas,
            "UPDATE salas SET (nome, bloco) = ('%s', '%s') WHERE nome = %s;",
            novo_nome_sala, novo_bloco, nome_sala
            );

    rc = sqlite3_exec(db, sql_atualizar_salas, 0, 0, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    }
}

void deletar_sala(sqlite3 *db, int rc, const char *nome_sala) {
    char sql_deletar_sala[100];

    sprintf(
            sql_deletar_sala,
            "DELETE FROM salas WHERE nome = '%s';",
            nome_sala
            );

    rc = sqlite3_exec(db, sql_deletar_sala, 0, 0, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    }
}

void inserir_registros(sqlite3 *db, int rc, const char *data, const char *horario_inicio, const char *horario_fim, char* nome_sala) {
    char sql_inserir_registros[100];


    sprintf(
            sql_inserir_registros,
            "INSERT INTO registros (data, horario_inicio, horario_fim, sala_id) VALUES ('%s', '%s', '%s', '%s');",
            data, horario_inicio, horario_fim, nome_sala
            );

    rc = sqlite3_exec(db, sql_inserir_registros, 0, 0, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    }
}

void atualizar_registro(sqlite3 *db, int rc, const char *data, const char *horario_inicio, const char *horario_fim, char* nome_sala) {
    char sql_atualizar_registros[100];

    sprintf(
            sql_atualizar_registros,
            "UPDATE registros SET (data, horario_inicio, horario_fim) = ('%s', '%s', '%s') WHERE sala_id = '%s';",
            data, horario_inicio, horario_fim, nome_sala
            );

    rc = sqlite3_exec(db, sql_atualizar_registros, 0, 0, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    }
}


void deletar_registro(sqlite3 *db, int rc, const char *data, const char *horario_inicio, const char *horario_fim, char* nome_sala) {
    char sql_deletar_registro[100];

    sprintf(
            sql_deletar_registro,
            "DELETE FROM registros WHERE data = '%s' AND horario_inicio = '%s' AND horario_fim = '%s' AND sala_id = '%s';",
            data, horario_inicio, horario_fim, nome_sala
            );

    rc = sqlite3_exec(db, sql_deletar_registro, 0, 0, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    }
}

void visualizar_dados_salas(sqlite3 *db, int rc) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM salas";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char *nome = sqlite3_column_text(stmt, 1);
        const unsigned char *bloco = sqlite3_column_text(stmt, 2);

        fprintf(stdout,"ID: %d\n", id);
        fprintf(stdout,"Nome: %s\n", nome);
        fprintf(stdout,"Bloco: %s\n", bloco);
    }

    sqlite3_finalize(stmt);
}

void visualizar_registros(sqlite3 *db, int rc) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM registros";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char *data = sqlite3_column_text(stmt, 0);
        const unsigned char *horario_inicio = sqlite3_column_text(stmt, 1);
        const unsigned char *horario_fim = sqlite3_column_text(stmt, 2);
        int sala_id = sqlite3_column_int(stmt, 3);

        printf("Data: %s\n", data);
        printf("Horário de início: %s\n", horario_inicio);
        printf("Horário de fim: %s\n", horario_fim);
        printf("ID da sala: %d\n", sala_id);
    }

    sqlite3_finalize(stmt);
}

void deletar_tudo(sqlite3 *db, int rc) {
    const char *sql_apagar_tudo = "DELETE FROM salas; DELETE FROM registros;"
                "UPDATE SQLITE_SEQUENCE SET seq = 0 WHERE name = 'salas';"
                "UPDATE SQLITE_SEQUENCE SET seq = 0 WHERE name = 'registros';";

    rc = sqlite3_exec(db, sql_apagar_tudo, 0, 0, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    }
}

