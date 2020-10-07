#include <config.h>

#include <gnome.h>

const gchar *str_array[] =
{
  N_("egg"),
  N_("miracle"),
  N_("drill"),
};

int main(int argc, char **argv)
{
  int i = 0;
  g_print(N_("strings:\n"));	
  for (i = 0; i < 3; i++)
  {
     g_print("%s\n", _(str_array[i]));
  }
}

