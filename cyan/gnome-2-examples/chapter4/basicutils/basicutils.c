#include <gnome.h>

int main(int argc, char **argv)
{
  GnomeProgram *program;
  char *path;

  program = gnome_program_init("basicutils",
                               "0.1",
                               LIBGNOME_MODULE,
                               argc, argv,
                               GNOME_PROGRAM_STANDARD_PROPERTIES,
                               GNOME_PARAM_HUMAN_READABLE_NAME, "BasicUtils",
                               GNOME_PARAM_ENABLE_SOUND, FALSE,
                               NULL);

  /* check file with gnome_program_locate_file() */

  /* find the green apple image; it should be in the standard
     GNOME pixmap directory */
  path = gnome_program_locate_file(program,
                                   GNOME_FILE_DOMAIN_PIXMAP,
                                   "apple-green.png",
                                   TRUE,
                                   NULL);
  
  g_print("Path for green apple: %s\n", path);
  g_free(path);

  /* look for an application-specific sound file --
     do not verify that it exists */
  path = gnome_program_locate_file(program,
                                   GNOME_FILE_DOMAIN_APP_SOUND,
                                   "plop.wav",
                                   FALSE,
                                   NULL);
  g_print("Alleged path for plop sound: %s\n", path);
  g_free(path);

  /* look for the same application-specific sound file --
     this time, return a path only if the file exists */
  path = gnome_program_locate_file(program,
                                   GNOME_FILE_DOMAIN_APP_SOUND,
                                   "plop.wav",
                                   TRUE,
                                   NULL);

  if (path)
  {
     g_print(" ... the actual location is %s.\n", path);
     g_free(path);
  } else {
     g_print(" ... however, plop.wav isn't there.\n");
  }

  g_print ("\n");

  /* directory prepend functions */
  path = gnome_util_prepend_user_home("pictures/house.png");
  g_print("Path for house.png: %s\n", path);
  g_free(path);

  path = gnome_util_home_file("mega-app/config");
  g_print("Path for mega-app configuration: %s\n", path);
  g_free(path);

  path = gnome_util_prepend_user_home("blah/house.png");
  g_print("suffix for %s: %s\n", path, g_extension_pointer(path));
  g_free(path);

  /* you'd call this if this program actually displayed a window */
  /* (and you would need change the init to LIBGNOMEUI_MODULE) */
  /* gtk_main(); */

  exit(0);
}
