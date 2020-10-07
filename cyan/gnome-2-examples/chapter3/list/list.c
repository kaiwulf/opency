/* -*-coding: utf-8;-*- */
/* list.c -- demonstrate GtkTreeView */

#include <gtk/gtk.h>

const char *numbers[] = { "Zero", "One", "Two", "Three", "Four", "Five" };

enum {
   INT_COLUMN,
   STRING_COLUMN,
   N_COLUMNS
};

/* standard event handlers */
gint delete_event(GtkWidget *widget, GdkEvent event, gpointer data)
{
  return FALSE;
}

void end_program(GtkWidget *widget, gpointer data)
{
  gtk_main_quit();
}

int main(int argc, char **argv)
{
  GtkWindow *window;
  GtkListStore *list;
  GtkTreeIter iter;
  GtkTreeView *view;
  GtkTreeViewColumn *num_column, *word_column;
  GtkCellRenderer *text_renderer;
  gint i;

  /* initialize GTK+, create main window */
  gtk_init(&argc, &argv);
  window = g_object_new(GTK_TYPE_WINDOW,
                        "title", "Two Column List",
                        "default-width", 300,
                        NULL);

  /* connect standard handlers */
  g_signal_connect(window, "delete_event", G_CALLBACK(delete_event), NULL);
  g_signal_connect(window, "destroy", G_CALLBACK(end_program), NULL);

  /* create a two-column list */
  list = gtk_list_store_new(N_COLUMNS, G_TYPE_INT, G_TYPE_STRING);

  /* put some data into the list */
  for (i = 0; i < 5; i++)
  {
     gtk_list_store_append(list, &iter);
     gtk_list_store_set(list, &iter,
                        INT_COLUMN, i,
                        STRING_COLUMN, numbers[i],
                        -1);
  }

  /* create tree view for the list */
  view = g_object_new(GTK_TYPE_TREE_VIEW,
                      "model", list,
                      "rules-hint", TRUE,
                      "headers-clickable", TRUE,
                      "reorderable", TRUE,
                      "enable-search", TRUE,
                      "search-column", STRING_COLUMN,
                      NULL);

  /* create and initialize text renderer for cells */
  text_renderer = gtk_cell_renderer_text_new();

  /* create column views */
  num_column = gtk_tree_view_column_new_with_attributes("Numeral",
                                                        text_renderer,
                                                        "text", INT_COLUMN,
                                                        NULL);

  g_object_set(num_column,
               "resizable", TRUE,
               "clickable", TRUE,
               "reorderable", TRUE,
               NULL);

  word_column = gtk_tree_view_column_new_with_attributes("Word",
                                                         text_renderer,
                                                         "text", STRING_COLUMN,
                                                         NULL);

  g_object_set(word_column,
               "resizable", TRUE,
               "clickable", TRUE,
               "reorderable", TRUE,
               NULL);

  /* insert columns into the view */
  gtk_tree_view_append_column(view, num_column);
  gtk_tree_view_append_column(view, word_column);

  /* pack/show everything; start GTK+ main event loop */
  gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(view));
  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_main();

  return 0;
}

