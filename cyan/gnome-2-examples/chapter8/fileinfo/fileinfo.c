/* -*-coding: utf-8;-*- */
/* file.c -- retrieve and summarize file information */

#include <string.h> /* for strlen() */
#include <time.h> /* for ctime() */
#include <libgnome/libgnome.h>
#include <libgnomevfs/gnome-vfs.h>
#include <libgnomevfs/gnome-vfs-mime-handlers.h>

#ifdef LINUX
# include <linux/fs.h> /* for MAJOR() und MINOR() */
#endif /* LINUX */

int print_file_error(GnomeVFSResult code, const gchar *uri_string)
{
  const gchar *error_str;

  error_str = gnome_vfs_result_to_string(code);
  g_printerr("%s: file error (%d): %s\n", uri_string, code, error_str);

  return 1;
}

int main(int argc, char **argv)
{
  gchar *file_uri_str;
  GnomeVFSResult result;
  GnomeVFSFileInfo *info;
  GnomeVFSMimeApplication *application;
  gchar *icon_name, *icon_name_tmp, *icon_path, *tmp;

  if (argc != 2)
  {
     g_printerr("Usage: %s <uri>\n", argv[0]);
     exit (1);
  }

  /* initialize GnomeVFS */
  if (!gnome_vfs_init())
  {
     g_error("%s: could not initialize GnomeVFS\n", argv[0]);
     exit (2);
  }

  /* determine URI */
  file_uri_str = argv[1];

  /* load a new file info structure with information about the URI */
  info = gnome_vfs_file_info_new();
  result = gnome_vfs_get_file_info(file_uri_str, info,
                                   GNOME_VFS_FILE_INFO_GET_MIME_TYPE);
  if (result != GNOME_VFS_OK)
  {
     exit(print_file_error(result, file_uri_str));
  }

  g_print("URI: %s\n", file_uri_str);
  g_print("base name: %s ", info->name);
  
  if (info->valid_fields & GNOME_VFS_FILE_INFO_FIELDS_TYPE)
  {
     switch (info->type) {
        case GNOME_VFS_FILE_TYPE_UNKNOWN:
           g_print("(unknown type)\n"); break;

        case GNOME_VFS_FILE_TYPE_REGULAR:
            g_print("(normal file)\n"); break;

        case GNOME_VFS_FILE_TYPE_DIRECTORY:
            g_print("(directory)\n"); break;

        case GNOME_VFS_FILE_TYPE_FIFO:
            g_print("(FIFO/named pipe)\n"); break;

        case GNOME_VFS_FILE_TYPE_SOCKET:
            g_print("(socket)\n"); break;

        case GNOME_VFS_FILE_TYPE_CHARACTER_DEVICE:
            g_print("(character device)\n"); break;

        case GNOME_VFS_FILE_TYPE_BLOCK_DEVICE:
            g_print("(block device)\n"); break;

        case GNOME_VFS_FILE_TYPE_SYMBOLIC_LINK:
            g_print("(symbolic link)\n"); break;
        }
  }

  if (info->valid_fields & GNOME_VFS_FILE_INFO_FIELDS_SYMLINK_NAME)
  {
     g_print("- symbolic link points to %s\n", info->symlink_name);
  }

  if (info->valid_fields & GNOME_VFS_FILE_INFO_FIELDS_PERMISSIONS)
  {
     g_print("- Permissions: 0%o\n", info->permissions);
  }

  if (info->valid_fields & GNOME_VFS_FILE_INFO_FIELDS_FLAGS)
  {
     if (info->flags & GNOME_VFS_FILE_FLAGS_LOCAL)
     {
        g_print("- is on a local file system");

#ifdef LINUX
        if (info->valid_fields & GNOME_VFS_FILE_INFO_FIELDS_DEVICE)
        {
           g_print(" (device %d, %d)",
                   (int)MAJOR(info->device), (int)MINOR(info->device));
        }
#endif /* LINUX */
        if (info->valid_fields & GNOME_VFS_FILE_INFO_FIELDS_INODE)
        {
           g_print(" (inode %"\
GNOME_VFS_SIZE_FORMAT_STR")", info->inode);
        }
     }
     g_print("\n");
  }
  if (info->valid_fields & GNOME_VFS_FILE_INFO_FIELDS_LINK_COUNT)
  {
     g_print("- %d link%s\n",
	     info->link_count, (info->link_count == 1) ? "" : "s");
  }

  g_print("- user id (UID): %d\n",info->uid);
  g_print("- group id (GID): %d\n", info->gid);

  if (info->valid_fields & GNOME_VFS_FILE_INFO_FIELDS_SIZE)
  {
     tmp = gnome_vfs_format_file_size_for_display(info->size);
     g_print("- size: %s ", tmp);
     g_free(tmp);
  }

  if (info->valid_fields & GNOME_VFS_FILE_INFO_FIELDS_BLOCK_COUNT)
  {
     g_print(" (%"\
GNOME_VFS_SIZE_FORMAT_STR" 512-byte blocks)\n", info->block_count);
  } else {
     g_print("\n");
  }

  if (info->valid_fields & GNOME_VFS_FILE_INFO_FIELDS_IO_BLOCK_SIZE)
  {
     g_print("- optimal read/write buffer: %d bytes\n", info->io_block_size);
  }

  if (info->valid_fields & GNOME_VFS_FILE_INFO_FIELDS_ATIME)
  {
     g_print("- last access:       %s", ctime(&(info->atime)));
  }
  if (info->valid_fields & GNOME_VFS_FILE_INFO_FIELDS_MTIME)
  {
     g_print("- last modification: %s", ctime(&(info->mtime)));
  }
  if (info->valid_fields & GNOME_VFS_FILE_INFO_FIELDS_CTIME)
  {
     g_print("- last change:       %s", ctime(&(info->ctime)));
  }

  if (info->valid_fields & GNOME_VFS_FILE_INFO_FIELDS_MIME_TYPE)
  {
     g_print("- MIME type: %s (%s)\n", info->mime_type,
             gnome_vfs_mime_get_description(info->mime_type));

     switch (gnome_vfs_mime_get_default_action_type(info->mime_type))
     {
        case GNOME_VFS_MIME_ACTION_TYPE_NONE:
           g_print("- no default action\n");
           break;

        case GNOME_VFS_MIME_ACTION_TYPE_APPLICATION:
           application =
              gnome_vfs_mime_get_default_application(info->mime_type);
           g_print("- default action: open with application %s\n",
                   application? application->name : "");
           gnome_vfs_mime_application_free(application);
           break;

        case GNOME_VFS_MIME_ACTION_TYPE_COMPONENT:
           g_print("- default action: open with a component\n");
           break;
     }

     /* somewhat roundabout way of finding an icon */
     icon_name = g_strdup(gnome_vfs_mime_get_icon(info->mime_type));
     if (icon_name)
     {
        g_print("- icon name: %s\n", icon_name);

        icon_path = gnome_vfs_icon_path_from_filename(icon_name);

        /* path doesn't exist? try with .png extension */
        if (!icon_path)
        {
           icon_name_tmp = g_strconcat(icon_name, ".png", NULL);
           icon_path = gnome_vfs_icon_path_from_filename(icon_name_tmp);
           g_free(icon_name_tmp);
        }

        if (icon_path)
        {
           g_print("- icon path: %s\n", icon_path);
           g_free(icon_path);
        }
        g_free(icon_name);
     }
  }

  /* clean up GnomeVFS, exit */
  gnome_vfs_file_info_unref(info);
  gnome_vfs_shutdown();

  exit (0);
}
