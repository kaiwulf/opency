/* -*-coding: utf-8;-*- */
/* vfsdemo.c -- synchronous operations */

#include <string.h>
#include <libgnome/libgnome.h>
#include <libgnomevfs/gnome-vfs.h>

/* print a VFS error code, its description, and the problem URI */
int print_error(GnomeVFSResult code, const gchar *uri)
{
  const gchar *error_desc;

  error_desc = gnome_vfs_result_to_string(code);
  g_printerr("error %d when accessing %s: %s\n", code, uri, error_desc);

  return (code);
}

const gchar filename[] = "gnome-vfsdemo-testfile";
const gchar test_text[] = "This is a test.";

int main(int argc, char **argv)
{
  gchar *file_path;
  gchar *uri;
  guint permissions;
  GnomeVFSHandle *fd;
  GnomeVFSResult result;
  GnomeVFSFileSize bytes_written;

  /* initialize GnomeVFS */
  if (!gnome_vfs_init())
  {
     g_error("could not initialize GnomeVFS\n");
  }

  /* get full pathname and URI */
  file_path = gnome_util_prepend_user_home(filename);
  g_print("filename is %s\n", file_path);

  uri = gnome_vfs_get_uri_from_local_path(file_path);
  g_print("URI is %s\n", uri);

  g_free(file_path);

  /* permissions: user has read-write access, group and other read-only */
  permissions = GNOME_VFS_PERM_USER_READ | GNOME_VFS_PERM_USER_WRITE |
                GNOME_VFS_PERM_GROUP_READ | GNOME_VFS_PERM_OTHER_READ;

  /* create and open URI */
  g_print("creating/opening URI...\n");
  result = gnome_vfs_create(&fd, uri, GNOME_VFS_OPEN_WRITE, FALSE, permissions);
  if (result != GNOME_VFS_OK)
  {
     return (print_error(result, uri));
  }
  g_print(" success.\n");

  /* write test_text into file descriptor */
  g_print("writing \"%s\" into file descriptor...\n", test_text);
  result = gnome_vfs_write(fd, test_text, strlen(test_text), &bytes_written);
  if (result != GNOME_VFS_OK)
  {
     return (print_error(result, uri));
  }
  g_print(" success. %d bytes written.\n", (guint) bytes_written);

  /* close file */
  g_print("closing file descriptor...\n");
  result = gnome_vfs_close(fd);
  if (result != GNOME_VFS_OK)
  {
     return (print_error(result, uri));
  }
  g_print(" success.\n");

  g_free(uri);

  /* shut down GnomeVFS */
  gnome_vfs_shutdown();

  return (0);
}

