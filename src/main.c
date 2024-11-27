#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "csv_operations.h"

#define NUM_HORARIOS 19

GtkBuilder *builder;
GtkWidget *window;


int main(int argc, char *argv[]) {
    char *reservas[NUM_HORARIOS];
    
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("interface_salas.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    if (!window) {
        g_critical("Falha em obter main_window");
        return 1;
    }

     // Carregar o arquivo CSS
    GtkCssProvider *css_provider = gtk_css_provider_new();
    if (!gtk_css_provider_load_from_path(css_provider, "style.css", NULL)) {
        g_warning("Não foi possível carregar o arquivo style.css");
    }

    // Aplicar o CSS globalmente
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );

    gtk_builder_connect_signals(builder, builder);
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
