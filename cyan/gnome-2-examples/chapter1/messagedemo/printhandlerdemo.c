/* printhandlerdemo.c */

#include <stdio.h>
#include <glib.h>

#define N 1

/* print messages in ALL CAPS */
void my_printerr_handler(gchar *string)
{
  GString *msg;

  msg = g_string_new(string);
  msg = g_string_ascii_up(msg);
  fprintf(stderr, "%s\n", msg->str);
  g_string_free(msg, TRUE);
}

int main(int argc, char **argv)
{
  /* print to stdout */
  g_print("If you lie %d time, no one believes you.\n", N);

  /* print to stderr */
  g_printerr("Ouch.\n");

  /* but if you lie all of the time... */
  g_set_printerr_handler((GPrintFunc)my_printerr_handler);
  g_printerr("%d. Ouch. Ouch. Ouch. (Hey, that really hurts.)", N);

  return 0;
}
