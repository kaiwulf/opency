CFLAGS=-ansi -Wall

gstringdemo: gstringdemo.c
	gcc $(CFLAGS) `pkg-config glib-2.0 --cflags` -o gstringdemo gstringdemo.c\
    `pkg-config glib-2.0 --libs`

clean:
	rm -f gstringdemo
