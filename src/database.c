#include <stdio.h>
#include <sqlite3.h>

sqlite3 *db;
int rc;

void criar_tabela_salas() {
    const char *tabela_salas = "CREATE TABLE IF NOT EXISTS salas ("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "nome TEXT NOT NULL,"
                              "bloco TEXT NOT NULL);";

    rc = sqlite3_exec(db, tabela_salas, 0, 0, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    } else {
        fprintf(stdout, "Tabela criada com sucesso\n");
    }
}

void criar_tabela_registros() {
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

void inserir_dados_salas() {
    const char *sql = "INSERT INTO salas (nome, bloco) VALUES ('A02', 'Bloco A');"
                      "INSERT INTO salas (nome, bloco) VALUES ('B05', 'Bloco B');"
                      "INSERT INTO salas (nome, bloco) VALUES ('C04', 'Bloco C');";

    rc = sqlite3_exec(db, sql, 0, 0, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    } else {
        fprintf(stdout, "Dados inseridos com sucesso\n");
    }
}

void inserir_registros() {
    const char *sql = "INSERT INTO registros (data, horario_inicio, horario_fim, sala_id) VALUES ('2021-10-01', '08:00', '10:00', 1);"
                      "INSERT INTO registros (data, horario_inicio, horario_fim, sala_id) VALUES ('2021-10-01', '10:00', '12:00', 2);"
                      "INSERT INTO registros (data, horario_inicio, horario_fim, sala_id) VALUES ('2021-10-01', '14:00', '16:00', 3);";

    rc = sqlite3_exec(db, sql, 0, 0, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    } else {
        fprintf(stdout, "Dados inseridos com sucesso\n");
    }
}

void visualizar_dados_salas() {
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

        printf("ID: %d\n", id);
        printf("Nome: %s\n", nome);
        printf("Bloco: %s\n", bloco);
    }

    sqlite3_finalize(stmt);
}

void visualizar_registros() {
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

void deletar_tudo() {
    char *sql = "DELETE FROM salas; DELETE FROM registros;"
                "UPDATE SQLITE_SEQUENCE SET seq = 0 WHERE name = 'salas';"
                "UPDATE SQLITE_SEQUENCE SET seq = 0 WHERE name = 'registros';";

    rc = sqlite3_exec(db, sql, 0, 0, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    } else {
        fprintf(stdout, "Dados deletados e IDs redefinidos com sucesso\n");
    }
}

int main() {
    rc = sqlite3_open("registros_salas.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    fprintf(stdout, "Opened database successfully\n");

    deletar_tudo();

    criar_tabela_salas();
    criar_tabela_registros();

    inserir_dados_salas();
    inserir_registros();

    visualizar_dados_salas();
    visualizar_registros();

    sqlite3_close(db);
    return 0;
}
