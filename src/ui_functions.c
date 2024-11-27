#include <gtk/gtk.h>
#include "ui_functions.h"
#include "csv_operations.h"

// TODO: Terminar função para popular lista de reservas
// TODO: Demais botões


void exibir_mensagem(GtkBuilder *builder, const char *mensagem) {
    // Obtém o GtkMessageDialog do arquivo Glade
    GtkWidget *dialog = GTK_WIDGET(gtk_builder_get_object(builder, "message_padrao"));

    if (!dialog) {
        g_critical("Erro: Não foi possível encontrar o GtkMessageDialog no Glade.");
        return;
    }

    // Define o texto da mensagem
    GtkWidget *label = GTK_WIDGET(gtk_builder_get_object(builder, "label_mensagem"));
    if (label) {
        gtk_label_set_text(GTK_LABEL(label), mensagem);
    } else {
        g_critical("Erro: Não foi possível encontrar a label_mensagem no Glade.");
    }

    // Exibe a caixa de diálogo
    gtk_widget_show(dialog);
}

void on_ok_clicked(GtkWidget *button, gpointer user_data)
{

    // Obtém o topo da hierarquia, que é o GtkDialog
    GtkWidget *dialog = gtk_widget_get_toplevel(button);


    if (GTK_IS_DIALOG(dialog)) {
        gtk_dialog_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK); // Finaliza o ciclo do diálogo
        gtk_widget_hide(dialog); // Oculta o diálogo
    }
}


void popular_lista_reservas(GtkBuilder *builder, const char *bloco, char *sala, char *data) {
    char *reservas[NUM_HORARIOS] = {NULL};
    char nome_arquivo[TAM_LINHA];

    pegar_nome_arquivo(nome_arquivo, bloco);
    carregar_reservas(nome_arquivo, reservas, sala, data);

    GtkListBox *lista = GTK_LIST_BOX(gtk_builder_get_object(builder, "lista_reservas"));
    if (!lista) {
        g_critical("Falha ao obter a lista");
        return;
    }

    gtk_container_foreach(GTK_CONTAINER(lista), (GtkCallback)gtk_widget_destroy, NULL);

    // Populando a lista com verificações
    for (int i = 0; i < NUM_HORARIOS; i++) {
        if (reservas[i] != NULL && strlen(reservas[i]) > 0 && g_strstrip(g_strdup(reservas[i]))[0] != '\0') {
            GtkWidget *label = gtk_label_new(reservas[i]);

            GtkStyleContext *context = gtk_widget_get_style_context(label);
            gtk_style_context_add_class(context, "label-lista");
            
            gtk_list_box_insert(lista, label, -1);
        } else {
            printf("Reserva inválida na posição %d\n", i);
        }
    }

    gtk_widget_show_all(GTK_WIDGET(lista));
    liberar_reservas(reservas);
}



// Tela login
void on_botao_login_clicked(GtkButton *b, gpointer user_data) {
    // Definindo as entradas
    GtkBuilder *builder = GTK_BUILDER(user_data);
    GtkEntry *entry_login = GTK_ENTRY(gtk_builder_get_object(builder, "entry_login"));
    GtkEntry *entry_senha = GTK_ENTRY(gtk_builder_get_object(builder, "entry_senha"));

    GtkEntry *entry_data = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(builder), "entry_data"));

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

        
        char data_atual[TAM_DATA];
        pegar_data_atual(data_atual);

        gtk_entry_set_text(entry_data, data_atual);
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


void on_botao_cadastrar_usuario_clicked(GtkButton *b, gpointer user_data) {
        
    GtkBuilder *builder = GTK_BUILDER(user_data);
    GtkStack *stack = GTK_STACK(gtk_builder_get_object(builder, "stack"));

     // Definindo as entradas
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
                exibir_mensagem(builder, "Usuário cadastrado com sucesso");
                gtk_stack_set_visible_child_name(stack, "box_login");  // voltar para tela de login

            } 
            else if (cadastrado == 1) {
                exibir_mensagem(builder, "Usuário já cadastrado");
            } 
            else {
                exibir_mensagem(builder, "Erro ao cadastrar usuário");
            }
        } 
        else {
            exibir_mensagem(builder, "Senhas não conferem");
        }
    } 
    else {
        exibir_mensagem(builder, "Preencha todos os campos");
        
    }


    if (!stack) {
        g_critical("Falha ao obter a GtkStack");
        return;
    }
}


