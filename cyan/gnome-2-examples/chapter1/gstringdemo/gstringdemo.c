/* Glib 2.0 GString demo */

#include <glib.h>

int main(int argc, char **argv)
{
  GString *s1;

  s1 = g_string_new("");

  s1 = g_string_assign(s1, "ar");
  s1 = g_string_append(s1, "gh");
  s1 = g_string_prepend(s1, "aa");
  s1 = g_string_prepend_c(s1, 'A');
  s1 = g_string_insert(s1, 4, "rr");

  g_print("%s\n", s1->str);           /* prints "Aaaarrrgh" */

  s1 = g_string_assign(s1, "Anyway");
  s1 = g_string_erase(s1, 4, 1);
  /* s1 should now be "Anywy" */
  s1 = g_string_truncate(s1, 3);

  g_print("%s\n", s1->str);           /* prints "Any" */

  g_string_free(s1, TRUE);

  return 0;
}
