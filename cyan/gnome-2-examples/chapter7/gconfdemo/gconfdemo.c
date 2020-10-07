/* -*-coding: utf-8;-*- */
/* gconfdemo.c -- GConf demo application */

#include <gnome.h>
#include <gconf/gconf-client.h>

GnomeProgram *program;
GConfClient *client;
GtkLabel *label[3];
GnomeApp *window;

/* the relative path of the key */
#define WORDS_KEY "show_nums_as_words"

/* standard window event handlers */
gint delete_event(GtkWidget *widget, GdkEvent event, gpointer data)
{
  return FALSE;
}

void end_program(GtkWidget *widget, gpointer data)
{
  gtk_main_quit();
}

void button_changed(GtkCheckButton *button, gpointer key_ptr)
{
  gboolean value;
  gchar *path;
  gchar *key = (gchar *)key_ptr;

  g_object_get(button, "active", &value, NULL);

  path	= gnome_gconf_get_app_settings_relative(program, key),
  gconf_client_set_bool(client, path, value, NULL);
  g_free(path);
}

void preferences(void)
{
  static GtkDialog *dialog;
  static GtkCheckButton *word_button;
  gboolean show_words_init;
  gboolean words_key_writable;
  GError *error = NULL;
  gchar *path;

  dialog = GTK_DIALOG(gtk_dialog_new_with_buttons(
                      "GConf Demo Preferences",
                      GTK_WINDOW(window),
                      GTK_DIALOG_DESTROY_WITH_PARENT,
                      GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
                      NULL));
  g_object_set(dialog, "border-width", 12, NULL);
  gtk_dialog_set_default_response(dialog, GTK_RESPONSE_ACCEPT);
  g_signal_connect_swapped(dialog,
                           "response", G_CALLBACK(gtk_widget_destroy), dialog);

  word_button = g_object_new(GTK_TYPE_CHECK_BUTTON,
                            "label", "Display numbers as _words",
                            "use-underline", TRUE,
                            NULL);

  path = gnome_gconf_get_app_settings_relative(program, WORDS_KEY);

  show_words_init = gconf_client_get_bool(client, path, &error);

  if (error)
  {
     show_words_init = TRUE;
     g_error_free(error);
  }

  words_key_writable = gconf_client_key_is_writable(client, path, NULL);

  g_free(path);

  g_object_set(word_button,
               "active", show_words_init,
               "sensitive", words_key_writable,
               NULL);

  g_signal_connect(word_button,
                   "toggled", G_CALLBACK(button_changed), WORDS_KEY);

  gtk_box_pack_start_defaults(GTK_BOX(dialog->vbox), GTK_WIDGET(word_button));

  gtk_widget_show_all(GTK_WIDGET(dialog));
}

void about(void)
{
  static GnomeAbout *about_window;

  const gchar *authors[] = {
     "Matthias Warkus",
     "Brian Ward",
     NULL
  };

  const gchar *translator = "Brian Ward";

  about_window =
     GNOME_ABOUT(gnome_about_new("GConf Demo", "0.1",
                                 "(C) 2002, 2003 Matthias Warkus",
                                 "A GConf Demonstration Program",
                                 authors, NULL, translator,
                                 NULL));

  g_signal_connect_swapped(about_window,
     "delete-event", G_CALLBACK(gtk_widget_destroy), about_window);

  gtk_widget_show_all(GTK_WIDGET(about_window));
}

GnomeUIInfo file_items[] = {
  GNOMEUIINFO_MENU_QUIT_ITEM(end_program, NULL),
  GNOMEUIINFO_END
};

GnomeUIInfo edit_items[] = {
  GNOMEUIINFO_MENU_PREFERENCES_ITEM(preferences, NULL),
  GNOMEUIINFO_END
};

GnomeUIInfo help_items[] = {
  GNOMEUIINFO_MENU_ABOUT_ITEM(about, NULL),
  GNOMEUIINFO_END
};

