#ifndef CYAN_WINDOW_H
#define CYAN_WINDOW_H

typedef struct {
	GtkWidget *window;
	GtkWidget *menubar;
	GtkWidget *view;
	GtkTextBuffer *buffer;
} CyanWin;
/*
CyanWin *create_main_window(void);
void set_main_window_title(void);
*/
#endif