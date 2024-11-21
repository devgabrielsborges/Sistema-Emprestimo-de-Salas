#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "csv_operations.h"

/*
    TODO: Testar as funções de csv_operations.c

    bool verificar_registro(const char *filename, const char *sala, const char *data, const char *horario_inicio, const char *horario_fim);

    int agendar_horario_sala(const char *filename, const char *sala, const char *data, const char *horario_inicio, const char *horario_fim);

    void apagar_reserva(const char *filename, const char *sala, const char *data, const char *horario_inicio, const char *horario_fim);

    void exibir_reservas(const char *filename);

    int cadastrar_usuario(const char *filename, const char login[20], const char senha[40]);

    int autenticar_usuario(const char *filename, const char login[20], const char senha[40]);
*/

int main(int argc, char *argv[]) {
    GtkBuilder *builder;
    GtkWidget *window;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("interface.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));

    if (!window) {
        g_print("Erro ao carregar a janela principal!\n");
        return -1;
    }

    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}


// gcc -o my_program main.c `pkg-config --cflags --libs gtk+-3.0`

