/* gtimerdemo.c -- demonstration of GTimer */

#include <glib.h>

#define DURATION 200000

int main(int argc, char **argv)
{
  GTimer *clock = NULL;
  gint i;
  gdouble elapsed_time;
  gulong us;  /* microseconds */

  clock = g_timer_new();
  g_timer_start(clock);
  g_print("Timer started.\n");

  g_print("Loop started.. ");
  for (i = 0; i < DURATION; i++) { ; }
  /* wasting time like this is only allowed in programming examples */

  g_print("and finished.\n");
  g_timer_stop(clock);
  g_print("Timer stopped.\n");
  
  elapsed_time = g_timer_elapsed(clock, &us);
  g_print("Elapsed: %g s\n", elapsed_time);
  g_print("         %ld us\n", us);

  g_timer_destroy(clock);

  return 0;
}
