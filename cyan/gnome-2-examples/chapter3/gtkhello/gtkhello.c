/* -*-coding: utf-8;-*- */
/* gtkhello.c -- traditional GTK+ Hello program */

#include <gtk/gtk.h>

void hello(GtkWidget *widget, gpointer data)
{
  g_print("Hello, World!\n");
}

/* standard event handlers */
gint delete_event(GtkWidget *widget, GdkEvent event, gpointer data)
{
  /* when this function returns FALSE, the delete-event
     signal becomes a destroy signal */
  return FALSE;
}

void end_program(GtkWidget *widget, gpointer data)
{
  /* End the main loop */
  gtk_main_quit();
}

int main(int argc, char **argv)
{
  GtkWindow *window;
  GtkButton *button;

  /* Initialize GTK+ */
  gtk_init(&argc, &argv);
 
  /* create window, set default height and width to 200 pixels */
  window = g_object_new(GTK_TYPE_WINDOW,
                        "default-height", 200,
                        "default-width", 200,
                        "border-width", 12,
                        "title", "GtkHello",
                        NULL);

  /* add signal handlers for window */
  g_signal_connect(window,
                   "delete-event", G_CALLBACK(delete_event),
                   NULL);

  g_signal_connect(window,
                   "destroy", G_CALLBACK(end_program),
                   NULL);

  /* create a button */
  button = g_object_new(GTK_TYPE_BUTTON,
                        "label", "_Hello, World!\nClick Here.",
                        "use-underline", TRUE,
                        NULL);

  /* install signal handlers for button */
  g_signal_connect(button,
                   "clicked", G_CALLBACK(hello),
                   NULL);

  g_signal_connect_swapped(button,
                           "clicked", G_CALLBACK(gtk_widget_destroy),
                           window);

  /* pack the button into the window, show all of its contents */
  gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(button));

  gtk_widget_show_all(GTK_WIDGET(window));

  /* start main event loop */
  gtk_main();

  return 0;
}
