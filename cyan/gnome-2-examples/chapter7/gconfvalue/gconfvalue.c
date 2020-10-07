/* confvalue.c -- demonstrate GConfValue */

#include <glib.h>
#include <gconf/gconf-client.h>

int main(int argc, char **argv)
{
  GConfValue *conf_value;
  GSList *list;
  gchar *string;

  /* create a value */
  conf_value = gconf_value_new(GCONF_VALUE_INT);

  /* set a value */
  gconf_value_set_int(conf_value, 42);

  /* check the value's type */
  switch (conf_value->type)
  {
     case GCONF_VALUE_INT: 
	g_print("conf_value is an integer.\n");
	break;
     case GCONF_VALUE_STRING: 
	g_print("conf_value is a string.\n");
	break;
     default: 
	g_print("conf_value is not an integer or a string.\n");
	break;
  }

  /* print the value (by means of access function) */
  g_print("conf_value = %d\n", gconf_value_get_int(conf_value));

  /* print with diagnostic function */
  string = gconf_value_to_string(conf_value);
  g_print("conf_value = %s\n", string);
  g_free(string);

  /* deallocate */
  gconf_value_free(conf_value);

  /* create a list */
  list = NULL;
  conf_value = gconf_value_new(GCONF_VALUE_STRING);
  gconf_value_set_string(conf_value, "yesterday");
  list = g_slist_append(list, conf_value);

  conf_value = gconf_value_new(GCONF_VALUE_STRING);
  gconf_value_set_string(conf_value, "today");
  list = g_slist_append(list, conf_value);

  conf_value = gconf_value_new(GCONF_VALUE_STRING);
  gconf_value_set_string(conf_value, "tomorrow");
  list = g_slist_append(list, conf_value);

  /* put the list into a GConfValue (conf_list_value) */
  conf_value = gconf_value_new(GCONF_VALUE_LIST);
  gconf_value_set_list_type(conf_value, GCONF_VALUE_STRING);
  gconf_value_set_list_nocopy(conf_value, list);

  /* print string with diagnostic function */
  string = gconf_value_to_string(conf_value);
  g_print("conf_value = %s\n", string);
  g_free(string);

  gconf_value_free(conf_value);

  return 0;
}
