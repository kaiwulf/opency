/* Glib 2.0 GTree demo */

#include <glib.h>

GMemChunk *key_chunk;

/* compare gints; ignore extra data parameter */
gint key_cmp(gconstpointer a_ptr, gconstpointer b_ptr, gpointer ignored)
{
  gint a, b;
  a = *(gint *)a_ptr;
  b = *(gint *)b_ptr;

  if (a > b)      { return (1); }
  if (a == b)   { return (0); }
  /* if a < b */  return (-1);
}

void free_key(gpointer key)
{
  g_mem_chunk_free(key_chunk, key);
}

void free_value(gpointer value) {
  g_string_free((GString *)value, TRUE);
}

/* add the keys; ignore the value */
gboolean sum_keys(gpointer key, gpointer value_ignored, gpointer sum)
{
  *(gint *)sum += *(gint*)key;
  return FALSE;
}

/* print a node in a traversal */
gboolean print_node(gpointer key, gpointer value, gpointer ignored)
{
  g_print("[%d %s] ", *(gint *)key, ((GString *)value)->str);
  return FALSE;
}

int main(int argc, char **argv)
{
  GTree *tree;
  gint *key_ptr, *key_ptr2;
  GString *value;
  gint sum = 0, i = 0, j = 0, k;

  /* prepare memory for keys and values */

  key_chunk = g_mem_chunk_create(gint, 1024, G_ALLOC_AND_FREE);

  /* create tree */
  tree = g_tree_new_full(key_cmp,
                         NULL,      /* data pointer, optional */
                         free_key,
                         free_value);

  /* insert 42 into the tree */
  key_ptr = g_chunk_new(gint, key_chunk);
  *key_ptr = 42;
  value = g_string_new("forty-two");
  g_tree_insert(tree, key_ptr, value);

  /* look up 37 in the tree */
  key_ptr = g_chunk_new(gint, key_chunk);
  *key_ptr = 37;
  value = (GString *) g_tree_lookup(tree, key_ptr);
  if (!value)
  {
     g_print("%d not found in tree.\n", *key_ptr);
  } else {
     g_print("%d found; value: %s.\n", *key_ptr, value->str);
  }

  /* see if 42 is in there */
  *key_ptr = 42;
  if (!g_tree_lookup_extended(tree, key_ptr,
                             (gpointer)&key_ptr2,
                             (gpointer)&value))
  {
     g_print("%d not found in tree.\n", *key_ptr);
  } else {
     g_print("%d found; value: %s.\n", *key_ptr, value->str);

     /* pull something from the tree */
     g_tree_steal(tree, key_ptr2);

     /* key_ptr2 and value contain the key and value (see above)--
        now we'll throw them right back into the tree */
     g_tree_insert(tree, key_ptr2, value);

     /* this time get rid of them for good */
     g_print("Removing 42.\n");
     g_tree_remove(tree, key_ptr2);
  }

  g_mem_chunk_free(key_chunk, key_ptr);

  sum = 0;
  /* sum the keys (should be 0) */
  g_tree_foreach(tree, sum_keys, (gpointer)&sum);
  g_print("Key sum: %d\n", sum);

  /* put some Fibonacci numbers in the tree */
  i = 1; j = 0;
  while (i < 3000)
  {
     key_ptr = g_chunk_new(gint, key_chunk);
     *key_ptr = i;
     value = g_string_new("fib");
     g_tree_insert(tree, key_ptr, value);
     k = i; i = i + j; j = k;
  }

  /* print the numbers */
  g_tree_foreach(tree, print_node, NULL);
  g_print("\n");

  sum = 0;
  /* sum the keys (should be big) */
  g_tree_foreach(tree, sum_keys, &sum);
  g_print("Key sum: %d\n", sum);

  g_print("Number of nodes: %d\n", g_tree_nnodes(tree));
  g_print("Tree height: %d\n", g_tree_height(tree));

  g_tree_destroy(tree);

  return 0;
}
