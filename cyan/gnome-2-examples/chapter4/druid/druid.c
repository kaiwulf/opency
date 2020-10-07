/* -*-coding: utf-8;-*- */
/* druid.c -- sample GNOME druid */

#include <gnome.h>

GtkEntry *last_name_entry;
GtkEntry *first_name_entry;
GtkWindow *window;

gchar *flavors[] = {
  "Vanilla", "Chocolate", "Strawberry", "Pistachio", "Mustard onion"
};
#define NUM_FLAVORS 5
#define NUM_MUSTARD NUM_FLAVORS - 1
GtkRadioButton *ice_cream[NUM_FLAVORS];

/* standard handler to terminate event loop */
void end_program(GtkWidget *widget, gpointer data)
{
  gtk_main_quit();
}

/* warning dialog using GNOME guidelines, as in dialog.c */
void warn_dialog(gchar *message)
{
  gchar *markup;
  GtkDialog *dialog;
  GtkHBox *hbox;
  GtkImage *icon;
  GtkLabel *text;

  markup = g_strdup_printf("<big><b>Druid configuration</b></big>\n\n%s",
			   message);

  dialog = GTK_DIALOG(gtk_dialog_new_with_buttons(
                      "Warning", window, GTK_DIALOG_MODAL,
                      "OK", GTK_RESPONSE_OK, NULL));

  gtk_dialog_set_default_response(dialog, GTK_RESPONSE_OK);
  g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_widget_destroy),
                           GTK_WIDGET(dialog));

  hbox = g_object_new(GTK_TYPE_HBOX, "border-width", 8, NULL);
  icon = g_object_new(GTK_TYPE_IMAGE,
		      "stock", GTK_STOCK_DIALOG_WARNING,
                      "icon-size", GTK_ICON_SIZE_DIALOG,
                      "xalign", 0.5, "yalign", 0.5, NULL);

  gtk_box_pack_start(GTK_BOX(hbox), GTK_WIDGET(icon), FALSE, FALSE, 0);
  text = g_object_new(GTK_TYPE_LABEL,
                      "wrap", TRUE,
                      "label", markup,
                      "use-markup", TRUE, NULL);
  g_free(markup);

  gtk_box_pack_start(GTK_BOX(hbox), GTK_WIDGET(text), TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(dialog->vbox), GTK_WIDGET(hbox),
                     FALSE, FALSE, 0);
  gtk_widget_show_all(GTK_WIDGET(dialog));
}

/* verify the first and last name (page 1) */
gboolean check_page1(GnomeDruidPage *page, GtkWidget *druid, gpointer data)
{
  gchar *last_name = NULL;
  gchar *first_name = NULL;
  gboolean return_val;

  g_object_get(last_name_entry, "text", &last_name, NULL);
  g_object_get(first_name_entry, "text", &first_name, NULL);
  if ((!*last_name) || (!*first_name))
  {
     warn_dialog("You must supply your first and last names.");
     return_val = TRUE;
  } else {
     return_val = FALSE;
  }

  g_free(last_name);
  g_free(first_name);
  return return_val;
}  

/* check the favorite ice cream flavor (page 2) */
/* if someone picks "mustard onion" as their favorite ice cream
   flavor, show a warning, but only once. If the user insists, they
   get it the second time. */
gboolean check_page2(GnomeDruidPage *page, GtkWidget *druid, gpointer data)
{
  static gboolean already_warned = FALSE;
  gboolean mustard_onion;

  if (!already_warned)
  {
     g_object_get(ice_cream[NUM_MUSTARD], "active", &mustard_onion, NULL);
     if (mustard_onion)
     {
        warn_dialog("Do you really want mustard onion? If you're\
 sure about this, click Forward again.");
        already_warned = TRUE;
        return TRUE;
     }
  }
  return FALSE;
}

/* signal handler to finish configuration */
void finish_config(GnomeDruidPage *page, GtkWidget *druid, gpointer data)
{
  gboolean active;
  gchar *first, *last;
  gint i;

  for (i = 0; i < NUM_FLAVORS; i++)
  {
     g_object_get(ice_cream[i], "active", &active, NULL);
     if (active)
     {
        break;
     }
  }
  g_object_get(first_name_entry, "text", &first, NULL);
  g_object_get(last_name_entry, "text", &last, NULL);

  g_print("Druid finished. Configuration:\n");
  g_print("First name: %s\nLast name: %s\nFavorite flavor: %s\n",
	  first, last, flavors[i]);
  g_free(first);   g_free(last);
  /* end the program */
  g_signal_emit_by_name(window, "destroy", NULL);
}

