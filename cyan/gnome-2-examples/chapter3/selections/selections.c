/* -*-coding: utf-8;-*- */
/* selections.c -- demonstrate selection widgets */

#include <gtk/gtk.h>

GtkFileSelection *file_browser;

/* standard event handlers */
gint delete_event(GtkWidget *widget, GdkEvent event, gpointer data)
{
  return FALSE;
}

void end_program(GtkWidget *widget, gpointer data)
{
  gtk_main_quit();
}

/* handler for "color-changed" signal, prints new color to console */
void print_color(GtkColorSelection *chooser, gpointer data)
{
  GdkColor color;
  guint red, green, blue;
  guint alpha;

  /* if user is still adjusting the widget, do nothing */
  if (!gtk_color_selection_is_adjusting(chooser))
  {
     g_object_get(chooser, "current-color", &color, NULL);
     red = color.red;
     green = color.green;
     blue = color.blue;

     g_object_get(chooser, "current-alpha", &alpha, NULL);
	  
     g_print("color chosen: R=%d, G=%d, B=%d, A=%d\n",
	     red, green, blue, alpha);
  }
}

/* prints name file selected in file_browser */
void print_filename(GtkButton *button, gpointer data)
{
  gchar *filename;

  g_object_get(file_browser, "filename", &filename, NULL);
  g_print("file chosen: %s\n", filename);
  g_free(filename);
}

/* creates the file browser and attaches signals */
void make_file_dialog(GtkButton *button, gpointer data)
{
  file_browser = g_object_new(GTK_TYPE_FILE_SELECTION, NULL);

  gtk_window_set_title(GTK_WINDOW(file_browser), "Select a File");

  g_signal_connect(file_browser->ok_button,
                   "clicked", G_CALLBACK(print_filename), NULL);

  g_signal_connect_swapped(file_browser->ok_button,
                           "clicked", G_CALLBACK(gtk_widget_destroy),
                           file_browser);

  g_signal_connect_swapped(file_browser->cancel_button,
                           "clicked", G_CALLBACK(gtk_widget_destroy),
                           file_browser);

  gtk_widget_show(GTK_WIDGET(file_browser));
}

int main(int argc, char **argv)
{
  GtkWindow *window;
  GtkColorSelection *color_chooser;
  GtkFontSelection *font_chooser;
  GtkButton *file_browser_button;
  GtkVBox *vbox;
  GtkLabel *color_label, *font_label, *file_label;

  /* initialize GTK+, create window */
  gtk_init(&argc, &argv);
  window = g_object_new(GTK_TYPE_WINDOW, "title", "Chooser Widgets", NULL);

  /* attach standard handlers */
  g_signal_connect(window, "delete_event", G_CALLBACK(delete_event), NULL);
  g_signal_connect(window, "destroy", G_CALLBACK(end_program), NULL);

  /* create vbox for main display */
  vbox = g_object_new(GTK_TYPE_VBOX, "spacing", 6,  "border-width", 12,  NULL);

  /* create a color chooser and label, pack them into a VBox */
  color_label = g_object_new(GTK_TYPE_LABEL,
                             "label", "<b>Color picker:</b>",
                             "use-markup", TRUE,
                             "xalign", 0.0,
                             NULL);

  color_chooser = g_object_new(GTK_TYPE_COLOR_SELECTION,
			       "has-palette", TRUE,
                               NULL);

  /* add an event handler for when user picks a color */
  g_signal_connect(color_chooser,
                   "color-changed", G_CALLBACK(print_color), NULL);

  gtk_box_pack_start_defaults(GTK_BOX(vbox), GTK_WIDGET(color_label));
  gtk_box_pack_start_defaults(GTK_BOX(vbox), GTK_WIDGET(color_chooser));

  gtk_box_pack_start_defaults(GTK_BOX(vbox),
                              g_object_new(GTK_TYPE_HSEPARATOR, NULL));

  /* create a font chooser and label, pack them into a VBox */
  font_label = g_object_new(GTK_TYPE_LABEL,
                            "label", "<b>Font chooser:</b>",
                            "use-markup", TRUE,
                            "xalign", 0.0,
                            NULL);

  font_chooser = g_object_new(GTK_TYPE_FONT_SELECTION, NULL);

  gtk_box_pack_start_defaults(GTK_BOX(vbox), GTK_WIDGET(font_label));
  gtk_box_pack_start_defaults(GTK_BOX(vbox), GTK_WIDGET(font_chooser));

  /* create a file browser button and label, pack them into a VBox */
  file_label = g_object_new(GTK_TYPE_LABEL,
                            "label", "<b>Click for file browser:</b>",
                            "use-markup", TRUE,
                            "xalign", 0.0,
                            NULL);

  /* use a stock label/image for the button */
  file_browser_button = g_object_new(GTK_TYPE_BUTTON,
                                     "use-stock", TRUE,
                                     "label", GTK_STOCK_OPEN,
                                     NULL);

  /* handler for creating file browser */
  g_signal_connect(file_browser_button,
                   "clicked", G_CALLBACK(make_file_dialog), NULL);

  gtk_box_pack_start_defaults(GTK_BOX(vbox), GTK_WIDGET(file_label));
  gtk_box_pack_start_defaults(GTK_BOX(vbox), GTK_WIDGET(file_browser_button));

  /* pack VBox into window, show everything, start event loop */
  gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(vbox));
  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_main();

  return 0;
}
