/* -*-coding: utf-8;-*- */
/* asyncviewer.c -- asynchronous GNOME VFS file viewer */

#include <gnome.h>
#include <libgnomeui/gnome-window.h>
#include <libgnomevfs/gnome-vfs.h>

#define MAX_BLOCKSIZE (8 * 1024)
#define MAX_PATHLEN (8 * 1024)
#define PRIORITY GNOME_VFS_PRIORITY_DEFAULT

/* information about a (current) file load operation */
typedef struct _LoadInfo {
  gchar *prog_name;
  gchar current_dir[MAX_PATHLEN];    /* current working directory */
  gchar *uri_string;
  GnomeVFSAsyncHandle *fd;
  guint block_size;
  GnomeVFSFileSize total_size;
  GnomeVFSFileSize bytes_read;
  GtkWindow *window;
  GtkTextBuffer *text_buffer;
  GtkProgressBar *progress_bar;
  GnomeFileEntry *file_entrybox;
  GtkButton *open_button;
  GtkButton *close_button;
  GtkButton *stop_button;

  /* initially TRUE; error detection changes this to FALSE */
  gboolean status_normal;
} LoadInfo;

/* Report an error with string (can be a URI or otherwise). If result is a
   valid GnomeVFSResult, display the string error representation; otherwise,
   show a custom message. */
void report_error(gchar *name, gchar *desc, GnomeVFSResult result, GtkWindow *win)
{
  GtkDialog *dialog;
  gchar *err_title, *err_message;

  if (name)
  {
     err_title = g_strdup_printf("File access error:\n %s: ", name);
  } else {
     err_title = g_strdup_printf("File access error:\n");
  }

  if (result < GNOME_VFS_NUM_ERRORS)
  {
     err_message = g_strdup(gnome_vfs_result_to_string(result));
  } else {
     err_message = g_strdup_printf("%s\n", desc ? desc : "");
  }
  dialog = GTK_DIALOG(gtk_message_dialog_new(win,
                                             GTK_DIALOG_DESTROY_WITH_PARENT,
                                             GTK_MESSAGE_ERROR,
                                             GTK_BUTTONS_CLOSE,
                                             "%s %s", err_title, err_message));
  g_signal_connect_swapped(dialog,
                           "response", G_CALLBACK (gtk_widget_destroy), dialog);
  g_object_set(dialog, "title", "", NULL);
  gtk_widget_show_all(GTK_WIDGET(dialog));
  g_free(err_title);
  g_free(err_message);
}

/* reset widgets after a file load */
void finish_file_load(LoadInfo *load_info)
{
  /* update progress bar; clear if the load was completely successful,
     clear the status bar. Otherwise, show a warning message. */
  if (load_info->status_normal)
  {
     g_object_set(load_info->progress_bar,
		  "text", "",
                  "fraction", 0.0,  NULL);
  } else {
     g_object_set(load_info->progress_bar,
                  "text", "Warning: File view incomplete",
                  "fraction", 0.0,  NULL);
  }

  /* disable the stop button, enable close and open buttons */
  g_object_set(load_info->stop_button,   "sensitive", FALSE,  NULL);
  g_object_set(load_info->close_button,  "sensitive", TRUE,   NULL);
  g_object_set(load_info->open_button,   "sensitive", TRUE,   NULL);
  g_object_set(load_info->file_entrybox, "sensitive", TRUE,   NULL);
}

/* callback (GnomeVFSAsyncCloseCallback) for a file close operation */
void file_closed(GnomeVFSAsyncHandle *fd, GnomeVFSResult result, gpointer li_ptr)
{
  LoadInfo *load_info = (LoadInfo*)li_ptr;

  if (result != GNOME_VFS_OK)
  {
     report_error(load_info->uri_string, NULL, result, load_info->window);
  }

  finish_file_load(load_info);
}

/* handler for when the user clicks the "stop" button */
void stop_load(GtkButton *knopf, gpointer li_ptr)
{
  LoadInfo *load_info = (LoadInfo*)li_ptr;

  gnome_vfs_async_cancel(load_info->fd);
  load_info->status_normal = FALSE;
  gnome_vfs_async_close(load_info->fd, file_closed, li_ptr);
}

/* callback (GnomeVFSAsyncReadCallback) to move the bytes in a buffer to
   the text buffer inside the application window; reports the progress. */
