#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>


GtkBuilder *builder;
GtkWidget *window;


int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("interface_salas.glade");

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
