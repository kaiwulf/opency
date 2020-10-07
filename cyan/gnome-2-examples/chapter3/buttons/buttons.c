/* -*-coding: utf-8;-*- */
/* buttons.c -- button demo */

#include <gtk/gtk.h>

static GtkRadioButton *radio_temper[3];
static GtkCheckButton *check_option[3];

/* standard event handlers */
gint delete_event(GtkWidget *widget, GdkEvent event, gpointer data)
{
  return FALSE;
}

void end_program(GtkWidget *widget, gpointer data)
{
  gtk_main_quit();
}

void prepare_drink(gpointer drink)
{
  gboolean scalding, hot, lukewarm = FALSE;
  gboolean milk, sugar, rum = FALSE;

  /* extract the temperature from radio buttons */
  g_object_get(radio_temper[0], "active", &scalding, NULL);
  g_object_get(radio_temper[1], "active", &hot, NULL);
  g_object_get(radio_temper[2], "active", &lukewarm, NULL);
  /* extract any options from check buttons */
  g_object_get(check_option[0], "active", &milk, NULL);
  g_object_get(check_option[1], "active", &sugar, NULL);
  g_object_get(check_option[2], "active", &rum, NULL);

  /* Piece together a little statement of drink to prepare. */
  g_print("Preparing%s%s%s %s",
          scalding ? " scalding" : "",
          hot      ? " hot"      : "",
          lukewarm ? " lukewarm" : "",
          (gchar *) drink);

  if (milk || sugar || rum)
  {
     g_print(" with:%s%s%s",
             milk  ? " milk"  : "",
             sugar ? " sugar" : "",
             rum   ? " rum"   : "" );
  }
  g_print(".\n");
}

int main(int argc, char **argv)
{
  GtkWindow *window;
  GtkHBox *hbox;
  GtkVBox *vbox[3];
  GtkButton *button_prepare[3];
  gint i;

  /* initialize GTK+, create a window */
  gtk_init(&argc, &argv);
 
  window = g_object_new(GTK_TYPE_WINDOW,
                        "title", "Mawa's Drink-o-Matic",
                        "border-width", 12,
                        NULL);

  /* add standard signal handlers */
  g_signal_connect(window, "delete_event", G_CALLBACK(delete_event), NULL);
  g_signal_connect(window, "destroy", G_CALLBACK(end_program), NULL);
  
  /* create boxes and pack them */
  hbox = g_object_new(GTK_TYPE_HBOX, "spacing", 6, NULL);

  for (i=0; i<3; i++)
  {
     vbox[i] = g_object_new(GTK_TYPE_VBOX, "spacing", 6, NULL);
     gtk_box_pack_start_defaults(GTK_BOX(hbox), GTK_WIDGET(vbox[i]));
  }
  gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(hbox));

  /* create and pack regular buttons for preparing drinks */
  /* they go in the leftmost VBox */
  for (i=0; i<3; i++)
  {
     button_prepare[i] = g_object_new(GTK_TYPE_BUTTON,
                                      "use-underline", TRUE,
                                      NULL);
     gtk_box_pack_start_defaults(GTK_BOX(vbox[0]), GTK_WIDGET(button_prepare[i]));
  }

  g_object_set(button_prepare[0], "label", "Prepare _Coffee", NULL);
  g_signal_connect_swapped(button_prepare[0],
                           "clicked", G_CALLBACK(prepare_drink), "coffee");

  g_object_set(button_prepare[1], "label", "Prepare _Tea", NULL);
  g_signal_connect_swapped(button_prepare[1],
                           "clicked", G_CALLBACK(prepare_drink), "tea");

  g_object_set(button_prepare[2], "label", "Prepare C_ocoa", NULL);
  g_signal_connect_swapped(button_prepare[2],
                           "clicked", G_CALLBACK(prepare_drink), "cocoa");

  /* create temperature radio buttons, pack into center VBox */
  for (i=0; i<3; i++)
  {
     radio_temper[i] = g_object_new(GTK_TYPE_RADIO_BUTTON,
                                    "use-underline", TRUE,
                                    NULL);
     gtk_box_pack_start_defaults(GTK_BOX(vbox[1]), GTK_WIDGET(radio_temper[i]));
  }
  g_object_set(radio_temper[0], "label", "_Scalding", NULL);
  g_object_set(radio_temper[1],
               "label", "_Hot",
               "group", radio_temper[0],
               NULL);
  g_object_set(radio_temper[2],
               "label", "_Lukewarm",
               "group", radio_temper[0],
               NULL);

  /* create check buttons for additives, pack into third VBox */
  for (i=0; i<3; i++)
  {
     check_option[i] = g_object_new(GTK_TYPE_CHECK_BUTTON,
                                    "use-underline", TRUE,
                                    NULL);
     gtk_box_pack_start_defaults(GTK_BOX(vbox[2]), GTK_WIDGET(check_option[i]));
  }
  g_object_set(check_option[0], "label", "_Milk", NULL);
  g_object_set(check_option[1], "label", "S_ugar", NULL);
  g_object_set(check_option[2], "label", "_Rum", NULL);

  /* show window, start main event loop */
  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_main();
  return 0;
}