void block_to_text_buffer(GnomeVFSAsyncHandle *fd,
                          GnomeVFSResult result,
                          gpointer buffer,
                          GnomeVFSFileSize bytes_requested,
                          GnomeVFSFileSize bytes_received,
                          gpointer load_info_ptr)
{
  LoadInfo *load_info = (LoadInfo*)load_info_ptr;
  GtkTextIter buffer_end;
  gdouble fraction_complete;
  gchar *progress_text;

  /* if there's an error, report the error, set the error flag, close file,
     and deallocate buffer */
  if ((result != GNOME_VFS_OK) && (result != GNOME_VFS_ERROR_EOF))
  {
     report_error(load_info->uri_string, NULL, result, load_info->window);
     load_info->status_normal = FALSE;
     gnome_vfs_async_close(load_info->fd, file_closed, load_info_ptr);
     g_free(buffer);
     return;
  }

  /* insert read operation buffer contents into text buffer */
  gtk_text_buffer_get_end_iter(load_info->text_buffer, &buffer_end);
  gtk_text_buffer_insert_with_tags_by_name(
     load_info->text_buffer, &buffer_end, buffer, bytes_received, "mono", NULL);

  /* update the fraction complete and progress bar */
  load_info->bytes_read += bytes_received;
  if (load_info->total_size != 0)
  {
     fraction_complete = (gdouble)(load_info->bytes_read) /
                         (gdouble)(load_info->total_size);
     fraction_complete = CLAMP(fraction_complete, 0.0, 0.999);
     progress_text = g_strdup_printf("%3.0f%% complete",
                                     fraction_complete * 100);
     g_object_set(load_info->progress_bar,
                  "fraction", fraction_complete,
                  "text", progress_text,
                  NULL);
     g_free(progress_text);
  }

  /* if this isn't the end of the file, read another block */
  if (result != GNOME_VFS_ERROR_EOF)
  {
     gnome_vfs_async_read(
        fd, buffer, bytes_requested, block_to_text_buffer, load_info_ptr);
  } else {
     g_free(buffer);
     gnome_vfs_async_close(fd, file_closed, load_info_ptr);
     load_info->status_normal = TRUE;
  }
}

/* callback (GnomeVFSAsyncOpenCallback) for completion of file open;
   initiates the first block read from the file */
void file_opened(GnomeVFSAsyncHandle *fd, GnomeVFSResult result, gpointer li_ptr)
{
  LoadInfo *load_info = (LoadInfo*)li_ptr;
  GnomeVFSURI *uri_tmp;
  gchar *short_name;
  gpointer buffer;

  /* report error and reset buttons when GnomeVFS can't open file  */
  if (result != GNOME_VFS_OK)
  {
     report_error(load_info->uri_string, NULL, result, load_info->window);
     load_info->status_normal = FALSE;
     finish_file_load(load_info);
     return;
  }

  /* extract file's short name and include this in window's title */
  uri_tmp = gnome_vfs_uri_new(load_info->uri_string);
  short_name = gnome_vfs_uri_extract_short_name(uri_tmp);
  gnome_vfs_uri_unref(uri_tmp);
  gnome_window_toplevel_set_title(load_info->window, short_name, "AsyncViewer",
                                  NULL);
  g_free(short_name);

  /* create a buffer and read a block from the file */
  buffer = g_malloc(load_info->block_size);
  gnome_vfs_async_read(
     fd, buffer, load_info->block_size, block_to_text_buffer, li_ptr);
}

/* handler for "Open" button - try to open the file */
void open_file(GtkButton *button, gpointer li_ptr)
{
  LoadInfo *load_info = (LoadInfo*)li_ptr;
  gchar *path;
  GtkTextIter text_begin, text_end;
  GnomeVFSResult result;
  GnomeVFSFileInfo *file_stats;
  GnomeVFSAsyncHandle *fd;

  /* clear text buffer */
  gtk_text_buffer_get_bounds(load_info->text_buffer, &text_begin, &text_end);
  gtk_text_buffer_delete(load_info->text_buffer, &text_begin, &text_end);

  /* free any pre-existing URI string */
  if (load_info->uri_string)
  {
     g_free(load_info->uri_string);
     load_info->uri_string = NULL;
  }

  /* extract URI from file entry box; return error when file does not exist */
  path = gnome_file_entry_get_full_path(load_info->file_entrybox, TRUE);
  load_info->uri_string = gnome_vfs_get_uri_from_local_path(path);
  if (path)
  {
    g_free(path);
  }
  if (!load_info->uri_string)
  {
     g_object_get(load_info->file_entrybox, "filename", &path, NULL);
     report_error(path, "File not found", GNOME_VFS_NUM_ERRORS,
                  load_info->window);
     g_free(path);
     return;
  }

  /* get the file information */
  file_stats = gnome_vfs_file_info_new();
  result = gnome_vfs_get_file_info(load_info->uri_string,
                                file_stats, GNOME_VFS_FILE_INFO_FOLLOW_LINKS);
  if (result != GNOME_VFS_OK)
  {
     report_error(load_info->uri_string, NULL, result, load_info->window);
     return;
  }

  /* determine buffer block size: optimal block size or MAX_BLOCKSIZE */
  if (file_stats->valid_fields & GNOME_VFS_FILE_INFO_FIELDS_IO_BLOCK_SIZE)
  {
     load_info->block_size = file_stats->io_block_size;
  } else {
     load_info->block_size = MAX_BLOCKSIZE;
  }

  /* determine file size */
  if (file_stats->valid_fields & GNOME_VFS_FILE_INFO_FIELDS_SIZE)
  {
     load_info->total_size = file_stats->size;
  } else {
     load_info->total_size = 0;
  }

  gnome_vfs_file_info_unref(file_stats);

  /* reset the number of bytes already read */
  load_info->bytes_read = 0;

  /* open file and set file descriptor in load_info */
  gnome_vfs_async_open(&fd, load_info->uri_string, GNOME_VFS_OPEN_READ,
                       PRIORITY, file_opened, load_info);  
  load_info->fd = fd;

  /* enable cancel; disable open and close */
  g_object_set(load_info->stop_button,   "sensitive", TRUE,   NULL);
  g_object_set(load_info->close_button,  "sensitive", FALSE,  NULL);
  g_object_set(load_info->open_button,   "sensitive", FALSE,  NULL);
  g_object_set(load_info->file_entrybox, "sensitive", FALSE,  NULL);
}

