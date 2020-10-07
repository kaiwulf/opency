/* -*-coding: utf-8;-*- */
/* appdemo.c -- bare-bones GnomeApp */

#include <gnome.h>

/* standard event handlers */
gint delete_event(GtkWidget *widget, GdkEvent event, gpointer data)
{
  return FALSE;
}

void end_program(GtkWidget *widget, gpointer data)
{
  gtk_main_quit();
}

/* edit command handlers */
void edit_handler(GtkWidget *item, gpointer data)
{
  g_print("`%s' called\n", (gchar *)data);
}

int main(int argc, char **argv)
{
  GnomeProgram *program;
  GnomeApp *window;
  GtkAccelGroup *accel_g;
  GtkMenuBar *menu_bar;
  GtkWidget *file, *edit, *help,
            *file_close,
            *edit_cut, *edit_copy, *edit_paste,
            *help_index, *help_about;
  GtkMenu *file_menu, *edit_menu, *help_menu;
  GtkToolbar *tools;
  GtkStatusbar *status;
  GtkLabel *label;

  /* initialize GNOME */
  program = gnome_program_init("appdemo",
                               "0.1",
                               LIBGNOMEUI_MODULE,
                               argc, argv,
                               GNOME_PROGRAM_STANDARD_PROPERTIES,
                               GNOME_PARAM_HUMAN_READABLE_NAME, "AppDemo",
                               GNOME_PARAM_ENABLE_SOUND, TRUE,
                               NULL);

  /* create main window */
  window = g_object_new(GNOME_TYPE_APP,
                        "title", "Application Window",
                        "app-id", "appdemo",
                        "default-width", 300,
                        "default-height", 300,
                        NULL);

  /* attach standard event handlers */
  g_signal_connect(window, "delete_event", G_CALLBACK(delete_event), NULL);
  g_signal_connect(window, "destroy", G_CALLBACK(end_program), NULL);

  /* add accelerator group */
  accel_g = g_object_new(GTK_TYPE_ACCEL_GROUP, NULL);
  gtk_window_add_accel_group(GTK_WINDOW(window), accel_g);

  /* create menu/tool/status bars */
  menu_bar = g_object_new(GTK_TYPE_MENU_BAR, NULL);
  tools = g_object_new(GTK_TYPE_TOOLBAR, NULL);
  status = g_object_new(GTK_TYPE_STATUSBAR, NULL);

  /* create file close item */
  file_close = gtk_image_menu_item_new_from_stock(GTK_STOCK_CLOSE, accel_g);

  /* attach the standard window destroy handler for the close item */
  g_signal_connect(file_close, "activate", G_CALLBACK(end_program), NULL);

  /* create file menu and attach the close item */
  file_menu = g_object_new(GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_close);

  /* create the item for "File" in the menu bar and attach the menu */
  file = gtk_menu_item_new_with_mnemonic("_File");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), GTK_WIDGET(file_menu));

  /* create edit menu items */
  edit_cut = gtk_image_menu_item_new_from_stock(GTK_STOCK_CUT, accel_g);
  /* use a custom handler */
  g_signal_connect(edit_cut, "activate", G_CALLBACK(edit_handler), "Cut");

  /* do the same for copy and paste items */
  edit_copy = gtk_image_menu_item_new_from_stock(GTK_STOCK_COPY, accel_g);
  g_signal_connect(edit_copy, "activate", G_CALLBACK(edit_handler), "Copy");

  edit_paste = gtk_image_menu_item_new_from_stock(GTK_STOCK_PASTE, accel_g);
  g_signal_connect(edit_paste, "activate", G_CALLBACK(edit_handler), "Paste");

  /* create edit menu and put items inside */
  edit_menu = g_object_new(GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), edit_cut);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), edit_copy);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), edit_paste);
	  
  /* create the item for "Edit" in the menu bar and attach the menu */
  edit = gtk_menu_item_new_with_mnemonic( "_Edit");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), edit);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit), GTK_WIDGET(edit_menu));

  /* make a help menu */
  help_index = gtk_image_menu_item_new_from_stock(GTK_STOCK_HELP, accel_g);
  help_about = gtk_image_menu_item_new_from_stock(GNOME_STOCK_ABOUT, accel_g);

  help_menu = g_object_new(GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_index);
  gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_about);

  help = gtk_menu_item_new_with_mnemonic("_Help");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), help);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), GTK_WIDGET(help_menu));

  /* place menus into application window and show everything */
  gnome_app_set_menus(window, menu_bar);
  gtk_widget_show_all(GTK_WIDGET(menu_bar));

  /* put some buttons in the toolbar */
  gtk_toolbar_insert_stock(tools, GTK_STOCK_CUT,
                           "Delete selection and place into clipboard",
                           NULL, G_CALLBACK(edit_handler), "Cut", -1);

  gtk_toolbar_insert_stock(tools, GTK_STOCK_COPY,
                           "Copy selection to clipboard",
                           NULL, G_CALLBACK(edit_handler), "Copy", -1);

  gtk_toolbar_insert_stock(tools, GTK_STOCK_PASTE,
                           "Paste clipboard",
                           NULL, G_CALLBACK(edit_handler), "Paste", -1);

  /* put toolbar in main application window */
  gnome_app_set_toolbar(window, tools);

  /* add the status bar */
  gnome_app_set_statusbar(window, GTK_WIDGET(status));

  /* a simple placeholder label as the content */
  label = g_object_new(GTK_TYPE_LABEL, "label", "Your idea goes here.", NULL);

  /* set the label as the application content */
  gnome_app_set_contents(window, GTK_WIDGET(label));

  /* show everything and start GTK+ main event loop */
  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_main();

  return 0;
}

