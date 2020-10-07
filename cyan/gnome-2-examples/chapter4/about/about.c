/* -*-coding: utf-8;-*- */
/* about.c -- demonstrate gnome "about" windows */

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
  GnomeAbout *info;
  gchar *translator;
  gchar *apple;
  GdkPixbuf *logo;

  const gchar *programmers[] = {
     "George Ricshaw",
     "Edna Kolakowski",
     "Merideth Gainer",
     NULL
  };

  const gchar *documenters[] = {
     "Walter Zimmerman",
     "Harold Fenner",
     "Heather Kunkle",
     NULL
  };

  /* Initialize GNOME */
  program = gnome_program_init(
               "about", "3.0",
               LIBGNOMEUI_MODULE,
               argc, argv,
               GNOME_PROGRAM_STANDARD_PROPERTIES,
               GNOME_PARAM_HUMAN_READABLE_NAME, "AboutDemo(GrannySmith)",
               GNOME_PARAM_ENABLE_SOUND, TRUE,
               NULL);

  /* determine if the program is running in a translated environment;
     if no, set translator to NULL */
  if (_("Translator") == "Translator")
  {
     translator = NULL;
  } else {
     translator = g_strdup(_("Put your translator here."));
  }

  /* find the green apple image; it should be in the standard
     GNOME pixmap directory */
  apple = gnome_program_locate_file(program,
                                    GNOME_FILE_DOMAIN_PIXMAP,
                                    "apple-green.png",
                                    TRUE,
                                    NULL);

  /* allocate logo pixmap */
  logo = gdk_pixbuf_new_from_file(apple, NULL);
  g_free(apple);
	
  /* create "about" window */
  info = GNOME_ABOUT(
            gnome_about_new("GrannySmith", "3.0",
                            "(C) 2003 Rixico Inc.",
                            "The Malus domestica Borkh Solutions People",
                            programmers, documenters, translator, logo));

  /* because this data was copied, it should be freed */
  g_free(translator);
  g_object_unref(logo);

  /* attach standard event handlers to info window */
  g_signal_connect(info, "delete-event", G_CALLBACK(delete_event), NULL);
  g_signal_connect(info, "destroy", G_CALLBACK(end_program), NULL);

  /* show widget, start GTK+ main loop */
  gtk_widget_show(GTK_WIDGET(info));
  gtk_main();

  return 0;
}
