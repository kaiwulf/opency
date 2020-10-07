/* -*-coding: utf-8;-*- */
/* high-scores.c -- demonstrate GNOME top ten dialog */

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

gint rank = 0;

/* put a score into the high scores table */
void enter_score(GtkWidget *widget, gpointer data)
{
  gchar *text;

  g_object_get(widget, "text", &text, NULL);
  rank = gnome_score_log(g_ascii_strtod(text, NULL), "Insane", FALSE);

  g_print("That was score number %d.\n", rank);
  g_free(text);
}

/* show the high scores */
void show_scores(GtkWidget *widget, gpointer prog_ptr)
{
  char *picfile;
  GnomeProgram *program;
  GnomeScores *scores;

  program = (GnomeProgram *)prog_ptr;
  picfile = gnome_program_locate_file(program, GNOME_FILE_DOMAIN_PIXMAP,
				      "apple-green.png", TRUE, NULL);

  scores = (GnomeScores *)
     gnome_scores_display_with_pixmap(picfile, "high-scores-demo-program",
                                     "Insane", rank);
  g_object_set(scores, "title", "High Scores: Insane", NULL);
  g_free(picfile);
}

int main(int argc, char **argv)
{
  GnomeProgram *program;
  GtkWindow *window;
  GtkVBox *hbox;
  GtkLabel *label;
  GtkEntry *entry;
  GtkButton *button;
  
  /* initialize application */
  program = gnome_program_init("high-scores", "0.1", LIBGNOMEUI_MODULE,
                               argc, argv,
                               GNOME_PROGRAM_STANDARD_PROPERTIES,
                               GNOME_PARAM_HUMAN_READABLE_NAME, "HighScores",
                               GNOME_PARAM_ENABLE_SOUND, TRUE,
                               NULL);

  /* create main window */
  window = g_object_new(GTK_TYPE_WINDOW,
                        "title", "GNOME High Scores",
                        "border-width", 12,
                        NULL);

  /* initialize high scores */
  gnome_score_init("high-scores-demo-program");

  /* attach standard event handlers */
  g_signal_connect(window, "delete_event", G_CALLBACK(delete_event), NULL);
  g_signal_connect(window, "destroy", G_CALLBACK(end_program), NULL);

  /* an entry box to enter a score */
  label = g_object_new(GTK_TYPE_LABEL, "label", "Enter a score:", NULL);
  entry = g_object_new(GTK_TYPE_ENTRY, NULL);
  g_signal_connect(entry, "activate", G_CALLBACK(enter_score), NULL);

  /* a button to show high scores */
  button = g_object_new(GTK_TYPE_BUTTON, "label", "Show High Scores", NULL);
  g_signal_connect(button,
		   "clicked", G_CALLBACK(show_scores), (gpointer)program);
		       
  /* pack all of that into an hbox */
  hbox = g_object_new(GTK_TYPE_HBOX, "spacing", 6, NULL);
  gtk_box_pack_start_defaults(GTK_BOX(hbox), GTK_WIDGET(label));
  gtk_box_pack_start_defaults(GTK_BOX(hbox), GTK_WIDGET(entry));
  gtk_box_pack_start_defaults(GTK_BOX(hbox), GTK_WIDGET(button));

  /* pack HBox, show everything, start GTK+ main event loop */
  gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(hbox));
  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_main();

  return 0;
}

