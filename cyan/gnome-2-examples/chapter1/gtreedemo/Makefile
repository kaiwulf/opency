CFLAGS=-ansi -Wall

gtreedemo: gtreedemo.c
	gcc $(CFLAGS) `pkg-config glib-2.0 --cflags` -o gtreedemo gtreedemo.c\
    `pkg-config glib-2.0 --libs`

clean:
	rm -f gtreedemo
