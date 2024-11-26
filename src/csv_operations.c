#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "csv_operations.h"


// TODO: Implementar array com horários
// TODO: Verificar sobrescrita de horários

const char *horarios[] = {
    "07:10", "08:00", "08:50", "09:40", "10:50", "11:40", "12:30", 
    "13:20", "14:30", "15:20", "16:10", "17:20", "18:10", "19:00", 
    "19:50", "20:40", "21:30", "22:20", "23:10"
}


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
        return 2;
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), file)) {
        char arquivo_login[20];
        
        // Use sscanf para separar os campos na linha
        if (sscanf(linha, "%19[^,],", arquivo_login) == 1) {
            // Verifica se o login já existe
            if (strcmp(login, arquivo_login) == 0) {
                fclose(file);
                return 1; // Login já existe
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

int autenticar_usuario(const char *filename, const char *login, const char *senha) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return 2;
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), file)) {
        // Removendo o caractere de nova linha, se existir
        linha[strcspn(linha, "\n")] = 0;

        // Separando os campos da linha em login e senha
        char *file_login = strtok(linha, ",");
        char *file_senha = strtok(NULL, ",");

        // Removendo espaços em branco no início e no final
        if (file_login) {
            while (*file_login == ' ') file_login++;
            char *end = file_login + strlen(file_login) - 1;
            while (end > file_login && *end == ' ') end--;
            *(end + 1) = '\0';
        }

        if (file_senha) {
            while (*file_senha == ' ') file_senha++;
            char *end = file_senha + strlen(file_senha) - 1;
            while (end > file_senha && *end == ' ') end--;
            *(end + 1) = '\0';
        }

        if (strcmp(login, file_login) == 0 && strcmp(senha, file_senha) == 0) {
            fclose(file);
            return 0; // Usuário autenticado com sucesso
        }
    }

    fclose(file);
    return 1; // Usuário ou senha incorretos
}


void pegar_data_atual(char *data) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(data, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

