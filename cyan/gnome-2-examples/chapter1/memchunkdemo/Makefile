CFLAGS=-ansi -Wall

memchunkdemo: memchunkdemo.c
	gcc $(CFLAGS) `pkg-config glib-2.0 --cflags` -o memchunkdemo memchunkdemo.c\
    `pkg-config glib-2.0 --libs`

clean:
	rm -f  memchunkdemo
