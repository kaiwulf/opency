/* -*-coding: utf-8;-*- */
/* scroll.c -- demonstrate scrolled window */

#include <gtk/gtk.h>

/* standard window handlers */
gint delete_event(GtkWidget *widget, GdkEvent event, gpointer data)
{
  return FALSE;
}

void end_program(GtkWidget *widget, gpointer data)
{
  gtk_main_quit();
}

/* number of buttons to a side */
#define TABLE_SIZE 30

int main(int argc, char **argv)
{
  GtkWindow *window;
  GtkTable *table;
  GtkButton *button[TABLE_SIZE][TABLE_SIZE];
  GtkScrolledWindow *scr_window;
  gint i, j;
  gchar *string;

  /* initialize GTK+, create window */
  gtk_init(&argc, &argv);
  window = g_object_new(GTK_TYPE_WINDOW,
                        "title", "Scrolled Window",
                        "default-width", 600,
                        "default-height", 400,
                        NULL);

  /* attach standard window handlers */
  g_signal_connect(window, "delete_event", G_CALLBACK(delete_event), NULL);
  g_signal_connect(window, "destroy", G_CALLBACK(end_program), NULL);

  /* create a TABLE_SIZE**2 buttons inside */
  table = g_object_new(GTK_TYPE_TABLE,
                       "n-rows", TABLE_SIZE,
                       "n-columns", TABLE_SIZE,
                       "homogeneous", TRUE,
                       NULL);

  for (i=0; i < TABLE_SIZE; i++)
  {
     for (j=0; j < TABLE_SIZE; j++)
     {
        string = g_strdup_printf("Button (%d, %d)", i, j);
        button[i][j] = g_object_new(GTK_TYPE_BUTTON, "label", string, NULL);
        g_free(string);
        gtk_table_attach_defaults(table, GTK_WIDGET(button[i][j]),
                                  i, i+1, j, j+1);
     }
  }

  /* create scrolled window */
  scr_window = g_object_new(GTK_TYPE_SCROLLED_WINDOW, NULL);

  /* pack the table into the window with a viewport between */
  gtk_scrolled_window_add_with_viewport(scr_window, GTK_WIDGET(table));

  /* pack scrolled window and show everything  */
  gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(scr_window));

  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_main();
  return 0;
}

