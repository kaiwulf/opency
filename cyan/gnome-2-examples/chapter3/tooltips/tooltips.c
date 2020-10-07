/* -*-coding: utf-8;-*- */
/* tooltips.c -- demonstrate tooltips */

#include <gtk/gtk.h>

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
  GtkWindow *window;
  GtkButton *cut, *copy, *paste, *other_button;
  GtkVButtonBox *box;
  GtkTooltips *tips, *other_tips;

  /* start GTK+, create window */
  gtk_init(&argc, &argv);

  window = g_object_new(GTK_TYPE_WINDOW,
                        "title", "Tooltips Demonstration",
                        "default-width", 300,
                        "default-height", 100,
                        "border-width", 12,
                         NULL);

  /* attach standard event handlers */
  g_signal_connect(window, "delete_event", G_CALLBACK(delete_event), NULL);
  g_signal_connect(window, "destroy", G_CALLBACK(end_program), NULL);

  /* create three buttons using stock labels and images */
  cut = g_object_new(GTK_TYPE_BUTTON,
                     "use-stock", TRUE,
                     "label", GTK_STOCK_CUT,
                     NULL);
  copy = g_object_new(GTK_TYPE_BUTTON,
                      "use-stock", TRUE,
                      "label", GTK_STOCK_COPY,
                      NULL);
  paste = g_object_new(GTK_TYPE_BUTTON,
                      "use-stock", TRUE,
                      "label", GTK_STOCK_PASTE,
                      NULL);

  /* pack buttons into VButtonBox */
  box = g_object_new(GTK_TYPE_VBUTTON_BOX, NULL);

  gtk_box_pack_end_defaults(GTK_BOX(box), GTK_WIDGET(cut));
  gtk_box_pack_end_defaults(GTK_BOX(box), GTK_WIDGET(copy));
  gtk_box_pack_end_defaults(GTK_BOX(box), GTK_WIDGET(paste));

  /* create tooltips object, assign to the buttons above */
  tips = g_object_new(GTK_TYPE_TOOLTIPS, NULL);

  gtk_tooltips_set_tip(tips, GTK_WIDGET(cut),
     "Cut selection and place it in clipboard", "");

  gtk_tooltips_set_tip(tips, GTK_WIDGET(copy),
     "Copy selection to the clipboard", "");

  gtk_tooltips_set_tip(tips, GTK_WIDGET(paste),
     "Paste clipboard into current location", "");

  /* create another button */
  other_button = g_object_new(GTK_TYPE_BUTTON,
                              "label", "_Other group",
			      "use-underline", TRUE,
                              NULL);

  gtk_box_pack_end_defaults(GTK_BOX(box), GTK_WIDGET(other_button));

  /* use a different tooltips object for this button */
  other_tips = g_object_new(GTK_TYPE_TOOLTIPS, NULL);

  gtk_tooltips_set_tip(other_tips, GTK_WIDGET(other_button),
     "This button uses a different Tooltips object", "");

  /* pack all boxes, show everything, start GTK+ loop */
  gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(box));
  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_main();

  return 0;
}

