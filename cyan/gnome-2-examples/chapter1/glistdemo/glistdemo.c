/* Glib 2.0 demo */

#include <glib.h>

void print_number(gpointer data_ptr, gpointer ignored)
{
  /* cast a pointer to a gint and print that value */
  g_print("%d ", *(gint *)data_ptr);
}

void print_list(GList *list)
{
  if (list == NULL) {
     g_print("List empty!\n\n");
  } else {
     g_list_foreach(list, print_number, NULL);
     g_print("\n\n");
   }
}

void plus(gpointer data_ptr, gpointer addend_ptr)
{
  *(gint *)data_ptr += *(gint *)addend_ptr;
}

void free_element(gpointer data_ptr, gpointer ignored)
{
  g_free(data_ptr);
}

gint gint_compare(gconstpointer ptr_a, gconstpointer ptr_b)
{
  gint a, b;
  a = *(gint *)ptr_a;
  b = *(gint *)ptr_b;

  if (a > b)  { return (1); }
  if (a == b) { return (0); }
  /* default: a < b */
                return (-1);
}

int main(int argc, char **argv)
{
  GList *list = NULL;
  GList *list2 = NULL;
  GList *orig_list = NULL;
  GList *ptr, *tmp;
  gint i, num;
  gint *data_ptr, *num_ptr;

  g_print("Initializing list...\n");
  for (i=0; i<10; i++)
  {
     data_ptr = g_new(gint, 1);
     *data_ptr = i;
     list = g_list_append(list, (gpointer)data_ptr);
  }
  print_list(list);

  g_print("Append three more elements...\n");
  for (i=0; i<3; i++)
  {
     data_ptr = g_new(gint, 1);
     *data_ptr = i;
     list = g_list_append(list, (gpointer)data_ptr);
  }
  print_list(list);

  g_print("Reverse the list...\n");
  list = g_list_reverse(list);
  print_list(list);

  g_print("Add 42 to each element...\n");
  num = 42;
  num_ptr = (gpointer)&num;
  g_list_foreach(list, plus, num_ptr);
  print_list(list);

  g_print("Subtract 65 from all elements...\n");
  num = -65;
  num_ptr = (gpointer)&num;
  g_list_foreach(list, plus, num_ptr);
  print_list(list);

  g_print("Sort the list...\n");
  list = g_list_sort(list, gint_compare);
  print_list(list);

  g_print("Insert 2003 after the third element...\n");
  data_ptr = g_new(gint, 1);
  *data_ptr = 2003;
  list = g_list_insert(list, data_ptr, 3);
  print_list(list);

  g_print("Find previously-inserted element and insert 2000 before it...\n");
  tmp = g_list_find(list, data_ptr);
  data_ptr = g_new(gint, 1);
  *data_ptr = 2000;
  list = g_list_insert_before(list, tmp, data_ptr);
  print_list(list);

  g_print("Pick out element at position 4 (the fifth) and print it...\n");
  tmp = g_list_nth(list, 4);
  g_print("Value: %d\n\n", *(gint *)(tmp->data));

  g_print("Remove element at position 3...\n");
  g_free(g_list_nth_data(list, 3));
  list = g_list_remove(list, g_list_nth_data(list, 3));
  print_list(list);


  g_print("Insert three 380s at the beginning of the list...\n");
  g_print("(note that they will share the same data pointer)\n");
  data_ptr = g_new(gint, 1);
  *data_ptr = 380;
  list = g_list_prepend(list, (gpointer)data_ptr);
  list = g_list_prepend(list, (gpointer)data_ptr);
  list = g_list_prepend(list, (gpointer)data_ptr);
  print_list(list);

  g_print("Set the first three and last elements to 42...\n");
  ptr = g_list_first(list);
  *(gint *)(ptr->data) = 42;
  ptr = g_list_last(list);
  *(gint *)(ptr->data) = 42;
  print_list(list);
  
  g_print("Remove first instance of 42...\n");
  list = g_list_remove(list, (gconstpointer)data_ptr);
  print_list(list);
  
  g_print("Remove the other two (sharing the same memory)...\n");
  list = g_list_remove_all(list, (gconstpointer)data_ptr);
  g_free(data_ptr);
  print_list(list);

  /* back up the old list */
  orig_list = g_list_copy(list);

  g_print("Append a copy of the list to itself...\n");
  list2 = g_list_copy(list);
  list = g_list_concat(list, list2);
  list2 = NULL;
  print_list(list);

  /* notice that the copy doesn't include data blocks
  g_print("Be mean...\n");
  ptr = g_list_first(list);
  *(gint *)(ptr->data) = 9999;
  print_list(list);
  */

  g_print("Reverse the list again...\n");
  list = g_list_reverse(list);
  print_list(list);
  
  g_print("Freeing lists.\n");
  /* this has no original data */
  g_list_free(list);

  /* orig_list contains all of the original data */
  g_list_foreach(orig_list, free_element, NULL);
  g_list_free(orig_list);

  return 0;
}