/* the handler for the file entry box; "clicks" the Open button */
void file_entered(GnomeFileEntry *entry, gpointer li_ptr)
{
  LoadInfo *load_info = (LoadInfo*)li_ptr;
  g_signal_emit_by_name(load_info->open_button, "activate");
}

/* standard window event handlers */
gint delete_event(GtkWidget *widget, GdkEvent event, gpointer data)
{
  return FALSE;
}

void end_program(GtkWidget *widget, gpointer li_ptr)
{
  LoadInfo *load_info = (LoadInfo*)li_ptr;
  gboolean still_loading_file;

  /* the application is still reading if the Cancel button is enabled;
     stop the load if that's the case */
  g_object_get(load_info->stop_button, "sensitive", &still_loading_file, NULL);
  if (still_loading_file)
  {
    stop_load(load_info->stop_button, li_ptr);
  }

  /* quit the main loop */
  gtk_main_quit();
}

/* handler for session manager's "save-yourself" request */
gboolean save_yourself(GnomeClient *client, int phase,
                       GnomeSaveStyle what,
                       gboolean end,
                       GnomeInteractStyle interaction,
                       gboolean fast,
                       gpointer li_ptr)
{
  LoadInfo *load_info = (LoadInfo*)li_ptr;
  gchar *argv[3];
  gchar *filename;

  /* determine the current directory */
  gnome_client_set_current_directory(client, load_info->current_dir);

  /* transmit a command for a new instance */
  argv[0] = g_strdup(load_info->prog_name);
  gnome_client_set_clone_command(client, 1, argv);

  /* transmit command for restarting this command with the current file */
  if (load_info->uri_string != NULL)
  {
     filename = gnome_vfs_get_local_path_from_uri(load_info->uri_string);
  }
  if (filename != NULL)
  {
     argv[1] = "-f";
     argv[2] = filename;
     gnome_client_set_restart_command(client, 3, argv);
  } else {
     gnome_client_set_restart_command(client, 1, argv);
  }
  return TRUE;
}

/* signal handler for termination via session manager */
void die(GnomeClient *client, gpointer data)
{
  end_program(NULL, data);
}

