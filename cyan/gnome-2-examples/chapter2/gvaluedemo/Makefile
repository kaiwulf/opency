CFLAGS=-ansi -Wall

gvaluedemo: gvaluedemo.c
	gcc $(CFLAGS) `pkg-config gobject-2.0 --cflags --libs` -o \
	gvaluedemo gvaluedemo.c

clean:
	rm -f gvaluedemo
