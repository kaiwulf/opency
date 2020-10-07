CFLAGS=-ansi -Wall

garraydemo: garraydemo.c
	gcc $(CFLAGS) `pkg-config glib-2.0 --cflags` -o garraydemo garraydemo.c\
    `pkg-config glib-2.0 --libs`

clean:
	rm -f garraydemo

