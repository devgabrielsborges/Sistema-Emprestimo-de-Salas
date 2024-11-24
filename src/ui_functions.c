#include <gtk/gtk.h>
#include "ui_functions.h"
#include "csv_operations.h"


// TODO: Implementar função para popular lista de reservas
// TODO: Demais botões

// // Tela login
void on_botao_login_clicked(GtkButton *b, gpointer user_data) {
    // Definindo as entradas
    GtkBuilder *builder = GTK_BUILDER(user_data);
    GtkEntry *entry_login = GTK_ENTRY(gtk_builder_get_object(builder, "entry_login"));
    GtkEntry *entry_senha = GTK_ENTRY(gtk_builder_get_object(builder, "entry_senha"));

    int autenticado = 1;

    if (!entry_login || !entry_senha) {
        g_critical("Falha ao obter as entradas");
        return;
    }

    const char *login = gtk_entry_get_text(entry_login);
    const char *senha = gtk_entry_get_text(entry_senha);

    // se login e senha forem diferentes de "" -> chamar autenticar_usuario
    if (strcmp(login, "") != 0 && strcmp(senha, "") != 0) {
        autenticado = autenticar_usuario("../data/login.csv", login, senha);
        printf("Autenticado: %d\n", autenticado);
        if (autenticado == 0) {
            g_message("Usuário autenticado com sucesso");
        } else if (autenticado == 1) {
            g_message("Usuário ou senha incorretos");
        } else {
            g_critical("Erro ao autenticar usuário");
        }
    } else {
        g_message("Preencha todos os campos");
    }

    if (autenticado == 0) {
        // Exibir a box denominada box_tela_geral na GtkStack
        GtkStack *stack = GTK_STACK(gtk_builder_get_object(builder, "stack"));
        if (!stack) {
            g_critical("Falha ao obter a GtkStack");
            return;
        }

        // TODO: Implementar função para popular lista de reservas
        // exibir_reservas("../data/reservas.csv");
        gtk_stack_set_visible_child_name(stack, "box_tela_geral");
    }
}


void on_botao_novo_usuario_clicked(GtkButton *b, gpointer user_data) {
    // Passando para tela de cadastro de usuário
    GtkBuilder *builder = GTK_BUILDER(user_data);
    GtkStack *stack = GTK_STACK(gtk_builder_get_object(builder, "stack"));
        if (!stack) {
            g_critical("Falha ao obter a GtkStack");
            return;
        }
    gtk_stack_set_visible_child_name(stack, "box_cadastro_usuario");
}


void on_botao_cadastrar_usuario_clicked(GtkButton *b, gpointer user_data)
{
        
    // Definindo as entradas
    GtkBuilder *builder = GTK_BUILDER(user_data);
    GtkEntry *entry_login = GTK_ENTRY(gtk_builder_get_object(builder, "entry_inserir_nome"));
    GtkEntry *entry_senha = GTK_ENTRY(gtk_builder_get_object(builder, "entry_inserir_senha"));
    GtkEntry *entry_confirmar_senha = GTK_ENTRY(gtk_builder_get_object(builder, "entry_confirmar_senha"));

    if (!entry_login || !entry_senha || !entry_confirmar_senha) {
        g_critical("Falha ao obter as entradas");
        return;
    }

    const char *login = gtk_entry_get_text(entry_login);
    const char *senha = gtk_entry_get_text(entry_senha);
    const char *confirmar_senha = gtk_entry_get_text(entry_confirmar_senha);

    if (strcmp(login, "") != 0 && strcmp(senha, "") != 0 && strcmp(confirmar_senha, "") != 0) {
        if (strcmp(senha, confirmar_senha) == 0) {
            int cadastrado = cadastrar_usuario("../data/login.csv", login, senha);
            if (cadastrado == 0) {
                g_message("Usuário cadastrado com sucesso");
            } else if (cadastrado == 1) {
                g_message("Usuário já cadastrado");
            } else {
                g_critical("Erro ao cadastrar usuário");
            }
        } else {
            g_message("As senhas não coincidem");
        }
    } else {
        g_message("Preencha todos os campos");
    }


    GtkStack *stack = GTK_STACK(gtk_builder_get_object(builder, "stack"));
    if (!stack) {
        g_critical("Falha ao obter a GtkStack");
        return;
    }
}
