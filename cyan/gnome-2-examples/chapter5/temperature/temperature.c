/* -*-coding: utf-8;-*- */
/* temperature.c -- temperature converter using libglade */

#include <gnome.h>
#include <glade/glade.h>

typedef enum {
  KELVIN,
  CELSIUS,
  FAHRENHEIT,
  REAUMUR,
  RANKINE,
  NUM_TEMPERATURE_SCALES
} TemperatureScale;

const gchar *widget_name[] = {
  "kelvin-spin",
  "celsius-spin",
  "fahrenheit-spin",
  "reaumur-spin",
  "rankine-spin"
};

GtkSpinButton *temp_spin[NUM_TEMPERATURE_SCALES];
gulong handler[NUM_TEMPERATURE_SCALES];
gdouble temp_value[NUM_TEMPERATURE_SCALES];

void changed(GtkSpinButton *spin_button, gpointer id_ptr)
{
  TemperatureScale scale_id = (TemperatureScale)id_ptr;
  TemperatureScale i;

  g_assert(scale_id >= KELVIN && scale_id < NUM_TEMPERATURE_SCALES);

  /* read the temperature from the spin button */
  g_object_get(temp_spin[scale_id], "value", &temp_value[scale_id], NULL);

  /* determine the Kelvin equivalent of that temperature */
  switch (scale_id)
  {
     case CELSIUS:
        temp_value[KELVIN] = temp_value[CELSIUS] + 273.15;
        break;

     case FAHRENHEIT:
        temp_value[KELVIN] = (5.0/9.0)*(temp_value[FAHRENHEIT]-32.0) + 273.15;
        break;
  
     case REAUMUR:
        temp_value[KELVIN] = (5.0/4.0)*temp_value[REAUMUR] + 273.15;
        break;

     case RANKINE:
        temp_value[KELVIN] = (5.0/9.0)*temp_value[RANKINE];
        break;

     default:
        break;
  }

  /* determine the rest of the values based on the Kelvin temperature */
  if (scale_id != CELSIUS)
     temp_value[CELSIUS] = temp_value[KELVIN] - 273.15;

  if (scale_id != FAHRENHEIT)
     temp_value[FAHRENHEIT] = (9.0/5.0)*(temp_value[KELVIN]-273.15) + 32.0;

  if (scale_id != REAUMUR)
     temp_value[REAUMUR] = (4.0/5.0)*(temp_value[KELVIN]-273.15);

  if (scale_id != RANKINE)
     temp_value[RANKINE] = (9.0/5.0)*temp_value[KELVIN];

  /* write the new values back into the spin buttons;
     disable signal handlers when doing this so that this
     handler function doesn't get called again and again and.. */
  for (i=KELVIN; i<NUM_TEMPERATURE_SCALES; i++)
  {
     if (scale_id != i)
     {
        g_signal_handler_block(temp_spin[i], handler[i]);
        g_object_set(temp_spin[i], "value", temp_value[i], NULL);
        g_signal_handler_unblock(temp_spin[i], handler[i]);
     }
  }
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

void close_clicked(GtkButton *button, gpointer window_ptr)
{
  GtkDialog *window = GTK_DIALOG(window_ptr);
  gtk_widget_destroy(GTK_WIDGET(window));
}

int main(int argc, char **argv)
{
  GladeXML *ui;
  GnomeProgram *program;
  GtkDialog *window;
  TemperatureScale i;

  /* initialize GNOME */
  program = gnome_program_init("temperature", "0.1",
                               LIBGNOMEUI_MODULE,
                               argc, argv,
                               GNOME_PROGRAM_STANDARD_PROPERTIES,
                               GNOME_PARAM_HUMAN_READABLE_NAME, "Temperature",
                               NULL);

  /* read the definitions with libglade */
  ui = glade_xml_new("temperature.glade", NULL, NULL);

  /* get a handle on each of the spin button widgets */
  for (i=KELVIN; i < NUM_TEMPERATURE_SCALES; i++)
  {
     temp_spin[i] = GTK_SPIN_BUTTON(glade_xml_get_widget(ui, widget_name[i]));
  }
  window = GTK_DIALOG(glade_xml_get_widget(ui, "window"));

  /* bind "value-changed" signal handler for each spin button,
     use an identifier as the the user data pointer */
  for (i = KELVIN; i < NUM_TEMPERATURE_SCALES; i++)
  {
     handler[i] = g_signal_connect(temp_spin[i],
                                   "value-changed", G_CALLBACK(changed),
                                   GUINT_TO_POINTER(i));
  }

  /* set the starting value to the boiling point of water */
  g_object_set(temp_spin[CELSIUS], "value", 100.0, NULL);

  /* attach standard handlers */
  g_signal_connect(window, "delete_event", G_CALLBACK(delete_event), NULL);
  g_signal_connect(window, "destroy", G_CALLBACK(end_program), NULL);

  /* attach the close button handler with the help of libglade */
  glade_xml_signal_connect_data(ui,
                                "close-button-clicked",
                                G_CALLBACK(close_clicked),
                                GUINT_TO_POINTER(window));

  gtk_main();        /* start GTK+ main event loop */
  return 0;
}
