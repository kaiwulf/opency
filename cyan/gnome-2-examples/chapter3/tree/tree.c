/* -*-coding: utf-8;-*- */
/* tree.c -- Rixico Inc. personnel */

#include <gtk/gtk.h>

/* employee data structure */
typedef struct _employee {
  gchar *path;
  gchar *last_name;
  gchar *first_name;
  gboolean on_site;
  gchar *job_title;
  gint room_no; /* -1 if employee is off-site */
} employee;

/* employee data */
employee staff[] = {
  { "0", "Ricshaw", "George", FALSE, "Majority Shareholder", -1 },
  { "1", "Kolakowski", "Edna", FALSE, "Shareholder", -1 },
  { "2", "Gainer", "Merideth", TRUE, "General Manager", 100 },
  { "2:0", "Zimmerman", "Walter", TRUE, "Administrative Assistant", 101 },
  { "2:1", "Fenner", "Harold", TRUE, "Accounting Director", 110 },
  { "2:1:0", "Kunkle", "Heather", TRUE, "Accountant", 111 },
  { "2:1:1", "Gasteiner", "Tom", TRUE, "Accountant", 111 },
  { "2:2", "Ardmore", "Henrietta", TRUE, "Production Director", 200 },
  { "2:2:0", "Lampoudi", "Fred", TRUE, "Flipper Lead", 210 },
  { "2:2:0:0", "Gray", "Anthony", TRUE, "Flipper Machinist", 211 },
  { "2:2:0:1", "de Vries", "Linda", TRUE, "Flipper Machinist", 211 },
  { "2:2:0:2", "Hawkins", "Drew", TRUE, "Flipper Machinist", 211 },
  { "2:2:0:3", "Wray", "Steven", TRUE, "Flipper Trainer", 212 },
  { "2:2:0:4", "Stein", "Martha", FALSE, "Flipper Trainee", 212 },
  { "2:2:1", "Sawyer", "Leonard", TRUE, "Slop Lead", 230 },
  { "2:2:1:0", "Nestroy", "Joan", TRUE, "Slop Assembler", 231 },
  { "2:2:1:1", "Parker", "Robin", FALSE, "Slop Assembler (Temp)", 232 },
  { "2:2:2", "Evering", "Tracy", TRUE, "Quality Control", 299 },
  { "2:2:3", "Zoidberg", "John", FALSE, "Company Doctor", -1 },
  { "2:3", "Stoner", "Martin", TRUE, "IT Director", 120 },
  { "2:3:0", "English", "Doug", FALSE, "Web Designer/Slacker", -1 },
  { "3", "Ledbetter", "Arthur", FALSE,
     "Research Partner, University of Cross Roads", -1},
  { NULL, NULL, NULL, FALSE, NULL, -1}
};

/* column identifiers in tree model */
enum {
  LASTNAME_COL,
  FIRSTNAME_COL,
  ONSITE_COL,
  TITLE_COL,
  ROOMNO_COL,
  NUM_COLS
};


/* verify that path is a valid node in tree */
gboolean node_exists(GtkTreeStore *tree, GtkTreePath *path)
{
  GtkTreeIter iter;

  return(gtk_tree_model_get_iter(GTK_TREE_MODEL(tree), &iter, path));
}

/* inserts a new node at path in tree */
void node_insert(GtkTreeStore *tree, GtkTreePath *path)
{
  gint depth;
  gint *indices;
  gint index;
  GtkTreeIter iter;

  /* determine depth and last index of path */
  depth = gtk_tree_path_get_depth(path);
  indices = gtk_tree_path_get_indices(path);
  index = indices[depth-1];

  if (!node_exists(tree, path))
  {
     if (depth == 1)
     {  /* if this is a child of the root node, use NULL instead of iter */
        while (!(gtk_tree_model_iter_n_children( GTK_TREE_MODEL(tree), NULL)
                == (index+1)))
        {
           gtk_tree_store_append(tree, &iter, NULL);
        }
     } else {
        GtkTreePath *parent_path;
        GtkTreeIter parent;
        
	/* determine parent node, creating parent if it does not exist */
        parent_path = gtk_tree_path_copy(path);
        gtk_tree_path_up(parent_path);
        if (!node_exists(tree, parent_path))
        {
           node_insert(tree, parent_path);
        }
        /* append new nodes up to index-th child of parent */
        gtk_tree_model_get_iter(GTK_TREE_MODEL(tree), &parent, parent_path);
        while(!(gtk_tree_model_iter_n_children(
                 GTK_TREE_MODEL(tree), &parent) == (index+1)))
        {
           gtk_tree_store_append(tree, &iter, &parent);
        }

        gtk_tree_path_free(parent_path);
     }
  }
}

/* read the room number column, and if the number is -1, send a dash (-)
   to the renderer instead */                   
