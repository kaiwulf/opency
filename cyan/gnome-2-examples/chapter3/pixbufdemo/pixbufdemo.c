/* -*-coding: utf-8;-*- */
/* pixbufdemo.c -- GdkPixbuf demo */

#include <gtk/gtk.h>

/* standard handlers */
gint delete_event(GtkWidget *widget, GdkEvent event, gpointer data)
{
  return FALSE;
}

void end_program(GtkWidget *widget, gpointer data)
{
  gtk_main_quit();
}

/* rounds a number to the nearest whole percentage and
   returns this (followed by %) in a newly-allocated string */
gchar* percent(GtkScale *scale, gdouble number, gpointer data)
{
  gint percent;
  gchar *str;

  percent = (gint)(number * 100);
  str = g_strdup_printf("%d%%", percent);

  return str;
}
  
/* (re-)render the image when it changes */
void render_image(GtkWidget *adjuster, gpointer data)
{
  GtkImage *image;
  GtkAdjustment *zoom, *saturation;
  GdkPixbuf *orig_pixbuf, *new_pixbuf;
  gint orig_width, orig_height;
  gint new_width, new_height;
  gdouble zoom_value, saturation_value;

  image = (GtkImage *) data;

  /* get the original pixbuf dimensions */
  orig_pixbuf = (GdkPixbuf *)g_object_get_data(G_OBJECT(image), "orig-pixbuf");
  orig_width = gdk_pixbuf_get_width(orig_pixbuf);
  orig_height = gdk_pixbuf_get_height(orig_pixbuf);

  /* get adjuster-induced changes */
  zoom = (GtkAdjustment *)g_object_get_data(G_OBJECT(image), "zoom");
  zoom_value = gtk_adjustment_get_value(zoom);
  saturation = (GtkAdjustment *)g_object_get_data(G_OBJECT(image), "saturation");
  saturation_value = gtk_adjustment_get_value(saturation);

  /* compute new size */
  new_width = (gint)(orig_width * zoom_value);
  new_height = (gint)(orig_height * zoom_value);

  /* prevent a height or width of 0 */
  new_width = MAX(1, new_width);
  new_height = MAX(1, new_height);

  /* scale the original pixbuf to the new dimensions
     (feel free to try other interpolation algorithms) */
  new_pixbuf = gdk_pixbuf_scale_simple(orig_pixbuf,
                                       new_width,
                                       new_height,
                                       GDK_INTERP_BILINEAR);

  /* modify the saturation on the newly-scaled pixbuf
     note that args 1 and 2 are the same */
  gdk_pixbuf_saturate_and_pixelate(new_pixbuf,
                                   new_pixbuf,
                                   saturation_value,
                                   FALSE);

  /* display the new pixbuf in the image widget */
  g_object_set(image, "pixbuf", new_pixbuf, NULL);
  /* reference to new_pixbuf is no longer necessary,
     the system may dispose of it when convenient */
  g_object_unref(new_pixbuf);
}

