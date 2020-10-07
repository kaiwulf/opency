/* -*-coding: utf-8;-*- */
/* text.c -- demonstrate text widgets */

#include <gtk/gtk.h>

GtkTextBuffer *buffer;
GtkTextTag *bold, *italic, *underline;
GtkCheckButton *bold_button, *italic_button, *underline_button;

gboolean format_dirty = FALSE;

/* when someone moves the cursor or selects something in the view, mark
   the selection variable as "changed" so that verify_format() can change
   the format buttons accordingly */
void changed_selection(GtkTextBuffer *buf,
                       GtkTextIter i,
                       GtkTextMark *mark,  /* function does not use parms */
                       gpointer data)
{
  format_dirty = TRUE;
}

/* check the selection and cursor position, set the format buttons to match */
gboolean verify_format(gpointer data) {
  GtkTextIter begin, end;
  gboolean bold_on, bold_off;
  gboolean italic_on, italic_off;
  gboolean underline_on, underline_off;

  /* if the cursor position or selection has not changed, do nothing */
  if (!format_dirty) {
     return(TRUE);
  }

  gtk_text_buffer_get_selection_bounds(buffer, &begin, &end);

  if (!gtk_text_iter_equal(&begin, &end))
  {
     GtkTextIter iter = begin;

     /* assume that all formatting styles are true at start */
     bold_on = bold_off = TRUE;
     italic_on = italic_off = TRUE;
     underline_on = underline_off = TRUE;

     /* run through the selection, setting format styles as necessary */
     while (!gtk_text_iter_equal(&iter, &end))
     {
        if (gtk_text_iter_has_tag(&iter, bold))
        {
           bold_off = FALSE;
        } else {
           bold_on = FALSE;
        }

        if (gtk_text_iter_has_tag(&iter, italic))
        {
           italic_off = FALSE;
        } else {
           italic_on = FALSE;
        }

        if (gtk_text_iter_has_tag(&iter, underline))
        {
           underline_off = FALSE;
        } else {
           underline_on = FALSE;
        }

        gtk_text_iter_forward_cursor_position(&iter);
     }

     /* set check buttons to inconsistent when the tags is both
        on and off in the selection */
        
     if (!bold_on && !bold_off)
     {
        g_object_set(bold_button, "inconsistent", TRUE, NULL);
     } else {
        g_object_set(bold_button, "inconsistent", FALSE,
                                  "active", bold_on,
                                  NULL);
     }

     if (!italic_on && !italic_off)
     {
        g_object_set(italic_button, "inconsistent", TRUE, NULL);
     } else {
        g_object_set(italic_button, "inconsistent", FALSE,
                                    "active", italic_on,
                                    NULL);
     }

     if (!underline_on && !underline_off)
     {
        g_object_set(underline_button, "inconsistent", TRUE, NULL);
     } else {
        g_object_set(underline_button, "inconsistent", FALSE,
                                       "active", underline_on,
                                       NULL);
     }
  } else {
    /* if there isn't a selection, just show the tags at the
       current cursor position */
     bold_on = gtk_text_iter_has_tag(&begin, bold);
     italic_on = gtk_text_iter_has_tag(&begin, italic);
     underline_on = gtk_text_iter_has_tag(&begin, underline);

     g_object_set(bold_button, "inconsistent", FALSE,
                               "active", bold_on,
                               NULL);

     g_object_set(italic_button, "inconsistent", FALSE,
                                 "active", italic_on,
                                 NULL);

     g_object_set(underline_button, "inconsistent", FALSE,
                                    "active", underline_on,
                                    NULL);
  }
  /* reset state */
  format_dirty = FALSE;

  return(TRUE);
}