void room_convert(GtkTreeViewColumn *column,
                  GtkCellRenderer *renderer,
                  GtkTreeModel *model,
                  GtkTreeIter *iter,
                  gpointer data)
{
  gint num;
  gchar *str;

  gtk_tree_model_get(model, iter, ROOMNO_COL, &num, -1);

  if (num == -1)
  {
     str = g_strdup(" - ");
  } else {
     str = g_strdup_printf("%d", num);
  }

  g_object_set(renderer, "text", str, NULL);
  g_free(str);
}

/* standard event handlers */
gint delete_event(GtkWidget *widget, GdkEvent event, gpointer data)
{
  return FALSE;
}

void end_program(GtkWidget *widget, gpointer data)
{
  gtk_main_quit();
}

int main(int argc, char **argv)
{
  GtkWindow *window;
  GtkTreeStore *company;
  employee *person;
  GtkTreePath *path;
  GtkTreeView *view;
  GtkScrolledWindow *scroller;

  GtkCellRenderer *room_no_renderer, *text_renderer,
                  *bold_renderer, *check_renderer;

  GtkTreeViewColumn *room_no_col, *title_col, *last_name_col,
                    *first_name_col, *on_site_col;

  /* initialize GTK+, create main window */
  gtk_init(&argc, &argv);
  window = g_object_new(GTK_TYPE_WINDOW,
                        "title", "Rixico Inc.",
                        "default-width", 600,
                        "default-height", 400,
                        NULL);

  /* attach standard handlers for main window */
  g_signal_connect(window, "delete_event", G_CALLBACK(delete_event), NULL);
  g_signal_connect(window, "destroy", G_CALLBACK(end_program), NULL);

  /* create tree model */
  company = gtk_tree_store_new(NUM_COLS,
                               G_TYPE_STRING,
                               G_TYPE_STRING,
                               G_TYPE_BOOLEAN,
                               G_TYPE_STRING,
                               G_TYPE_INT);

  /* point at start of personnel data */
  person = staff;
  /* fill model with personnel data */
  while (person->path != NULL)
  {
     GtkTreeIter iter;

     path = gtk_tree_path_new_from_string(person->path);
     node_insert(company, path);
     gtk_tree_model_get_iter(GTK_TREE_MODEL(company), &iter, path);
     gtk_tree_path_free(path);
     gtk_tree_store_set(company, &iter,
                        LASTNAME_COL, person->last_name,
                        FIRSTNAME_COL, person->first_name,
                        ONSITE_COL, person->on_site,
                        TITLE_COL, person->job_title,
                        ROOMNO_COL, person->room_no,
                        -1);
     person++;
  }

  /* create a right-justified renderer for room numbers */
  room_no_renderer = gtk_cell_renderer_text_new();
  g_object_set(room_no_renderer, "xalign", 1.0, NULL);

  /* a normal text renderer */
  text_renderer = gtk_cell_renderer_text_new();

  /* a renderer for text in boldface (for last name column) */
  bold_renderer = gtk_cell_renderer_text_new();
  g_object_set(bold_renderer, "weight", 500, NULL);

  /* a checkbox renderer */
  check_renderer = gtk_cell_renderer_toggle_new();

  /* create view columns */
  room_no_col = gtk_tree_view_column_new_with_attributes(
                   "Room", room_no_renderer,
                   NULL);

  gtk_tree_view_column_set_cell_data_func(room_no_col, room_no_renderer,
                                          room_convert,
                                          NULL, NULL);

  title_col = gtk_tree_view_column_new_with_attributes("Title", text_renderer,
                                                       "text", TITLE_COL,
                                                        NULL);

  /* allow user to move this column around */
  g_object_set(title_col, "reorderable", TRUE, NULL);

  last_name_col = gtk_tree_view_column_new_with_attributes(
                     "Last name", bold_renderer,
                     "text", LASTNAME_COL,
                      NULL);

  first_name_col = gtk_tree_view_column_new_with_attributes(
                      "First name", text_renderer,
                      "text", FIRSTNAME_COL,
                      NULL);

  on_site_col = gtk_tree_view_column_new_with_attributes(
                   "On site?", check_renderer,
                   "active", ONSITE_COL,
                   NULL);

  /* create overall view */
  view = g_object_new(GTK_TYPE_TREE_VIEW,
                      "model", company,
                      "rules-hint", TRUE,
                      "enable-search", TRUE,
                      "search-column", LASTNAME_COL,
                      NULL);

  /* put all columns into the view */
  gtk_tree_view_append_column(view, room_no_col);
  gtk_tree_view_append_column(view, last_name_col);
  gtk_tree_view_append_column(view, first_name_col);
  gtk_tree_view_append_column(view, on_site_col);
  gtk_tree_view_append_column(view, title_col);

  /* set drop arrow at front of the title column */
  g_object_set(view, "expander-column", title_col, NULL);

  /* put everything into a scrolled window */
  scroller = g_object_new(GTK_TYPE_SCROLLED_WINDOW, NULL);
  gtk_container_add(GTK_CONTAINER(scroller), GTK_WIDGET(view));
  gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(scroller));

  /* show everything and start GTK+ main loop */
  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_main();

  return 0;
}
