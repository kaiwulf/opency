/* messagedemo.c -- show logging features */

#include <glib.h>

#define NR 42

int main(int argc, char **argv)
{
  g_message("Coffee preparation engaged");
  g_warning("Bean canister #%d empty", NR);
  g_critical("Water flow failure");
  g_error("Heating element incinerated");

  /* this program shouldn't reach this point */
  return 0;
}