int main(int argc, char **argv)
{
  GnomeProgram *program;
  GdkColor color;
  GnomeDruid *druid;
  GdkPixbuf *logo, *watermark, *watermark_top;
  GnomeDruidPageEdge *title_page, *finish_page;
  GnomeDruidPageStandard *page_1, *page_2;
  GtkVBox *ice_vbox;
  gint i;

  /* initialize GNOME */
  program = gnome_program_init("druid", "0.1",
                               LIBGNOMEUI_MODULE,
                               argc, argv,
                               GNOME_PARAM_HUMAN_READABLE_NAME, "Druid",
                               NULL);

  /* load images */
  logo = gdk_pixbuf_new_from_file("settings.png", NULL);
  watermark = gdk_pixbuf_new_from_file("watermark.png", NULL);
  watermark_top = gdk_pixbuf_new_from_file( "watermark-top.png", NULL);

  /* create title page */
  title_page = GNOME_DRUID_PAGE_EDGE(
     gnome_druid_page_edge_new_with_vals(
        GNOME_EDGE_START, TRUE,
        "Welcome to the Sample Druid!",
        "This is the explanation on the title page.\n\
Like other druid pages, this page has a title and images: \n\
 - A logo in the upper right corner,\n\
 - A watermark at the top (empty in this druid), and\n\
 - A watermark along the left edge.",
        logo, watermark, watermark_top));

  /* title page uses black text on a white background */
  gdk_color_parse("white", &color);
  gnome_druid_page_edge_set_bg_color(title_page, &color);
  gdk_color_parse("black", &color);
  gnome_druid_page_edge_set_title_color(title_page, &color);

  /* create page 1 (the one after the title) */
  page_1 = g_object_new(GNOME_TYPE_DRUID_PAGE_STANDARD,
                        "title", "Your Name",
                        "logo", logo,
                        "top-watermark", watermark_top,
                         NULL);

  /* fill page 1 main content */
  first_name_entry = g_object_new(GTK_TYPE_ENTRY, NULL);

  gnome_druid_page_standard_append_item(
     page_1,
     "This is page 1.\nThere is no image to the left, but \
the bar at the top remains.\n\n
What is your first name?",
     GTK_WIDGET(first_name_entry),
     "Your parents probably supplied your first name. Ask them \
if you can't remember what it is.");

  last_name_entry = g_object_new(GTK_TYPE_ENTRY, NULL);
  gnome_druid_page_standard_append_item(
     page_1,
     "What is your last name?",
     GTK_WIDGET(last_name_entry),
     "If you don't know, open a telephone book and pick one that looks nice.");

  /* attach handler for verifying the information on this page */
  g_signal_connect(page_1, "next", G_CALLBACK(check_page1), NULL);

  /* create page 2 */
  page_2 = g_object_new(GNOME_TYPE_DRUID_PAGE_STANDARD,
                        "title", "Favorite Ice Cream",
                        "logo", logo,
                        "top-watermark", watermark_top,
                        NULL);  

  /* fill second page */
  ice_vbox = g_object_new(GTK_TYPE_VBOX, NULL);
  for (i = 0; i < NUM_FLAVORS; i++)              /* ice cream widgets */
  {
     ice_cream[i] = g_object_new(GTK_TYPE_RADIO_BUTTON, NULL);
     gtk_box_pack_start_defaults(GTK_BOX(ice_vbox), GTK_WIDGET(ice_cream[i]));
     g_object_set(ice_cream[i], "label", flavors[i], NULL);
     if (i != 0)
     {
        g_object_set(ice_cream[i], "group", ice_cream[0], NULL);
     }
  }
  
  gnome_druid_page_standard_append_item(
     page_2,
     "Choose your favorite ice cream flavor.",
     GTK_WIDGET(ice_vbox),
     "Please note that mustard onion is an acquired taste.");

  /* attach signal to check the page input */
  g_signal_connect(page_2, "next", G_CALLBACK(check_page2), NULL);

  /* create final page */
  finish_page = GNOME_DRUID_PAGE_EDGE(
     gnome_druid_page_edge_new_with_vals(
        GNOME_EDGE_FINISH,
        TRUE,
        "Configuration Finished",
        "Thanks for using this sample druid!",
        logo, watermark, watermark_top));

  /* text and title black on white */
  gdk_color_parse("white", &color);
  gnome_druid_page_edge_set_bg_color(finish_page, &color);
  gnome_druid_page_edge_set_textbox_color(finish_page, &color);
  gdk_color_parse("black", &color);
  gnome_druid_page_edge_set_text_color(finish_page, &color);
  gnome_druid_page_edge_set_title_color(finish_page, &color);
  
  /* connect signal for the finish button */
  g_signal_connect(finish_page, "finish", G_CALLBACK(finish_config), NULL);

  /* create main druid widget */
  druid = GNOME_DRUID(gnome_druid_new_with_window(
             "Druid Demonstration", NULL, TRUE, (GtkWidget**)(&window)));  

  /* add all of the pages to the druid and show the widget */
  gnome_druid_append_page(druid, GNOME_DRUID_PAGE(title_page));
  gtk_widget_show_all(GTK_WIDGET(title_page));
  gnome_druid_append_page(druid, GNOME_DRUID_PAGE(page_1));
  gtk_widget_show_all(GTK_WIDGET(page_1));
  gnome_druid_append_page(druid, GNOME_DRUID_PAGE(page_2));
  gtk_widget_show_all(GTK_WIDGET(page_2));
  gnome_druid_append_page(druid, GNOME_DRUID_PAGE(finish_page));
  gtk_widget_show_all(GTK_WIDGET(finish_page));

  /* attach destroy handler (end program when druid cancels) */
  g_signal_connect(druid, "destroy", G_CALLBACK(end_program), NULL);

  /* no longer need references to these images */
  g_object_unref(logo);
  g_object_unref(watermark);
  g_object_unref(watermark_top);

  /* start GTK+ main event loop */
  gtk_main();

  return 0;
}
