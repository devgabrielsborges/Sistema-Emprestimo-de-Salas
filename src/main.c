#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "csv_operations.h"


int main(int argc, char *argv[]) {
    
    GtkBuilder *builder;
    GtkWidget *window;
    
    // Tela login
    void on_botao_login_clicked(GtkButton *b); 
    void on_botao_novo_usuario_clicked(GtkButton *b);
    
    // Tela cadastro usuarios
    void on_botao_cadastrar_usuario_clicked(GtkButton *b);

    // Tela reservas e cancelamentos
    void on_botao_cancelar_reserva_clicked(GtkButton *b);
    void on_botao_inserir_reservas_clicked(GtkButton *b);
    
    void on_botao_cancelar_operacao_clicked(GtkButton *b);
    void on_botao_reservar_clicked(GtkButton *b);

    
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("interface_salas.glade");
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


// gcc -o emprestimo main.c `pkg-config --cflags --libs gtk+-3.0`

