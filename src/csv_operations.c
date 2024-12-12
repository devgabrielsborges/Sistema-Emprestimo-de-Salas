#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "csv_operations.h"


char *horarios[] = {
    "07:10", "08:00", "08:50", "09:40", "10:30", "11:20", "12:10", 
    "13:00", "13:50", "14:40", "15:30", "16:20", "17:10", "18:00",
    "18:50", "19:40", "20:30", "21:20", "22:10"
};


int pegar_indice_horario(const char *horario) {
    register int i = 0;

    for (i; i < NUM_HORARIOS; i++) {
        if (strcmp(horarios[i], horario) == 0) {
            return i;
        }
    }

    return -1;  // Horário não encontrado
}


void pegar_data_atual(char *data) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(data, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}


bool verificar_registro(const char *filename, const char *sala, const char *data, const char *horario_inicio, const char *horario_fim) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return false;
    }

    char linha[TAM_LINHA];
    
    while (fgets(linha, sizeof(linha), file)) {
        char arquivo_sala[MAX_TAM_SALA], arquivo_data[TAM_DATA], arquivo_professor[MAX_TAM_INFO], arquivo_disciplina[MAX_TAM_INFO], arquivo_turma[MAX_TAM_INFO], arquivo_horario_inicio[TAM_HORARIO], arquivo_horario_fim[TAM_HORARIO];
        
        // sscanf para separar os campos na linha
        if (sscanf(linha, "%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,\n]", arquivo_sala, arquivo_data, arquivo_professor, arquivo_disciplina, arquivo_turma, arquivo_horario_inicio, arquivo_horario_fim) == 7) {
            // verifica se todos os campos correspondem
            // verificando pelo indice dos horarios
            int indice_inicio_arquivo = pegar_indice_horario(arquivo_horario_inicio);
            int indice_fim_arquivo = pegar_indice_horario(arquivo_horario_fim);
            int indice_inicio = pegar_indice_horario(horario_inicio);
            int indice_fim = pegar_indice_horario(horario_fim);

            if (strcmp(sala, arquivo_sala) == 0 && strcmp(data, arquivo_data) == 0 &&
                indice_inicio >= indice_inicio_arquivo && indice_fim == indice_fim_arquivo) {
                fclose(file);
                return true; // Registro já existe
            }
        }
    }

    fclose(file);
    return false; // Registro não existe
}

// Sala, Data, Professor, Disciplina, Turma, Horario_inicio, Horario_fim
int agendar_horario_sala(const char *filename, const char *sala, const char *data, const char *horario_inicio, const char *horario_fim, const char *professor, const char *disciplina, const char *turma) {
    FILE *file = fopen(filename, "a");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return 2;
    }

    fprintf(file, "%s,%s,%s,%s,%s,%s,%s\n", sala, data, professor, disciplina, turma, horario_inicio, horario_fim);
    fclose(file);

    return 0; // Registro realizado com sucesso
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

    char arquivo_sala[MAX_TAM_SALA], arquivo_data[TAM_DATA], arquivo_professor[MAX_TAM_INFO], arquivo_disciplina[MAX_TAM_INFO], arquivo_turma[MAX_TAM_INFO], arquivo_horario_inicio[TAM_HORARIO], arquivo_horario_fim[TAM_HORARIO];

    int indice_inicio_arquivo;
    int indice_fim_arquivo;
    int indice_inicio = pegar_indice_horario(horario_inicio);
    int indice_fim = pegar_indice_horario(horario_fim);

    char linha[TAM_LINHA];
    while (fgets(linha, sizeof(linha), file)) {
        indice_inicio_arquivo = pegar_indice_horario(arquivo_horario_inicio);
        indice_fim_arquivo = pegar_indice_horario(arquivo_horario_fim) - 1;
        
        // Use sscanf para separar os campos na linha
        if (sscanf(linha, "%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,\n]", arquivo_sala, arquivo_data, arquivo_professor, arquivo_disciplina, arquivo_turma, arquivo_horario_inicio, arquivo_horario_fim) == 7) {
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

}

// Função para remover espaços em branco e novas linhas
void trim(char *str) {
    char *end;

    // Trim espacos
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0)  // All spaces?
        return;

    // Trim espacos no final
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Caracter nulo no final
    end[1] = '\0';
}

void pegar_nome_arquivo(char *nome_arquivo, const char *bloco)
{
    sprintf(nome_arquivo, "../data/%s.csv", bloco);
}

