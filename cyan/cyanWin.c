#include "cyan.h"

gint delete_event(GtkWidget *widget, GdkEvent event, gpointer data) {

	return FALSE;
}

void hello(GtkWidget *widget, gpointer data) {

	g_print("hello");
}

void end_program(GtkWidget *widget, gpointer data) {

	gtk_main_quit();
}

//CyanWin *

void create_main_window(void)
{
	GtkWidget *window;
	GtkWidget *vbox;
 	GtkWidget *menubar;
 	GtkWidget *sw;
 	GtkWidget *view;

 	CyanWin *cw = g_malloc(sizeof(CyanWin));
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	
	gtk_widget_set_name(window, "Win");

	g_signal_connect(G_OBJECT(window), "delete-event", G_CALLBACK(delete_event), NULL);

	g_signal_connect(window, "destroy", G_CALLBACK(end_program), NULL);

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	
	//menubar = create_menu_bar(window);
	//gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);
	
	sw = gtk_scrolled_window_new(NULL, NULL);

	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(sw), GTK_SHADOW_IN);
	
	gtk_box_pack_start(GTK_BOX(vbox), sw, TRUE, TRUE, 0);

	//view = create_text_view();

	gtk_container_add(GTK_CONTAINER(sw), view);

	cw->window = window;
	cw->menubar = menubar;
	cw->view = view;
	cw->buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
	
//	return cw;
}
/*
void set_main_window_title(void)
{
	gchar *title;
	
	//title = get_file_basename(pub->fi->filename, TRUE);
	gtk_window_set_title(NULL, title);
	g_free(title);
}
*/
int main(int argc, char** argv) {

	gtk_init(&argc, &argv);

	create_main_window();

	gtk_main();

	return 0;
}