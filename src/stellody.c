/** @brief

    @author
    @file
*/

#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

int main (int argc, char *argv[])
{
	GtkBuilder *builder = NULL;
	GtkWidget *window;

	gtk_init(&argc, &argv);

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "data/windows/Window.glade", NULL);
	gtk_builder_connect_signals(builder, NULL);

	window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
	gtk_widget_show_all(window);

	gtk_main();

	return 0;
}
