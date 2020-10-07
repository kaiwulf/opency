/* Glib 2.0 GMemChunk demo */

#include <glib.h>
#include <stdio.h>

typedef struct _footype {
  gchar flat[50];
  gchar foot[50];
  gchar floogee[256];
} footype;

int main(int argc, char **argv)
{
  GMemChunk *my_chunk;
  GMemChunk *pile_of_mem;
  gchar *data[50000];
  gint i;  
 
  my_chunk = g_mem_chunk_new("My Chunk", /* diagnostic name */
                             42,         /* size of an atom (bytes) */
                             42*16,      /* size of the entire block  */
                             G_ALLOC_AND_FREE);     /* access mode */
  g_mem_chunk_print(my_chunk);

  /* allocate 40,000 atoms */  
  for(i=0; i<40000; i++)
  {
     data[i] = g_mem_chunk_alloc(my_chunk);
  }
  g_mem_chunk_print(my_chunk);
  
  /* allocate 10,000 more atom and initialize them */
  for(i=40000; i<50000; i++)
  {
     data[i] = g_mem_chunk_alloc0(my_chunk);
  }
  g_mem_chunk_print(my_chunk);
  
  /* free one atom */
  g_mem_chunk_free(my_chunk, data[42]);
  g_mem_chunk_print(my_chunk);

  pile_of_mem = g_mem_chunk_create(footype, 128, G_ALLOC_AND_FREE);
  
  g_mem_chunk_info();

  return 0;
}
