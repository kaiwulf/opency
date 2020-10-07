/* -*-coding: utf-8;-*- */
/* gconflister.c -- lists contents in a GConf database */

#include <gnome.h>
#include <gconf/gconf-client.h>

/* the path to list */
#define PATH "/"

/* recursively list the entries in the GConf database under path */
void list_contents(GConfClient *client, gchar *path, gint depth)
{
  GSList *subdirs = NULL, *entries = NULL, *p;
  gint i;
  gchar *value_str;

  /* make sure that this is a directory */
  if (!gconf_client_dir_exists(client, path, NULL))
  {
     g_printerr("%s: No such GConf directory in database\n", path);
     return;
  }

  /* extract all subdirectories in path */
  subdirs = gconf_client_all_dirs(client, path, NULL);

  /* extract all entries in path */
  entries = gconf_client_all_entries(client, path, NULL);

  /* depth is the current directory depth; print spaces before the output */
  if (depth)
  {
     for (i=0; i < depth; i++)
     {
        g_print(" ");
     }
  }

  /* print directory name */
  g_print("%s:\n", path);

  /* print the subdirectory contents */
  if (subdirs)
  {
     for (p = subdirs; p != NULL; p = g_slist_next(p))
     {
        list_contents(client, p->data, depth+1);
        g_free(p->data);
     }
     g_slist_free(subdirs);
  }

  /* print all entries in the directory */
  if (entries)
  {
     for (p = entries; p != NULL; p = g_slist_next(p))
     {
        /* if there is no value behind a key, print the value as NULL,
           otherwise, format the value a diagnostic function */
        if (!gconf_entry_get_value(p->data))
        {
           value_str = g_strdup("NULL");
        } else {
           value_str =
             gconf_value_to_string(gconf_entry_get_value((GConfEntry*)(p->data)));
        }

        /* indent the output */
        for (i=0; i < (depth+1); i++)
        {
           g_print(" ");
        }

       /* print the key and formatted value */
        g_print("%s = %s\n",
                gconf_entry_get_key((GConfEntry*)(p->data)), value_str);

        g_free(value_str);
        gconf_entry_free(p->data);
     }
     g_print("\n");
     g_slist_free(entries);
  }
}

int main(int argc, char **argv)
{
  GnomeProgram *program;
  GConfClient *client;

  program = gnome_program_init("gconflister", "0.1",
                               LIBGNOMEUI_MODULE,
                               argc, argv,
                               GNOME_PROGRAM_STANDARD_PROPERTIES,
                               GNOME_PARAM_HUMAN_READABLE_NAME, "GConfLister",
                               GNOME_PARAM_ENABLE_SOUND, TRUE,
                               NULL);

  /* create client object */
  client = gconf_client_get_default();
  gconf_client_set_error_handling(client, GCONF_CLIENT_HANDLE_UNRETURNED);

  /* list everything in PATH */
  list_contents(client, PATH, 0);

  /* finalize client object */
  g_object_unref(client);
  
  return 0;
}
