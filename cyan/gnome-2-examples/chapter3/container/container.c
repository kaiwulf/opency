/* -*-coding: utf-8;-*- */
/* container.c -- container demo */

#include <gtk/gtk.h>

/* standard handlers */
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
  GtkHPaned *h_pane;
  GtkVPaned *v_pane;
  GtkVButtonBox *button_column; /* button box elements */
  GtkButton *button[3];
  GtkTable *table;              /* table elements */
  GtkButton *tictac[3][3];
  gint i, j;
  GtkNotebook *notebook;        /* notebook elements */
  GtkLabel *page_1_content;
  GtkImage *page_2_apple;
  GtkButton *page_3_button;
  GtkLabel *page_1_title, *page_2_title, *page_3_title;

  /* initialize GTK+, create a window, attach handlers */
  gtk_init(&argc, &argv);
  window = g_object_new(GTK_TYPE_WINDOW,
                        "title", "Container Madness",
                        "default_height", 200,
                        "default_width", 300,
                        "border-width", 12,
                        NULL);

  /* attach standard event handlers */
  g_signal_connect(window, "delete_event", G_CALLBACK(delete_event), NULL);
  g_signal_connect(window, "destroy", G_CALLBACK(end_program), NULL);

  /* Divide window horizontally with a pane */
  h_pane = g_object_new(GTK_TYPE_HPANED, NULL);
  gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(h_pane));

  /* create a vertical paned container and put that
     in the left side of the horizontal pane above */
  v_pane = g_object_new(GTK_TYPE_VPANED, NULL);
  gtk_paned_add1(GTK_PANED(h_pane), GTK_WIDGET(v_pane));

  /* create three buttons */
  button[0] = g_object_new(GTK_TYPE_BUTTON, "label", "Foo", NULL);
  button[1] = g_object_new(GTK_TYPE_BUTTON, "label", "Bar", NULL);
  button[2] = g_object_new(GTK_TYPE_BUTTON, "label", "Baz", NULL);

  /* put the buttons in a vertical button box */
  button_column = g_object_new(GTK_TYPE_VBUTTON_BOX, NULL);
  for (i=0; i<3; i++)
  {
     gtk_box_pack_start_defaults(GTK_BOX(button_column), GTK_WIDGET(button[i]));
  }
  /* put the vertical button box into the top pane of v_pane, from earlier */
  gtk_paned_add1(GTK_PANED(v_pane), GTK_WIDGET(button_column));

  /* create a 3x3 table container */
  table = g_object_new(GTK_TYPE_TABLE,
                       "n-rows", 3,
                       "n-columns", 3,
                       "homogeneous", TRUE,
                       NULL);

  /* fill the table with some buttons */
  for (i=0; i<3; i++)
  {
     for (j=0; j<3; j++)
     {
        tictac[i][j] = g_object_new(GTK_TYPE_BUTTON, NULL);
        gtk_table_attach_defaults(table,
                                  GTK_WIDGET(tictac[i][j]),
                                  i, i+1, j, j+1);
     }
  }
  /* label the buttons in the table's diagonal */
  g_object_set(tictac[0][0], "label", "Tic", NULL);
  g_object_set(tictac[1][1], "label", "Tac", NULL);
  g_object_set(tictac[2][2], "label", "Toe", NULL);

  /* put the table in the lower pane of v_pane, from above */
  gtk_paned_add2(GTK_PANED(v_pane), GTK_WIDGET(table));

  /* create a notebook... */
  notebook = g_object_new(GTK_TYPE_NOTEBOOK, NULL);

  /* put the notebook in the window's right pane */
  gtk_paned_add2(GTK_PANED(h_pane), GTK_WIDGET(notebook));
  
  /* create notebook's page 1, containing only a label */
  page_1_content = g_object_new(GTK_TYPE_LABEL, "label", "Page 1!", NULL);

  /* create page 1's title ("This") */
  page_1_title = g_object_new(GTK_TYPE_LABEL, "label", "This", NULL);

  /* add the page to the notebook */
  gtk_notebook_append_page_menu(notebook,
                                GTK_WIDGET(page_1_content),
                                GTK_WIDGET(page_1_title),
                                NULL);

  /* add another page containing an apple image */
  page_2_apple = g_object_new(GTK_TYPE_IMAGE, "file", "apple-green.png", NULL);
  page_2_title = g_object_new(GTK_TYPE_LABEL, "label", "That", NULL);
  gtk_notebook_append_page_menu(notebook,
                                GTK_WIDGET(page_2_apple),
                                GTK_WIDGET(page_2_title),
                                NULL);

  /* page 3 contains a button */
  page_3_button = g_object_new(GTK_TYPE_BUTTON, "label", "Click me", NULL);
  page_3_title = g_object_new(GTK_TYPE_LABEL, "label", "The Other", NULL);
  gtk_notebook_append_page_menu(notebook,
                                GTK_WIDGET(page_3_button),
                                GTK_WIDGET(page_3_title),
                                NULL);

  /* show the whole thing and start GTK+ main loop */
  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_main();

  return 0;
}

