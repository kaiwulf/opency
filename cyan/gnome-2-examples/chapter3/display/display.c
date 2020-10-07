/* -*-coding: utf-8;-*- */
/* display.c -- demonstrate display widgets */

#include <gtk/gtk.h>
#include <math.h>
const gdouble PROGRESS_STEP = 0.05;

/* standard handlers */
gint delete_event(GtkWidget *widget, GdkEvent event, gpointer data)
{
  return FALSE;
}

void end_program(GtkWidget *widget, gpointer data)
{
  gtk_main_quit();
}

/* bump the display counter on the given
   GtkProgressBar one PROGRESS_STEP further */
void advance(GtkButton *button, gpointer progress_ptr)
{
  GtkProgressBar *progress_bar = GTK_PROGRESS_BAR(progress_ptr);
  gdouble current;
  gchar *embedded_text;

  g_object_get(progress_bar, "fraction", &current, NULL);
  current += PROGRESS_STEP;
  current = CLAMP(current, 0.0, 0.999);
  embedded_text = g_strdup_printf("%3.0f%% completed", current*100);
  g_object_set(progress_bar,
               "fraction", current,
               "text", embedded_text,
                NULL);
  g_free(embedded_text);
}

int main(int argc, char **argv)
{
  GtkWindow *window;
  GtkVBox *vbox;
  GtkLabel *label, *label_markup;
  GtkImage *image;
  GtkHBox *progress_bar_box;
  GtkProgressBar *progress_bar;
  GtkButton *pulse_button, *advance_button;

  /* initialize GTK+, create a window */
  gtk_init(&argc, &argv);
  window = g_object_new(GTK_TYPE_WINDOW,
                        "title", "Display Widgets",
                        "border-width", 12,
                        NULL);

  /* attach standard window event handlers */
  g_signal_connect(window, "delete_event", G_CALLBACK(delete_event), NULL);
  g_signal_connect(window, "destroy", G_CALLBACK(end_program), NULL);

  /* create a vertical box and pack it into window  */
  vbox = g_object_new(GTK_TYPE_VBOX, "spacing", 6, NULL);

  gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(vbox));

  /* a new label. */
  label = g_object_new(GTK_TYPE_LABEL,
	               "label", "When lilacs last in the door-yard bloom'd.\n \
Fonts normally do not support all markup attributes:",
                       NULL);

  gtk_box_pack_start_defaults(GTK_BOX(vbox), GTK_WIDGET(label));

  /* a label with extensive markup */
  label_markup = g_object_new(GTK_TYPE_LABEL,
                              "wrap", TRUE,
                              "use-markup", TRUE,
                              "label", "\
<span font_desc=\"Courier New Bold 16\">Courier - </span>\
<span font_desc=\"Bitstream Charter Bold 16\">Charter - </span>\
<span font_family=\"Times\">Times - </span>\
<span face=\"Verdana\">Verdana - </span>\
<span size=\"xx-small\">Tiny - </span>\
<span size=\"x-small\">Extra Small - </span>\
<span size=\"small\">Small - </span>\
<span size=\"medium\">Medium - </span>\
<span size=\"large\">Large - </span>\
<span size=\"x-large\">Extra Large - </span>\
<span size=\"xx-large\">Huge - </span>\
<span size=\"36000\">Gigantic - </span>\
<span face=\"Times\">\
<span style=\"oblique\">Oblique - </span>\
<span style=\"italic\">Italic - </span>\
</span>\
<span weight=\"ultrabold\">Ultra Bold - </span>\
<span face=\"Times New Roman\" variant=\"smallcaps\">\
Caps and Small Caps - </span>\
<span stretch=\"condensed\">Condensed - </span>\
H<span rise=\"-5000\">2</span>O - \
<span background=\"black\" foreground=\"#FFFFFF\">Inverse</span> - \
<span underline=\"double\">Important - </span>\
<span strikethrough=\"true\">Strikethrough</span>\
\n\
The <tt>span</tt> tag appears to function \
well. However, it's <i>easier</i> to use \
<b>short forms</b><sup>*</sup>.\n\
\n\
<small><sup>*)</sup> short tags that work just\
 the same as <tt>span</tt> with corresponding \
attributes</small>");

  gtk_box_pack_start_defaults(GTK_BOX(vbox), GTK_WIDGET(label_markup));

  /* a GtkImage */
  image = g_object_new(GTK_TYPE_IMAGE, "file", "apple-green.png", NULL);
  gtk_box_pack_start_defaults(GTK_BOX(vbox), GTK_WIDGET(image));

  /* a progress bar with buttons to advance the progress or pulse activity */
  progress_bar_box = g_object_new(GTK_TYPE_HBOX, "spacing", 6, NULL);
  gtk_box_pack_start_defaults(GTK_BOX(vbox), GTK_WIDGET(progress_bar_box));
  
  advance_button = g_object_new(GTK_TYPE_BUTTON,
                                "label", "_Advance",
                                "use-underline", TRUE,
                                NULL);

  pulse_button = g_object_new(GTK_TYPE_BUTTON,
                              "label", "_Pulse",
                              "use-underline", TRUE,
                              NULL);

  gtk_box_pack_start_defaults(GTK_BOX(progress_bar_box),
                              GTK_WIDGET(advance_button));
  gtk_box_pack_start_defaults(GTK_BOX(progress_bar_box),
                              GTK_WIDGET(pulse_button));

  progress_bar = g_object_new(GTK_TYPE_PROGRESS_BAR, NULL);
  gtk_box_pack_start_defaults(GTK_BOX(progress_bar_box),
                              GTK_WIDGET(progress_bar));

  g_signal_connect(advance_button,
                   "clicked", G_CALLBACK(advance), progress_bar);

  /* attach a builtin GTK+ handler to pulse the progress bar */
  g_signal_connect_swapped(pulse_button,
     "clicked", G_CALLBACK(gtk_progress_bar_pulse), progress_bar);

  /* show window and begin GTK+ main loop */
  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_main();
  return 0;
}
