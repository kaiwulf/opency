/* -*-coding: utf-8;-*- */
/* decorations.c -- various decoration widgets */

#include <gtk/gtk.h>

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
  GtkHBox *hbox[2];
  GtkFrame *frame[5];
  GtkArrow *arrow[4];
  GtkVSeparator *v_sep[3];
  GtkHSeparator *h_sep;
  GtkVBox *vbox;
  gint i;

  /* initialize GTK+, create a window */
  gtk_init(&argc, &argv);
  window = g_object_new(GTK_TYPE_WINDOW,
                        "title", "Decorations",
                        "default-width", 500,
                        "default-height", 200,
                        "border-width", 12,
                        NULL);

  /* attach standard event handlers */
  g_signal_connect(window, "delete_event", G_CALLBACK(delete_event), NULL);
  g_signal_connect(window, "destroy", G_CALLBACK(end_program), NULL);
  
  /* HBox with four arrows (different directions) */
  hbox[0] = g_object_new(GTK_TYPE_HBOX, NULL);
  arrow[0] = g_object_new(GTK_TYPE_ARROW, "arrow-type", GTK_ARROW_UP,    NULL);
  arrow[1] = g_object_new(GTK_TYPE_ARROW, "arrow-type", GTK_ARROW_DOWN,  NULL);
  arrow[2] = g_object_new(GTK_TYPE_ARROW, "arrow-type", GTK_ARROW_LEFT,  NULL);
  arrow[3] = g_object_new(GTK_TYPE_ARROW, "arrow-type", GTK_ARROW_RIGHT, NULL);

  /* pack the arrows into the hbox, put separators between */
  for (i = 0; i < 4; i++)
  {
     gtk_box_pack_start_defaults(GTK_BOX(hbox[0]), GTK_WIDGET(arrow[i]));
     if (i < 3)
     {
        v_sep[i] = g_object_new(GTK_TYPE_VSEPARATOR, NULL);
        gtk_box_pack_start_defaults(GTK_BOX(hbox[0]), GTK_WIDGET(v_sep[i]));
     }
  }

  /* create a frame and put the hbox above inside */
  frame[0] = g_object_new(GTK_TYPE_FRAME, "label", "Arrows", NULL);

  gtk_container_add(GTK_CONTAINER(frame[0]), GTK_WIDGET(hbox[0]));

  /* create another hbox and put four more frames inside */
  hbox[1] = g_object_new(GTK_TYPE_HBOX, "spacing", 5, NULL);

  frame[1] = g_object_new(GTK_TYPE_FRAME,
                          "shadow-type", GTK_SHADOW_IN,
                          "label", "Shadowed In",
                          "label-xalign", 0.0,
                          NULL);
  frame[2] = g_object_new(GTK_TYPE_FRAME,
                          "shadow-type", GTK_SHADOW_OUT,
                          "label", "Shadowed Out",
                          "label-xalign", 0.5,
                          NULL);
  frame[3] = g_object_new(GTK_TYPE_FRAME,
                          "shadow-type", GTK_SHADOW_ETCHED_IN,
                          "label", "Etched Line",
                          "label-xalign", 0.5,
                          NULL);
  frame[4] = g_object_new(GTK_TYPE_FRAME,
                          "shadow-type", GTK_SHADOW_ETCHED_OUT,
                          "label", "Embossed Line",
                          "label-xalign", 1.0,
                          NULL);

  /* pack the frames into the hbox */
  for (i = 1; i < 5; i++)
  {
     gtk_box_pack_start_defaults(GTK_BOX(hbox[1]),
                                 GTK_WIDGET(frame[i]));
  }

  /* create a vbox, put both hboxes inside with a separator */
  vbox = g_object_new(GTK_TYPE_VBOX, NULL);
  gtk_box_pack_start_defaults(GTK_BOX(vbox), GTK_WIDGET(frame[0]));

  h_sep = g_object_new(GTK_TYPE_HSEPARATOR, NULL);
  gtk_box_pack_start_defaults(GTK_BOX(vbox), GTK_WIDGET(h_sep));

  /* put everything inside the window, show window, start loop */
  gtk_box_pack_start_defaults(GTK_BOX(vbox), GTK_WIDGET(hbox[1]));
  gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(vbox));
  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_main();

  return 0;
}

