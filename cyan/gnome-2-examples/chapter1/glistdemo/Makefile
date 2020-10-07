CFLAGS=-ansi -Wall

glistdemo: glistdemo.c
	gcc $(CFLAGS) `pkg-config glib-2.0 --cflags` -o glistdemo glistdemo.c\
    `pkg-config glib-2.0 --libs`

clean:
	rm -f glistdemo
