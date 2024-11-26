#ifndef CSV_OPERATIONS_H
#define CSV_OPERATIONS_H

#include <stdbool.h>

bool verificar_registro(const char *filename, const char *sala, const char *data, const char *horario_inicio, const char *horario_fim);

int agendar_horario_sala(const char *filename, const char *sala, const char *data, const char *horario_inicio, const char *horario_fim);

void apagar_reserva(const char *filename, const char *sala, const char *data, const char *horario_inicio, const char *horario_fim);

void exibir_reservas(const char *filename);

int cadastrar_usuario(const char *filename, const char login[20], const char senha[40]);

int autenticar_usuario(const char *filename, const char *login, const char *senha);

void pegar_data_atual(char *data);

#endif // CSV_OPERATIONS_H