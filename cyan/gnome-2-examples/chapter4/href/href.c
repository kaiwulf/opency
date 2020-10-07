/* -*-coding: utf-8;-*- */
/* href.c -- demonstrate GNOME hyperlinks */

#include <gnome.h>

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
  GnomeProgram *program;
  GtkWindow *window;
  GtkVBox *vbox;
  GnomeHRef *link[5];
  gint i;
  
  /* initialize application */
  program = gnome_program_init("href", "0.1", LIBGNOMEUI_MODULE,
                               argc, argv,
                               GNOME_PROGRAM_STANDARD_PROPERTIES,
                               GNOME_PARAM_HUMAN_READABLE_NAME, "HRef",
                               GNOME_PARAM_ENABLE_SOUND, TRUE,
                               NULL);

  /* create main window */
  window = g_object_new(GTK_TYPE_WINDOW,
                        "title", "GNOME Links",
                        "border-width", 12,
                        NULL);

  /* attach standard event handlers */
  g_signal_connect(window, "delete_event", G_CALLBACK(delete_event), NULL);
  g_signal_connect(window, "destroy", G_CALLBACK(end_program), NULL);

  /* create five hyperlink objects */
  link[0] = g_object_new(GNOME_TYPE_HREF,
                         "url", "http://www.gnome.org",
                         "text", "www.gnome.org",
                         NULL);

  link[1] = g_object_new(GNOME_TYPE_HREF,
                         "url", "http://news.gnome.org/gnome-news/",
                         "text", "Gnotices",
                         NULL);

  link[2] = g_object_new(GNOME_TYPE_HREF,
                         "url", "http://www.gnomedesktop.org",
                         "text", "FootNotes",
                         NULL);

  link[3] = g_object_new(GNOME_TYPE_HREF,
                         "url", "http://download.gnome.org",
                         "text", "download.gnome.org",
                         NULL);

  link[4] = g_object_new(GNOME_TYPE_HREF,
                         "url", "http://developer.gnome.org",
                         "text", "developer.gnome.org",
                         NULL);

  /* pack all of these into a VBox */
  vbox = g_object_new(GTK_TYPE_VBOX, "spacing", 6, NULL);
  for (i=0; i<5; i++)
  {
     gtk_box_pack_start_defaults(GTK_BOX(vbox), GTK_WIDGET(link[i]));
  }

  /* pack VBox, show everything, start GTK+ main event loop */
  gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(vbox));
  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_main();

  return 0;
}