int main(int argc, char **argv)
{
  GnomeProgram *program;
  GnomeClient *client;
  GnomeFileEntry *file_entrybox;
  GtkLabel *file_label;
  LoadInfo load_info;
  GtkTextBuffer *text_buffer;
  GtkTextView *text_view;
  GtkScrolledWindow *text_window;
  GtkProgressBar *progress_bar;
  GtkButton *stop_button, *close_button, *open_button;
  GtkHBox *hbox, *hbox1;
  GtkVBox *vbox;
  GtkWindow *window;
  gint i;

  program = gnome_program_init("asyncviewer", "0.1", LIBGNOMEUI_MODULE,
                               argc, argv,
                               GNOME_PROGRAM_STANDARD_PROPERTIES,
                               GNOME_PARAM_HUMAN_READABLE_NAME, "AsyncViewer",
                               GNOME_PARAM_ENABLE_SOUND, TRUE,
                               NULL);

  /* initialize session management */
  client = gnome_master_client();

  /* bind session management handlers */
  g_signal_connect(client,
                   "save_yourself", G_CALLBACK(save_yourself), &load_info);
  g_signal_connect(client, "die", G_CALLBACK(die), &load_info);

  /* create text view and buffer; put view into a scrolled window */
  text_buffer = gtk_text_buffer_new(NULL);
  gtk_text_buffer_create_tag(text_buffer, "mono", "family", "Courier", NULL);
  text_view = GTK_TEXT_VIEW(gtk_text_view_new_with_buffer(text_buffer));

  g_object_set(text_view,
               "wrap-mode", GTK_WRAP_CHAR,
               "editable", FALSE,
               "cursor-visible", FALSE,
               NULL);

  text_window = g_object_new(GTK_TYPE_SCROLLED_WINDOW,
                             "hscrollbar-policy", GTK_POLICY_AUTOMATIC,
                             "vscrollbar-policy", GTK_POLICY_ALWAYS,
                             NULL);

  gtk_container_add(GTK_CONTAINER(text_window), GTK_WIDGET(text_view));

  /* create progress bar, stop/close buttons */
  progress_bar = g_object_new(GTK_TYPE_PROGRESS_BAR,
		              "text", "Choose a file and click Open",
                              NULL);

  stop_button = g_object_new(GTK_TYPE_BUTTON,
                              "label", GTK_STOCK_STOP,
                              "use-stock", TRUE,
                              "sensitive", FALSE,
                              NULL);

  close_button = g_object_new(GTK_TYPE_BUTTON,
                              "label", GTK_STOCK_CLOSE,
                              "use-stock", TRUE,
                              NULL);

  hbox = g_object_new(GTK_TYPE_HBOX, "border-width", 6,  "spacing", 6,  NULL);
  gtk_box_pack_start_defaults(GTK_BOX(hbox), GTK_WIDGET(progress_bar));
  gtk_box_pack_start(GTK_BOX(hbox), GTK_WIDGET(stop_button), FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(hbox), GTK_WIDGET(close_button), FALSE, FALSE, 0);

  /* file entry box with Open button */
  file_entrybox = g_object_new(GNOME_TYPE_FILE_ENTRY,
                               "history-id", "filename",
                               "browse-dialog-title", "Choose a File",
                               NULL);

  file_label = g_object_new(GTK_TYPE_LABEL,
                           "label", "_File:",
                           "use-underline", TRUE,
                           "mnemonic-widget", file_entrybox,
                           NULL);

  open_button = g_object_new(GTK_TYPE_BUTTON,
                             "label", GTK_STOCK_OPEN,
                             "use-stock", TRUE,
                             NULL);

  hbox1 = g_object_new(GTK_TYPE_HBOX, "border-width", 6,  "spacing", 6,  NULL);
  gtk_box_pack_start(GTK_BOX(hbox1), GTK_WIDGET(file_label), FALSE, FALSE, 0);
  gtk_box_pack_start_defaults(GTK_BOX(hbox1), GTK_WIDGET(file_entrybox));
  gtk_box_pack_start(GTK_BOX(hbox1), GTK_WIDGET(open_button), FALSE, FALSE, 0);

  vbox = g_object_new(GTK_TYPE_VBOX, NULL);
  gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(hbox1), FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(text_window), TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(hbox), FALSE, FALSE, 0);

  /* create and fill window */
  window = g_object_new(GTK_TYPE_WINDOW,
                        "default-height", 500,
                        "default-width", 400,
                        "title", "AsyncViewer",
                        NULL);

  gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(vbox));

  /* write widgets into LoadInfo structure so that the callbacks can
     access the widgets */
  load_info.window        = window;
  load_info.text_buffer   = text_buffer;
  load_info.progress_bar  = progress_bar;
  load_info.open_button   = open_button;
  load_info.close_button  = close_button;
  load_info.stop_button   = stop_button;
  load_info.file_entrybox = file_entrybox;

  /* record command for the saveyourself signal */
  load_info.prog_name = argv[0];

  if (!getcwd(load_info.current_dir, MAX_PATHLEN))
  {
     g_error("could not determine current working directory\n");
  }
  load_info.uri_string = NULL;

  /* attach all signal handlers */
  g_signal_connect(open_button, "clicked", G_CALLBACK(open_file), &load_info);
  g_signal_connect(file_entrybox,
                   "activate", G_CALLBACK(file_entered), &load_info);

  g_signal_connect(stop_button, "clicked", G_CALLBACK(stop_load), &load_info);
  g_signal_connect_swapped(close_button,
                          "clicked", G_CALLBACK(gtk_widget_destroy), window);

  g_signal_connect(window, "delete-event", G_CALLBACK(delete_event), NULL);
  g_signal_connect(window, "destroy", G_CALLBACK(end_program), &load_info);

  /* rudimentary command line parsing (use popt if you want better handling) */
  for (i=1; i < (argc-1); i++)
  {
     if (!strcmp("-f", argv[i]))
     {
        g_object_set(file_entrybox, "filename", argv[i+1], NULL);
        open_file(NULL, &load_info);
        break;
      }
  }

  /* show everything, start GTK+ main event loop */
  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_main();

  gnome_vfs_shutdown();
  return 0;
}
