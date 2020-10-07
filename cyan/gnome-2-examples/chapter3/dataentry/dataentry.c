/* -*-coding: utf-8;-*- */
/* dataentry.c -- basic data entry widgets */

#include <gtk/gtk.h>

/* usual event handlers */
gint delete_event(GtkWidget *widget, GdkEvent event, gpointer data)
{
  return FALSE;
}

void end_program(GtkWidget *widget, gpointer data)
{
  gtk_main_quit();
}

#define NUM_WIDGETS 5

int main(int argc, char **argv)
{
  GtkWindow *window;
  GtkTable *table;
  GtkOptionMenu *option_menu;
  GtkEntry *entry_box;
  GtkCombo *combo_box;
  GtkSpinButton *spin_button;
  GtkHScale *h_slider;
  GtkVScale *v_slider;

  GtkMenu *names_menu;         /* for option menu */
  GList *combo_list = NULL;    /* for combo button */
  GtkAdjustment *adjust;       /* for sliders and spin button */

  GtkWidget *mnemo;

  /* initialize GTK+, create window */
  gtk_init(&argc, &argv);
  window = g_object_new(GTK_TYPE_WINDOW,
                        "title", "Data Entry Widgets",
                        "border-width", 12,
                        "default-width", 400,
                         NULL);

  /* connect standard window handlers */
  g_signal_connect(window, "delete_event", G_CALLBACK(delete_event), NULL);
  g_signal_connect(window, "destroy", G_CALLBACK(end_program), NULL);

  /* define the range for the slider widgets, spin button */
  adjust = GTK_ADJUSTMENT(gtk_adjustment_new(1.0,    /* initial value */
                                             -100.0, /* minimum */
                                             100.0,  /* maximum */
                                             0.5,    /* step */
                                             10.0,   /* page step*/
                                             0.0));  /* page size */

  /* create vertical slider */
  v_slider = g_object_new(GTK_TYPE_VSCALE, "adjustment", adjust, NULL);

  /* create horizontal slider */
  h_slider = g_object_new(GTK_TYPE_HSCALE, "adjustment", adjust, NULL);

  /* create and define a menu */
  names_menu = g_object_new(GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(names_menu),
                        gtk_menu_item_new_with_label("Harrisburg"));
  gtk_menu_shell_append(GTK_MENU_SHELL(names_menu),
                        gtk_menu_item_new_with_label("Pittsburgh"));
  gtk_menu_shell_append(GTK_MENU_SHELL(names_menu),
                        gtk_menu_item_new_with_label("Hollidaysburg"));

  /* create an options menu, using the menu above */
  option_menu = g_object_new(GTK_TYPE_OPTION_MENU, "menu", names_menu, NULL);

  /* set index 1 as default */
  gtk_option_menu_set_history(option_menu, 1);

  /* create an entry box */
  entry_box = g_object_new(GTK_TYPE_ENTRY, "text", "type something here", NULL);

  /* create a list of names */
  combo_list = g_list_append(combo_list, "Townsville");
  combo_list = g_list_append(combo_list, "Pleasantville");
  combo_list = g_list_append(combo_list, "Springfield");
  combo_list = g_list_append(combo_list, "Smallville");

  /* create a combo box */
  combo_box = g_object_new(GTK_TYPE_COMBO, NULL);

  /* add the list of names to the combo box */
  gtk_combo_set_popdown_strings(combo_box, combo_list);

  /* set a default entry in combo box */
  g_object_set(combo_box->entry, "text", "Alphaville", NULL);

  /* list structure no longer necessary */
  g_list_free(combo_list);

  /* spin button */
  spin_button = g_object_new(GTK_TYPE_SPIN_BUTTON,
                            "adjustment", adjust,
                            "digits", 1,
                            "value", 42.0,
                            NULL);

  table = g_object_new(GTK_TYPE_TABLE,
                       "n-rows", NUM_WIDGETS,
                       "n-columns", 3,
                       "row-spacing", 6,
                       NULL);

  /* this macro packs one of the data entry widgets into
     the table, along with a label */
  /* if the widget is a combo box, set its mnemonic to
     the entry box inside the combo */
#define PACK_IN_TABLE(widget, label, row_num)                  \
    mnemo = GTK_WIDGET(widget);                                \
    if (GTK_IS_COMBO(widget))                                  \
      { mnemo = GTK_COMBO(widget)->entry; }                    \
    gtk_table_attach(table,                                    \
                     g_object_new(GTK_TYPE_LABEL,              \
                                  "label", label,              \
                                  "use-underline", TRUE,       \
                                  "mnemonic-widget", mnemo,    \
                                  "xalign", 1.0,               \
                                  "justify", GTK_JUSTIFY_RIGHT,\
                                  NULL),                       \
                     0, 1, row_num, row_num+1,                 \
                     GTK_EXPAND|GTK_FILL, 0, 0, 0);            \
                                                               \
    gtk_table_attach(table,                                    \
                     g_object_new(GTK_TYPE_ALIGNMENT,          \
                                  "xalign", 0.0,               \
                                  "child", widget,             \
                                  NULL),                       \
                     1, 2, row_num, row_num+1,                 \
                     GTK_EXPAND|GTK_FILL, 0, 6, 0);

  /* pack the widgets and their labels into the table */
  PACK_IN_TABLE(option_menu, "_Option menu:",      0)
  PACK_IN_TABLE(entry_box,   "_Entry box:",        1)
  PACK_IN_TABLE(combo_box,   "_Combo box:",        2)
  PACK_IN_TABLE(spin_button, "_Spin button:",      3)
  PACK_IN_TABLE(h_slider,    "_Horizonal slider:", 4)

  /* pack the vertical slider and its label into the table */
  gtk_table_attach(table,
                   g_object_new(GTK_TYPE_LABEL,
                                "label", "_Vertical slider:",
                                "use_underline", TRUE,
                                "mnemonic-widget", v_slider,
                                NULL),
                   2, 3, 0, 1,
                   GTK_SHRINK, 0, 6, 0);

  gtk_table_attach(table, GTK_WIDGET(v_slider),
                   2, 3, 1, 5,
                   GTK_SHRINK,
                   GTK_EXPAND|GTK_FILL, 0, 0);

  /* pack table into window, show everything, begin main loop */
  gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(table));
  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_main();
  return 0;
}