GnomeUIInfo menu_bar_items[] = {
  GNOMEUIINFO_MENU_FILE_TREE(file_items),
  GNOMEUIINFO_MENU_EDIT_TREE(edit_items),
  GNOMEUIINFO_MENU_HELP_TREE(help_items),
  GNOMEUIINFO_END
};

void set_labels(void)
{
  gboolean show_as_words;
  GError *error = NULL;
  gchar *path;

  path = gnome_gconf_get_app_settings_relative(program, WORDS_KEY);
  show_as_words = gconf_client_get_bool(client, path, &error);
  g_free(path);

  if (error)
  {
     show_as_words = TRUE;
     g_error_free(error);
  }

  if (show_as_words)
  {
     g_object_set(label[0], "label", "One", NULL);
     g_object_set(label[1], "label", "Two", NULL);
     g_object_set(label[2], "label", "Forty-two", NULL);
  } else {
     g_object_set(label[0], "label", "1", NULL);
     g_object_set(label[1], "label", "2", NULL);
     g_object_set(label[2], "label", "42", NULL);
  }
}

void display_type_changed(GConfClient *client, guint cxnid,
                          GConfEntry *entry, gpointer data)
{
  set_labels();
}

int main(int argc, char **argv)
{
  gchar *gconf_path;
  GtkStatusbar *status;
  GtkVBox *vbox;
  gint i;
  guint words_cxnid;
  gchar *path;

  program = gnome_program_init("gconfdemo", "0.1",
                               LIBGNOMEUI_MODULE,
                               argc, argv,
                               GNOME_PROGRAM_STANDARD_PROPERTIES,
                               GNOME_PARAM_HUMAN_READABLE_NAME, "GConf Demo",
                               GNOME_PARAM_ENABLE_SOUND, TRUE,
                               NULL);

  /* create GConf client object */
  client = gconf_client_get_default();
  gconf_client_set_error_handling(client, GCONF_CLIENT_HANDLE_UNRETURNED);

  /* add this application's recommended path */
  gconf_path = gnome_gconf_get_app_settings_relative(program, "");
  gconf_client_add_dir(client, gconf_path, GCONF_CLIENT_PRELOAD_ONELEVEL, NULL);

  /* create window */
  window = g_object_new(GNOME_TYPE_APP,
                         "title", "GConf Demo",
                         "app-id", "gconfdemo",
                         "default-width", 300,
                         "default-height", 300,
                          NULL);

  /* attach standard signal handlers */
  g_signal_connect(window, "delete_event", G_CALLBACK(delete_event), NULL);
  g_signal_connect(window, "destroy", G_CALLBACK(end_program), NULL);

  /* add menu and status bar */
  gnome_app_create_menus(window, menu_bar_items);
  status = g_object_new(GTK_TYPE_STATUSBAR, NULL);
  gnome_app_set_statusbar(window, GTK_WIDGET(status));

  /* create some labels for the main window */
  vbox = g_object_new(GTK_TYPE_VBOX, "border-width", 12, NULL);
  for (i=0; i<3; i++)
  {
     label[i] = g_object_new(GTK_TYPE_LABEL, "xalign", 0.0, NULL);
     gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(label[i]), FALSE, FALSE, 0);
  }
  /* set the label text */
  set_labels();

  /* add label vbox to main window */
  gnome_app_set_contents(window, GTK_WIDGET(vbox));

  /* attach callback functions for when value behind WORDS_KEY changes */
  path = gnome_gconf_get_app_settings_relative(program, WORDS_KEY),
  words_cxnid = gconf_client_notify_add(client, path, display_type_changed,
                                        NULL, NULL, NULL);
  g_free(path);

  /* show window, start main event loop */
  gtk_widget_show_all(GTK_WIDGET(window));

  gtk_main();

  /* remove callback functions */
  gconf_client_notify_remove(client, words_cxnid);

  /* remove path */
  gconf_client_remove_dir(client, gconf_path, NULL);
  g_free(gconf_path);

  /* let go of reference to GConf client object */
  g_object_unref(client);

  return 0;
}
