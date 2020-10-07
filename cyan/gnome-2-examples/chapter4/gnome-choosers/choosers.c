/* -*-coding: utf-8;-*- */
/* choosers.c -- demonstrate GNOME choosers */

#include <gnome.h>

#define NUM_WIDGETS 9

gchar *widget_label[NUM_WIDGETS] = {
  "_Text entry with history:",
  "_File chooser:",
  "Font _picker with default icon:",
  "Font picker _with font display:",
  "Font picker with custom _label:",
  "_Color picker:",
  "_Icon picker:",
  "Image picker and _viewer:",
  "_Date entry:" };

/* handler for GnomeEntry::activate */
void textentry_activated(GnomeEntry *text_widget, gpointer data)
{
  GtkEntry *gtk_entry;
  gchar *contents;

  g_object_get(text_widget, "gtk-entry", &gtk_entry, NULL);
  g_object_get(gtk_entry, "text", &contents, NULL);

  g_object_unref(gtk_entry);
  
  g_print("Text entry box activated; new contents: %s\n", contents);
  g_free(contents);
}

/* handler for GnomeFileEntry::activate */
void fileentry_activated(GnomeFileEntry *file_widget, gpointer data)
{
  gchar *path;

  path = gnome_file_entry_get_full_path(file_widget, FALSE);

  g_print("File chooser activated; new File: %s\n", path);
  g_free(path);
}

/* handler for GnomeFontPicker::font-set
   gets a picker id through the data pointer
   so that you know what picker changed */
void fontpicker_changed(GnomeFontPicker *picker, gchar *font_name, gpointer id)
{
  g_print("Font picker %d changed; new font: %s\n", (gint)id, font_name);
}

/* handler for GnomeColorPicker::color-set */
void colorpicker_changed(GnomeColorPicker *picker,
                         guint red, guint green, guint blue, guint alpha,
                         gpointer data)
{
  g_print("Color picker changed; new color: R=%d; G=%d; B=%d; A=%d\n",
          red, green, blue, alpha);
}

/* handler for GnomeIconEntry::changed */
void iconpicker_changed(GnomeIconEntry *picker, gpointer data)
{
  gchar *path;

  g_object_get(picker, "filename", &path, NULL);

  g_print("Icon picker changed; new icon: %s\n", path);
  g_free(path);
}

/* handler for GnomePixmapEntry::activate */
void pixmap_changed(GnomePixmapEntry *picker, gpointer data)
{
  gchar *path;

  g_print("Pixmap changed; ");

  path = gnome_pixmap_entry_get_filename(picker);
  if (path == NULL)
  {
     g_print("selection invalid.\n");
  } else {
     g_print("new image file: %s\n", path);
     g_free(path);
  }
}

/* handler for GnomeDateEdit::date-changed and GnomeDateEdit::time-changed
   If this is a date, is_date_ptr is TRUE */
