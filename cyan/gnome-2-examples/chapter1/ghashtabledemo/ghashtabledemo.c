/* ghashtabledemo.c -- GHashTable demo */

#include <glib.h>

void print_entry(gpointer key, gpointer data, gpointer user_data)
{
  /* user_data not used */
  g_print("key: %-10s     value: %-10s\n", (gchar *)key, (gchar *)data);
}

int main(int argc, char *argv[])
{
  GHashTable *hash1;
  GHashTable *hash2;
  gchar *value;
  gchar *key;
  
  /* hash table with pointers as keys */
  hash1 = g_hash_table_new(g_direct_hash, g_direct_equal);

  /* hash table with C strings as keys,
     with deallocation functions */
  hash2 = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

  /* insert a bunch of entries */
  g_hash_table_replace(hash2, g_strdup("foo"), g_strdup("bar"));
  g_hash_table_replace(hash2, g_strdup("this"), g_strdup("that"));
  g_hash_table_replace(hash2, g_strdup("the"), g_strdup("other"));
  g_hash_table_replace(hash2, g_strdup("more"), g_strdup("junk"));

  /* look up "foo" */
  value = (gchar*)g_hash_table_lookup(hash2, "foo");
  if (value)
  {
      g_print("hash2{\"foo\"} = %s\n", value);
  } else {
      g_print("foo isn't in the hash table\n");
  }

  /* get pointers into the entry for "foo" */
  if (g_hash_table_lookup_extended(hash2,
  			           "foo",
			           (gpointer)&key,
			           (gpointer)&value))
  {
     g_print("hash2{\"%s\"} = %s\n", key, value);
  } else {
     g_print("foo isn't in the hash table\n");
  }

  /* remove the "more" entry */
  g_hash_table_remove(hash2, "more");

  /* print the contents */
  g_print("Hash table entries:\n");
  g_hash_table_foreach(hash2, print_entry, NULL);

  /* print number of entries */
  g_print("\nNumber of entries: %d\n", g_hash_table_size(hash2));

  return 0;
}
 