int main(int argc, char **argv)
{
  GtkWindow *window;
  GtkHBox *hbox;

  GtkImage *image;

  GtkVBox       *zoom_box,    *saturation_box;
  GtkLabel      *zoom_label,  *saturation_label;
  GtkVScale     *zoom_slider, *saturation_slider;
  GtkAdjustment *zoom,        *saturation;

  GdkPixbuf *orig_pixbuf;

  /* initialize GTK+, create a window, attach handlers */
  gtk_init(&argc, &argv);
  window = g_object_new(GTK_TYPE_WINDOW,
                        "title", "GdkPixbuf Demo",
                        "default-width", 300,
                        "default-height", 300,
                        "border-width", 12,
                        NULL);

  /* attach standard event handlers */
  g_signal_connect(window, "delete_event", G_CALLBACK(delete_event), NULL);
  g_signal_connect(window, "destroy", G_CALLBACK(end_program), NULL);

  /* create image widget and load a file */
  image = g_object_new(GTK_TYPE_IMAGE, "file", "apple-green.png", NULL);

  /* store the original pixbuf in the image widget data */
  g_object_get(image, "pixbuf", &orig_pixbuf, NULL);
  g_object_set_data(G_OBJECT(image), "orig-pixbuf", (gpointer)orig_pixbuf);
  
  /* define storage for zoom slider */
  zoom = GTK_ADJUSTMENT(gtk_adjustment_new(1.0,   /* default */
                                           0.01,  /* minimum */
                                           7.5,   /* maximum */
                                           0.01,  /* step increment */
                                           0.1,   /* page increment  */
                                           0.0)); /* page size */

  /* create zoom slider */
  zoom_slider = g_object_new(GTK_TYPE_VSCALE,
                             "draw_value", TRUE,
                             "value-pos", GTK_POS_BOTTOM,
                             "adjustment", zoom,
                              NULL);

  /* create label for zoom slider */
  zoom_label = g_object_new(GTK_TYPE_LABEL,
                            "label", "_Zoom:",
                            "use-underline", TRUE,
                            "mnemonic-widget", zoom_slider,
                            NULL);

  /* format the zoom slider's display as a percentage */
  g_signal_connect(zoom_slider, "format-value", G_CALLBACK(percent), NULL);

  /* put all of the zoom elements in a vbox */
  zoom_box = g_object_new(GTK_TYPE_VBOX, NULL);
  gtk_box_pack_start(GTK_BOX(zoom_box), GTK_WIDGET(zoom_label), FALSE, FALSE, 0);
  gtk_box_pack_start_defaults(GTK_BOX(zoom_box), GTK_WIDGET(zoom_slider));

  /* now do all of the above for a saturation slider */
  saturation = GTK_ADJUSTMENT(gtk_adjustment_new(1.0, 0.0, 5.0, 0.01, 0.1, 0.0));

  saturation_slider = g_object_new(GTK_TYPE_VSCALE,
                                   "draw-value", TRUE,
                                   "value-pos", GTK_POS_BOTTOM,
                                   "adjustment", saturation,
                                   "update-policy", GTK_UPDATE_DELAYED,
                                    NULL);

  saturation_label = g_object_new(GTK_TYPE_LABEL,
                                  "label", "_Saturation:",
                                  "use-underline", TRUE,
                                  "mnemonic-widget", saturation_slider,
                                  NULL);

  saturation_box = g_object_new(GTK_TYPE_VBOX, NULL);
  gtk_box_pack_start(GTK_BOX(saturation_box),
                     GTK_WIDGET(saturation_label),
                     FALSE, FALSE, 0);

  gtk_box_pack_start_defaults(GTK_BOX(saturation_box),
                              GTK_WIDGET(saturation_slider));

  /* store the adjuster widgets in the image object */
  g_object_set_data(G_OBJECT(image), "zoom", (gpointer)zoom);
  g_object_set_data(G_OBJECT(image), "saturation", (gpointer)saturation);

  /* install adjuster signal handlers */
  g_signal_connect(zoom, "value-changed",
                   G_CALLBACK(render_image), (gpointer) image);
  g_signal_connect(saturation, "value-changed",
                   G_CALLBACK(render_image), (gpointer) image);

  /* create a new HBox, pack the image and vboxes above */
  hbox = g_object_new(GTK_TYPE_HBOX, NULL);
  gtk_box_pack_start_defaults(GTK_BOX(hbox), GTK_WIDGET(image));
  gtk_box_pack_end(GTK_BOX(hbox), GTK_WIDGET(zoom_box), FALSE, FALSE, 3);
  gtk_box_pack_end(GTK_BOX(hbox), GTK_WIDGET(saturation_box), FALSE, FALSE, 3);

  /* pack everything into the window, show everything, start GTK+ main loop */
  gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(hbox));
  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_main();

  return 0;
}