void date_or_time_changed(GnomeDateEdit *widget, gpointer is_date_ptr)
{
  gboolean is_date = (gboolean)is_date_ptr;
  time_t time;
  gchar *date_or_time;

  date_or_time = is_date? "date" : "time";
  g_print("%s changed; ", date_or_time);

  g_object_get(widget, "time", (gulong*)(&time), NULL);
  g_print("new %s: %s", date_or_time, ctime(&time));
}

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
  GtkTable *table;
  GtkWidget *widgets[NUM_WIDGETS];
  gint i;

  program = gnome_program_init("choosers",
                               "0.1", LIBGNOMEUI_MODULE,
                               argc, argv,
                               GNOME_PROGRAM_STANDARD_PROPERTIES,
                               GNOME_PARAM_HUMAN_READABLE_NAME,
                               "GNOME Choosers Demo",
                               GNOME_PARAM_ENABLE_SOUND, TRUE,
                               NULL);

  window = g_object_new(GTK_TYPE_WINDOW,
                        "title", "GNOME Choosers",
                        "default-height", 300,
                        "default-width", 300,
                        "border-width", 12,
                        NULL);

  /* attach standard event handlers */
  g_signal_connect(window, "delete_event", G_CALLBACK(delete_event), NULL);
  g_signal_connect(window, "destroy", G_CALLBACK(end_program), NULL);

  /* create entry/chooser widgets and bind handlers */

  /* text widget with history */
  widgets[0] = g_object_new(GNOME_TYPE_ENTRY, "history-id", "textentry", NULL);

  g_signal_connect(widgets[0], "activate", G_CALLBACK(textentry_activated),
                   NULL);

  /* file chooser */
  widgets[1] = g_object_new(GNOME_TYPE_FILE_ENTRY, "history-id", "fileentry",
                            NULL);

  g_signal_connect(widgets[1], "activate", G_CALLBACK(fileentry_activated),
                   NULL);

  /* font picker with default font picker image */
  widgets[2] = g_object_new(GNOME_TYPE_FONT_PICKER,
                            "mode", GNOME_FONT_PICKER_MODE_PIXMAP,
                            NULL);

  /* font picker with selected font on label */
  widgets[3] = g_object_new(GNOME_TYPE_FONT_PICKER,
                            "mode", GNOME_FONT_PICKER_MODE_FONT_INFO,
                            "use-font-in-label", TRUE,
                            NULL);

  /* font picker with custom label */
  widgets[4] = g_object_new(GNOME_TYPE_FONT_PICKER,
                            "mode", GNOME_FONT_PICKER_MODE_USER_WIDGET,
                            NULL);

  gnome_font_picker_uw_set_widget(
     GNOME_FONT_PICKER(widgets[4]),
     GTK_WIDGET(g_object_new(GTK_TYPE_LABEL, "label", "My Text", NULL)));

  /* attach signal handlers for all three font pickers */
  for (i=2; i<=4; i++)
  {
     g_signal_connect(widgets[i],
          "font-set", G_CALLBACK(fontpicker_changed), (gpointer)(i-1));
  }

  /* color picker */
  widgets[5] = g_object_new(GNOME_TYPE_COLOR_PICKER,
                            "dither", TRUE,
                            "use-alpha", TRUE,
                            NULL);

  g_signal_connect(widgets[5], "color-set", G_CALLBACK(colorpicker_changed),
                   NULL);

  /* icon picker */
  widgets[6] = g_object_new(GNOME_TYPE_ICON_ENTRY,
                            "history-id", "iconpicker",
                            "browse-dialog-title", "Select icon",
                            NULL);

  g_signal_connect(widgets[6], "changed", G_CALLBACK(iconpicker_changed),
                   NULL);

  /* image picker/viewer */
  widgets[7] = gnome_pixmap_entry_new("pixmapentry", "Select image", TRUE);

  g_signal_connect(widgets[7], "activate", G_CALLBACK(pixmap_changed),
                   NULL);

  /* date/time entry: workday from 9 'til 5 */
  widgets[8] = g_object_new(GNOME_TYPE_DATE_EDIT,
                            "time", (gulong)time(NULL),
                            "lower-hour", 9,
                            "upper-hour", 17,
                            NULL);

  /* distinguish date and time to event handler with gboolean */
  g_signal_connect(widgets[8],
                   "date-changed", G_CALLBACK(date_or_time_changed),
                   (gpointer)TRUE);

  g_signal_connect(widgets[8],
                   "time-changed", G_CALLBACK(date_or_time_changed),
                   (gpointer)FALSE);


  /* create a two-column table for all of the widgets */
  table = g_object_new(GTK_TYPE_TABLE,
                       "n-rows", 9,
                       "n-columns", 2,
                       "column-spacing", 6,
                       "row-spacing", 6,
                       NULL);

  /* pack a widget with its label in each row */
  for (i = 0; i < NUM_WIDGETS; i++)
  {
     gtk_table_attach(table, g_object_new(GTK_TYPE_LABEL,
                                          "label", widget_label[i],
                                          "use-underline", TRUE,
                                          "mnemonic-widget", widgets[i],
                                          "xalign", 1.0,
                                          "justify", GTK_JUSTIFY_RIGHT,
                                          NULL),
                      0, 1, i, i+1, GTK_EXPAND|GTK_FILL, 0, 0, 0);

     gtk_table_attach(table, g_object_new(GTK_TYPE_ALIGNMENT,
                                          "xalign", 0.0,
                                          "child", widgets[i],
                                          NULL),
                      1, 2, i, i+1, GTK_EXPAND|GTK_FILL, 0, 0, 0);
  }

  /* pack table, show all widgets, and start GTK+ main event loop */
  gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(table));
  gtk_widget_show_all(GTK_WIDGET(window));

  gtk_main();

  return 0;
}
