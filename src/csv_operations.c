#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "csv_operations.h"

bool verificar_registro(const char *filename, const char *sala, const char *data, const char *horario_inicio, const char *horario_fim) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return false;
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), file)) {
        char arquivo_sala[50], arquivo_data[50], arquivo_horario_inicio[50], arquivo_horario_fim[50];
        
        // Use sscanf para separar os campos na linha
        if (sscanf(linha, "%49[^,],%49[^,],%49[^,],%49[^,\n]", arquivo_sala, arquivo_data, arquivo_horario_inicio, arquivo_horario_fim) == 4) {
            // Verifica se todos os campos correspondem
            if (strcmp(sala, arquivo_sala) == 0 && strcmp(data, arquivo_data) == 0 &&
                strcmp(horario_inicio, arquivo_horario_inicio) == 0 && strcmp(horario_fim, arquivo_horario_fim) == 0) {
                fclose(file);
                return true; // Registro encontrado
            }
        }
    }

    fclose(file);
    return false; // Registro não encontrado
}

int agendar_horario_sala(const char *filename, const char *sala, const char *data, const char *horario_inicio, const char *horario_fim) {
    if (verificar_registro(filename, sala, data, horario_inicio, horario_fim)) {
        printf("Registro já existe.\n");
        return 1;
    }

    FILE *file = fopen(filename, "a");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return 2;
    }

    fprintf(file, "%s,%s,%s,%s\n", sala, data, horario_inicio, horario_fim);
    fclose(file);

    printf("Registro adicionado com sucesso.\n");
    return 0;
}
void apagar_reserva(const char *filename, const char *sala, const char *data, const char *horario_inicio, const char *horario_fim) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    FILE *temp_file = fopen("temp.csv", "w");
    if (!temp_file) {
        perror("Erro ao criar o arquivo temporário");
        fclose(file);
        return;
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), file)) {
        char arquivo_sala[50], arquivo_data[50], arquivo_horario_inicio[50], arquivo_horario_fim[50];
        
        // Use sscanf para separar os campos na linha
        if (sscanf(linha, "%49[^,],%49[^,],%49[^,],%49[^,\n]", arquivo_sala, arquivo_data, arquivo_horario_inicio, arquivo_horario_fim) == 4) {
            // Verifica se todos os campos correspondem
            if (strcmp(sala, arquivo_sala) == 0 && strcmp(data, arquivo_data) == 0 &&
                strcmp(horario_inicio, arquivo_horario_inicio) == 0 && strcmp(horario_fim, arquivo_horario_fim) == 0) {
                continue; // Ignora a linha
            }
        }

        fprintf(temp_file, "%s", linha);
    }

    fclose(file);
    fclose(temp_file);

    if (remove(filename) != 0) {
        perror("Erro ao remover o arquivo original");
        return;
    }

    if (rename("temp.csv", filename) != 0) {
        perror("Erro ao renomear o arquivo temporário");
        return;
    }

    printf("Registro removido com sucesso.\n");
}

void exibir_reservas(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), file)) {
        char sala[50], data[50], horario_inicio[50], horario_fim[50];
        
        // Use sscanf para separar os campos na linha
        if (sscanf(linha, "%49[^,],%49[^,],%49[^,],%49[^,\n]", sala, data, horario_inicio, horario_fim) == 4) {
            printf("Sala: %s\n", sala);
            printf("Data: %s\n", data);
            printf("Horário de início: %s\n", horario_inicio);
            printf("Horário de fim: %s\n", horario_fim);
            printf("\n");
        }
    }

    fclose(file);
}

int cadastrar_usuario(const char *filename, const char login[20], const char senha[40]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), file)) {
        char arquivo_login[20];
        
        // Use sscanf para separar os campos na linha
        if (sscanf(linha, "%19[^,],", arquivo_login) == 1) {
            // Verifica se o login já existe
            if (strcmp(login, arquivo_login) == 0) {
                fclose(file);
                return 2; // Login já existe
            }
        }
    }

    fclose(file);

    file = fopen(filename, "a");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return 3;
    }

    fprintf(file, "%s,%s\n", login, senha);
    fclose(file);

    return 0; // Usuário cadastrado com sucesso
}

int autenticar_usuario(const char *filename, const char login[20], const char senha[40]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), file)) {
        char arquivo_login[20], arquivo_senha[40];
        
        // Use sscanf para separar os campos na linha
        if (sscanf(linha, "%19[^,],%39[^\n]", arquivo_login, arquivo_senha) == 2) {
            // Verifica se o login e a senha correspondem
            if (strcmp(login, arquivo_login) == 0 && strcmp(senha, arquivo_senha) == 0) {
                fclose(file);
                return 0; // Usuário autenticado
            }
        }
    }

    fclose(file);
    return 2; // Usuário não autenticado
}