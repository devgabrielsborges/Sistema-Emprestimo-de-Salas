#ifndef UI_FUNCTIONS_H
#define UI_FUNCTIONS_H


#include <gtk/gtk.h>

// // Tela login
void on_botao_login_clicked(GtkButton *b, gpointer user_data);
void on_botao_novo_usuario_clicked(GtkButton *b, gpointer user_data);
    
// Tela cadastro usuarios
void on_botao_cadastrar_usuario_clicked(GtkButton *b, gpointer user_data);

// Tela reservas e cancelamentos
void on_botao_cancelar_reserva_clicked(GtkButton *b);
void on_botao_inserir_reservas_clicked(GtkButton *b);

// Tela informacoes para reserva
void on_botao_cancelar_operacao_clicked(GtkButton *b);
void on_botao_reservar_clicked(GtkButton *b);

#endif // UI_FUNCTIONS_H