void on_calendar_day_selected(GtkCalendar *calendar, gpointer user_data) {
    GtkBuilder *builder = GTK_BUILDER(user_data);
    GtkEntry *entry_data = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(builder), "entry_data"));

    guint ano, mes, dia;
    gchar texto_data[TAM_DATA];

    // Pegando a data selecionada no calendário
    gtk_calendar_get_date(calendar, &ano, &mes, &dia);

    // Passando data para a entry
    g_snprintf(texto_data, sizeof(texto_data), "%02d/%02d/%04d", dia, mes + 1, ano);

    // Setando a data na entry
    gtk_entry_set_text(entry_data, texto_data);

    // Atualizando a lista de reservas - se houver bloco e sala selecionados
    GtkComboBoxText *bloco_combobox = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "bloco_combobox"));
    GtkComboBoxText *sala_combobox = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "sala_combobox"));

    char *bloco = gtk_combo_box_text_get_active_text(bloco_combobox);
    char *sala = gtk_combo_box_text_get_active_text(sala_combobox);

    if (bloco && sala) {
        popular_lista_reservas(builder, bloco, sala, texto_data);
    }
}


void on_entry_data_activate(GtkEntry *entry_data, gpointer user_data) {
    const char *texto_data = gtk_entry_get_text(entry_data);
    guint ano, mes, dia;

    // Tenta analisar a data no formato DD/MM/YYYY
    if (sscanf(texto_data, "%02d/%02d/%04d", &dia, &mes, &ano) == 3) {
        // Obtém o GtkCalendar do GtkBuilder (certifique-se de passar o GtkBuilder correto)
        GtkBuilder *builder = GTK_BUILDER(user_data);
        GtkCalendar *calendar = GTK_CALENDAR(gtk_builder_get_object(builder, "calendar"));
        
        if (calendar) {
            gtk_calendar_select_day(calendar, dia);
            gtk_calendar_select_month(calendar, mes - 1, ano);
        } 
    } 
    else {
        g_warning("Data inválida: %s", texto_data);
    }
}

void atualizar_sala_combobox(GtkComboBoxText *sala, const char *bloco) {
    GtkComboBoxText *sala_combobox = GTK_COMBO_BOX_TEXT(sala);
    char salas[NUM_SALAS][MAX_TAM_SALA];

    // Obtendo as salas do bloco
    for (int i = 0; i < NUM_SALAS; i++) {
        snprintf(salas[i], sizeof(salas[i]), "%s%02d", bloco, i + 1);
    }

    // Limpa o combobox
    gtk_combo_box_text_remove_all(sala_combobox);

    // Adiciona os itens
    for (int i = 0; i < NUM_SALAS; i++) {
        gtk_combo_box_text_append_text(sala_combobox, salas[i]);
    }
}


void on_bloco_combobox_changed(GtkComboBox *bloco_combobox, gpointer user_data) {
    GtkBuilder *builder = GTK_BUILDER(user_data);
    GtkComboBoxText *sala_combobox = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "sala_combobox"));

    // Obtendo o bloco selecionado
    const char *bloco = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(bloco_combobox));

    // Limpar itens do combobox
    gtk_combo_box_text_remove_all(sala_combobox);

     // Atualiza os itens de combo2 com base no item selecionado em combo1
    atualizar_sala_combobox(sala_combobox, bloco);


}


void on_sala_combobox_changed(GtkComboBoxText *sala_combobox, gpointer user_data) {
    GtkBuilder *builder = GTK_BUILDER(user_data);
    GtkComboBoxText *bloco_combobox = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "bloco_combobox"));

    // Texto do bloco
    char *bloco = gtk_combo_box_text_get_active_text(bloco_combobox);

    // Texto do sala_combobox
    char *sala = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(sala_combobox));

    // Texto da entry_data
    char *data = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_data")));
    
    if (sala && bloco && data) {
        popular_lista_reservas(builder, bloco, sala, data);
    }
    
}
