/* gvaluedemo.c -- demonstrate GValue */

#include <glib-object.h>
#include <stdio.h>

int main(int argc, char **argv)
{
  GValue *value;

  g_type_init(); /* initialize type system */

  /* allocate new GValue, zero out contents */
  value = g_new0(GValue, 1);

  /* initialize GValue type to gint */
  g_value_init(value, G_TYPE_INT);

  /* set the value to 42 and read it out again */
  g_value_set_int(value, 42);
  g_print("Value: %d\n", g_value_get_int(value));

  /* is the type of the GValue a GObject? */
  if (G_VALUE_HOLDS_OBJECT(value))
  {
     g_print("Container holds a GObject\n");
  } else {
     g_print("Container does not hold a GObject\n");
  }
  /* expect "Container does not hold a GObject" */

  g_free(value);

  return 0;
}
