CFLAGS=-ansi -Wall

ghashtabledemo: ghashtabledemo.c
	gcc $(CFLAGS) `pkg-config glib-2.0 --cflags` -o ghashtabledemo ghashtabledemo.c\
    `pkg-config glib-2.0 --libs`

clean:
	rm -f ghashtabledemo
