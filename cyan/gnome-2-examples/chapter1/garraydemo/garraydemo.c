/* Glib 2.0 GArray demo */

#include <glib.h>

void print(GArray *array)
{
  gint i = 0;

  while (0 != g_array_index(array, gint, i))
  {
     g_print("%d ", g_array_index(array, gint, i));
     i++;
  }

  g_print("\n");
}

int main(int argc, char **argv)
{
  GArray *array;
  gint c_array[3];
  gint tmp;

  array = g_array_new(TRUE,     /* with null terminator */
                      FALSE,         /* don't blank memory */
                      sizeof(gint)); /* element size */

  c_array[0] = 42;
  c_array[1] = 23;
  c_array[2] = 69;
  array = g_array_append_vals(array, (gconstpointer)c_array, 3);
  print(array);

  tmp = 380;
  array = g_array_prepend_val(array, tmp);
  print(array);

  c_array[0] = 220;
  c_array[1] = 0xdeadbeef;
  array = g_array_insert_vals(array, 1, (gconstpointer)c_array, 2);
  print(array);

  g_array_index(array, gint, 1) = 37;
  print(array);

  /* pull the deadbeef out */
  g_array_remove_index(array, 2);
  print(array);

  /* pull the second element out, replacing it with the last
   * because this function messes up the array order
   */
  g_array_remove_index_fast(array, 1);
  print(array);

  g_array_free(array, TRUE);

  return 0;
}
