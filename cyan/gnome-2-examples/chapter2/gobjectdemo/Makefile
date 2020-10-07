CFLAGS=#-ansi -pedantic #-Wall

gobjectdemo: gobjectdemo.c
	gcc $(CFLAGS) `pkg-config gobject-2.0 --cflags --libs` -o gobjectdemo gobjectdemo.c

clean:
	rm -f gobjectdemo
