#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "csv_operations.h"

// TODO: Implementar mudança de tela ao autenticar usuário
// TODO: Implementar função para popular lista de reservas
// TODO: Demais botões

GtkBuilder *builder;
GtkWidget *window;

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

        gtk_stack_set_visible_child_name(stack, "box_tela_geral");
    }
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("interface_salas.glade");
    if (!builder) {
        g_critical("Failed to create GtkBuilder");
        return 1;
    }

    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    if (!window) {
        g_critical("Failed to get main window");
        return 1;
    }

    gtk_builder_connect_signals(builder, builder);
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}