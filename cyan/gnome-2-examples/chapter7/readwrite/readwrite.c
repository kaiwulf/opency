
#include <glib.h>
#include <gconf/gconf-client.h>

int main(int argc, char **argv)
{
  GConfClient *client;
  GSList *list, *l_ptr;
  gchar *name;
  gint n;

  /* initialize GConf */
  client = gconf_client_get_default();

  /* read some values */
  g_print("Your desktop has %d workspace(s).\n",
          gconf_client_get_int(client,
             "/desktop/gnome/applications/window_manager/number_of_workspaces",
             NULL));

  g_print("The desktop names are:\n");
  list = gconf_client_get_list(client, 
           "/desktop/gnome/applications/window_manager/workspace_names",
           GCONF_VALUE_STRING,
           NULL);

  for (l_ptr = list; l_ptr != NULL; l_ptr = g_slist_next(l_ptr))
  {
     g_print(" %s", (gchar *)l_ptr->data);
     g_free(l_ptr->data);
  }
  g_slist_free(list);
  g_print("\n");

  if (gconf_client_get_bool(client, "/desktop/gnome/applications/cursor_blink", NULL))
  {
     g_print("Your cursor blinks.\n");
  } else {
     g_print("Your cursor does not blink.\n");
  }

  name = gconf_client_get_string(client,
				 "/desktop/gnome/interface/gtk_theme",
                                 NULL);

  g_print("Your GTK+ theme is: %s\n", name);
  g_free(name);

  /* write some values */
  gconf_client_set_int(client, "/example/number", 42, NULL);
  gconf_client_set_float(client, "/example/pi", 3.14159, NULL);
  gconf_client_set_bool(client, "/example/state", FALSE, NULL);
  gconf_client_set_string(client, "/example/message", "Hello, World.", NULL);

  /* put a list of integers into conf value */
  list = NULL;
  list = g_slist_append(list, GINT_TO_POINTER(42));
  list = g_slist_append(list, GINT_TO_POINTER(37));
  list = g_slist_append(list, GINT_TO_POINTER(11217));
  gconf_client_set_list(client, "/example/nums", GCONF_VALUE_INT, list, NULL);
  g_slist_free(list);

  /* set a pair; a number and a string */
  n = 52;
  name = g_strdup("fifty-two");
  gconf_client_set_pair(client,
                        "/example/number_name",
                        GCONF_VALUE_INT,
                        GCONF_VALUE_STRING,
                        &n,
                        &name,
                        NULL);
  g_free(name);

  g_object_unref(client);
  return 0;
}
