/* -*-coding: utf-8;-*- */
/* dialog.c -- demonstrate a dialog box */

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

/* handler for "response" signal from dialog */
void dialog_action(GtkDialog *dialog, gint response, gpointer data)
{
  g_assert(response == GTK_RESPONSE_ACCEPT || response == GTK_RESPONSE_REJECT);

  switch (response)
  {
     case GTK_RESPONSE_ACCEPT:
        g_print("Planet destroyed.\n");
        break;
     case GTK_RESPONSE_REJECT:
        g_print("Planet destruction aborted.\n");
        break;
     default:
        /* do nothing */
        break;
  }
}

int main(int argc, char **argv)
{
  GtkWindow *window;
  GtkButton *button;
  GtkDialog *dialog;
  GtkHBox *dialog_hbox;
  GtkImage *dialog_icon;
  GtkLabel *dialog_text;

  /* initialize GTK, create window */
  gtk_init(&argc, &argv);
 
  window = g_object_new(GTK_TYPE_WINDOW,
                        "default_height", 200,
                        "default_width", 200,
                        "border-width", 12,
                        "title", "Dialog Demo",
                         NULL);

  /* attach standard window handlers */
  g_signal_connect(window, "delete_event", G_CALLBACK(delete_event), NULL);
  g_signal_connect(window, "destroy", G_CALLBACK(end_program), NULL);

  /* create dialog box */
  dialog = GTK_DIALOG(gtk_dialog_new_with_buttons(
                         "Destroy Planet?",                /* title */
                         window,                           /* parent */
                         GTK_DIALOG_DESTROY_WITH_PARENT,   /* options */
                         /* list of button labels and responses */
                         "_Destroy",             GTK_RESPONSE_ACCEPT,
                         "_Abort Destruction",   GTK_RESPONSE_REJECT,
                         NULL));

  gtk_dialog_set_default_response(dialog, GTK_RESPONSE_REJECT);

  /* attach handler for dialog response */
  g_signal_connect(dialog, "response", G_CALLBACK(dialog_action), NULL);

  /* remove dialog box when it returns a response */
  g_signal_connect_swapped(dialog,
                           "response", G_CALLBACK(gtk_widget_destroy), window);

  /* fill dialog window: create HBox packed with icon and Text */
  dialog_hbox = g_object_new(GTK_TYPE_HBOX, "border-width", 8, NULL);

  dialog_icon = g_object_new(GTK_TYPE_IMAGE,
                             "stock", GTK_STOCK_DIALOG_WARNING,
                             "icon-size", GTK_ICON_SIZE_DIALOG,
                             "xalign", 0.5,
                             "yalign", 1.0,
                             NULL);

  gtk_box_pack_start(GTK_BOX(dialog_hbox), GTK_WIDGET(dialog_icon),
                     FALSE, FALSE, 0);

  dialog_text = g_object_new(GTK_TYPE_LABEL,
                             "wrap", TRUE,
                             "use-markup", TRUE,
                             "label", "\
<big><b>Do you really want to destroy the planet?</b></big>\
\n\
Please note that the annihilation of a planet is rarely \
regarded as a successful show of environmentally \
friendly behavior.");

  gtk_box_pack_start(GTK_BOX(dialog_hbox), GTK_WIDGET(dialog_text),
                     TRUE, TRUE, 0);

  /* pack HBox into dialog */
  gtk_box_pack_start(GTK_BOX(dialog->vbox), GTK_WIDGET(dialog_hbox),
                     FALSE, FALSE, 0);


  /* create button for main window */
  button = g_object_new(GTK_TYPE_BUTTON,
                        "label", "_Show dialog",
                        "use-underline", TRUE,
                        NULL);

  /* show the dialog when user clicks button */
  g_signal_connect_swapped(button,
                           "clicked", G_CALLBACK(gtk_widget_show_all), dialog);

  /* pack button, show everything, start main loop */
  gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(button));
  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_main();

  return 0;
}