/* when user clicks on a format check button, change tags */
void format_text(GtkCheckButton *tag_button, gpointer formatptr)
{
  gchar *format = (gchar *)formatptr;
  GtkTextIter begin, end;
  gboolean inconsistent;
  gboolean button_checked;

  gtk_text_buffer_get_selection_bounds(buffer, &begin, &end);
  if (!gtk_text_iter_equal(&begin, &end))
  {
     g_object_get(tag_button, "active", &button_checked,
                              "inconsistent", &inconsistent, NULL);
     if (button_checked || inconsistent)
     {
        gtk_text_buffer_apply_tag_by_name(buffer, format, &begin, &end);
        g_object_set(tag_button, "active", TRUE,
                                 "inconsistent", FALSE,
                                 NULL);
     } else {
        gtk_text_buffer_remove_tag_by_name(buffer, format, &begin, &end);
	  g_object_set(tag_button, "active", FALSE,
                                   "inconsistent", FALSE,
                                   NULL);
     }
  }
}

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
  GtkTextTagTable *tags;
  GtkTextView *view;
  GtkScrolledWindow *scroller;
  GtkHButtonBox *buttons;
  GtkVBox *vbox;

  GtkTextChildAnchor *anchor;
  GtkTextIter embed_iter;
  GtkButton *text_button;
  GdkPixbuf *pixbuf;

  /* initialize GTK+, create main window */
  gtk_init(&argc, &argv);
  window = g_object_new(GTK_TYPE_WINDOW,
                        "title", "Text Demo",
                        "default-width", 400,
                        "default-height", 500,
                        NULL);

  /* attach standard handlers */
  g_signal_connect(window, "delete_event", G_CALLBACK(delete_event), NULL);
  g_signal_connect(window, "destroy", G_CALLBACK(end_program), NULL);

  /* create new tag table and buffer objects */
  tags = gtk_text_tag_table_new();
  buffer = gtk_text_buffer_new(tags);

  /* create new text view using buffer */
  view = GTK_TEXT_VIEW(gtk_text_view_new_with_buffer(buffer));

  g_object_set(view, "wrap-mode", GTK_WRAP_WORD, NULL);

  /* add a handler to verify format buttons when the selection, 
     cursor position, or mark otherwise changes */
  g_signal_connect(buffer, "mark-set", G_CALLBACK(changed_selection), NULL);

  /* create tags for bold, italic, and underline */
  bold = gtk_text_buffer_create_tag(buffer, "bold",
                                    "weight", 500,
                                    NULL);

  italic = gtk_text_buffer_create_tag(buffer, "italic",
                                    "style", PANGO_STYLE_ITALIC,
                                     NULL);

  underline = gtk_text_buffer_create_tag(buffer, "underline",
                                         "underline", PANGO_UNDERLINE_SINGLE,
                                         NULL);

  /* add some default text to the buffer */
  gtk_text_buffer_set_text(buffer, "\
The text you see in this window is currently stored in a \
GtkTextBuffer object; you see it with the help of a \
GtkTextView widget.\n
Feel free to type your own text anywhere in this window.\n\n\
If you click any of the buttons below, you will change the \
formatting of any selected text.\n\n\
Widgets and images can appear in text widgets:\n", -1);

  /* add a button to the text */
  text_button = g_object_new(GTK_TYPE_BUTTON, "label", "Button", NULL);
  gtk_text_buffer_get_end_iter(buffer, &embed_iter);
  anchor = gtk_text_buffer_create_child_anchor(buffer, &embed_iter);
  gtk_text_view_add_child_at_anchor(view, GTK_WIDGET(text_button), anchor);

  /* add an image to the text */
  pixbuf = gdk_pixbuf_new_from_file("apple-green.png", NULL);
  gtk_text_buffer_get_end_iter(buffer, &embed_iter);
  gtk_text_buffer_insert_pixbuf(buffer, &embed_iter, pixbuf);

  /* put view into scrolling widget */
  scroller = g_object_new(GTK_TYPE_SCROLLED_WINDOW, NULL);
  gtk_container_add(GTK_CONTAINER(scroller), GTK_WIDGET(view));

  /* create check buttons for text formatting */
  bold_button = g_object_new(GTK_TYPE_CHECK_BUTTON,
                             "use-stock", TRUE,
                             "label", GTK_STOCK_BOLD,
                             NULL);

  italic_button = g_object_new(GTK_TYPE_CHECK_BUTTON,
                               "use-stock", TRUE,
                               "label", GTK_STOCK_ITALIC,
                               NULL);

  underline_button = g_object_new(GTK_TYPE_CHECK_BUTTON,
                                  "use-stock", TRUE,
                                  "label", GTK_STOCK_UNDERLINE,
                                  NULL);

  /* bind format_text() to these check buttons so that the 
     the selection's format changes when you click one */
  g_signal_connect(bold_button, "clicked",
                   G_CALLBACK(format_text), "bold");
  g_signal_connect(italic_button, "clicked",
                   G_CALLBACK(format_text), "italic");
  g_signal_connect(underline_button, "clicked",
                   G_CALLBACK(format_text), "underline");

  /* pack the check buttons into a button box */
  buttons = g_object_new(GTK_TYPE_HBUTTON_BOX, NULL);
  gtk_box_pack_start_defaults(GTK_BOX(buttons), GTK_WIDGET(bold_button));
  gtk_box_pack_start_defaults(GTK_BOX(buttons), GTK_WIDGET(italic_button));
  gtk_box_pack_start_defaults(GTK_BOX(buttons), GTK_WIDGET(underline_button));

  /* pack the text view into a scrolled container */
  vbox = g_object_new(GTK_TYPE_VBOX, NULL);

  /* put everything into a vbox, then into the main window*/
  gtk_box_pack_start_defaults(GTK_BOX(vbox), GTK_WIDGET(scroller));
  gtk_box_pack_end(GTK_BOX(vbox), GTK_WIDGET(buttons), FALSE, FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(vbox));

  /* show everything */
  gtk_widget_show_all(GTK_WIDGET(window));

  /* add a timeout for the format button update */
  g_timeout_add(100, verify_format, NULL);

  /* start GTK+ main event loop */
  gtk_main();

  return 0;
}