void carregar_reservas(const char *filename, char *reservas[NUM_HORARIOS], char *sala, char *data) {

    int indice_horario_inicio;
    int indice_horario_fim;

    // Inicializando o array de reservas
    for (int i = 0; i < NUM_HORARIOS; i++) {
        reservas[i] = NULL;
    }

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    char linha[TAM_LINHA];
    while (fgets(linha, sizeof(linha), file)) {
        char arquivo_data[TAM_DATA], arquivo_sala[MAX_TAM_SALA], professor[MAX_TAM_INFO], disciplina[MAX_TAM_INFO], turma[MAX_TAM_INFO], horario_inicio[TAM_HORARIO], horario_fim[TAM_HORARIO];
        
        // Para separar os campos na linha por virgula
        if (sscanf(linha, "%19[^,],%19[^,],%19[^,],%29[^,],%4[^,],%19[^,],%19[^,\n]",
                   arquivo_sala, arquivo_data, professor, disciplina, turma, horario_inicio, horario_fim) == 7) {
            printf("linha: %s\n", linha);
            // Remover espaços em branco e novas linhas
            trim(arquivo_sala);
            trim(arquivo_data);


            if (strcmp(sala, arquivo_sala) == 0 && strcmp(data, arquivo_data) == 0) {
                indice_horario_inicio = pegar_indice_horario(horario_inicio);
                indice_horario_fim = pegar_indice_horario(horario_fim);

                printf("horario valido\n");
                printf("indice horario inicio -> %d\n", indice_horario_inicio);
                printf("indice horario fim -> %d\n", indice_horario_fim);

                

                // Verificar se os índices são válidos
                if (indice_horario_inicio == -1 || indice_horario_fim == -1) {
                    printf("erro horario\n");
                    continue;
                }


                for (int i = indice_horario_inicio; i < indice_horario_fim; i++) {
                    reservas[i] = malloc(TAM_LINHA * sizeof(char));
                    if (reservas[i] == NULL) {
                        printf("erro mem\n");
                        perror("Erro ao alocar memória");
                        fclose(file);
                        return;
                    }
                    printf("here\n");
                    snprintf(reservas[i],TAM_LINHA * sizeof(reservas[i]), "%s - %s - %s - %s", horarios[i], professor, disciplina, turma);
                    printf("reserva->%s", reservas[i]);
                }
            }
        }
    }

    // Percorrendo lista de reservas para preencher horarios não reservados
    for (int i = 0; i < NUM_HORARIOS; i++) {
        if (reservas[i] == NULL) {
            reservas[i] = strdup(horarios[i]);
            if (reservas[i] == NULL) {
                perror("Erro ao alocar memória");
                fclose(file);
                return;
            }
        }
    }

    fclose(file);
    
}


void liberar_reservas(char *reservas[NUM_HORARIOS]) {
    for (int i = 0; i < NUM_HORARIOS; i++) {
        if (reservas[i] != NULL) {
            free(reservas[i]);
        }
    }
}
int cadastrar_usuario(const char *filename, const char login[MAX_TAM_INFO], const char senha[MAX_TAM_INFO]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return 2;
    }

    char linha[TAM_LINHA];
    while (fgets(linha, sizeof(linha), file)) {
        char arquivo_login[MAX_TAM_INFO];
        
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

    char linha[TAM_LINHA];
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

void escrever_ultimo_login(const char *filename, const char *login, const char *senha)
{
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    fprintf(file, "%s,%s\n", login, senha);
    fclose(file);
}

void ler_ultimo_login(const char *filename, char *login, char *senha)
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    char linha[TAM_LINHA];

    if (fgets(linha, sizeof(linha), file)) {
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

        strlen(file_login) > 1 ? strcpy(login, file_login) : strcpy(login, NULL);
        strlen(file_senha) > 1 ? strcpy(senha, file_senha) : strcpy(senha, NULL);
    }

    fclose(file);
}


char lembrar_login_cond(char *filename)
{
    // pegar int no arquivo .lembrar_login

    FILE *file = fopen(filename, "r");

    if (!file) {
        perror("Erro ao abrir o arquivo");
        return 2;
    }

    char cond;  // arquivo so contem 1 char

    if (fscanf(file, "%c", &cond) == 1) {
        fclose(file);
        return cond - '0';
    }

    fclose(file);
    return 3;

    

}