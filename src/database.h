//
// Created by boreges on 14/11/24.
//

#ifndef DATABASE_H
#define DATABASE_H


void criar_tabela_salas(sqlite3 *db, int rc);
void criar_tabela_registros(sqlite3 *db, int rc);
void inserir_dados_sala(sqlite3 *db, int rc, const char *nome_sala, const char *bloco);
void atualizar_dados_sala(sqlite3 *db, int rc, const char *nome_sala, const char *novo_nome_sala, const char *novo_bloco);
void deletar_sala(sqlite3 *db, int rc, const char *nome_sala);
void inserir_registros(sqlite3 *db, int rc, const char *data, const char *horario_inicio, const char *horario_fim, char* nome_sala);
void atualizar_registro(sqlite3 *db, int rc, const char *data, const char *horario_inicio, const char *horario_fim, char* nome_sala);
void deletar_registro(sqlite3 *db, int rc, const char *data, const char *horario_inicio, const char *horario_fim, char* nome_sala);
void visualizar_registros(sqlite3 *db, int rc);
void visualizar_dados_salas(sqlite3 *db, int rc);
void deletar_tudo(sqlite3 *db, int rc);


#endif //DATABASE_H
