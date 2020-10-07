all: messagedemo printhandlerdemo
test: messagedemo.out printhandlerdemo.out

CFLAGS=-ansi -Wall

messagedemo: messagedemo.c
	gcc $(CFLAGS) `pkg-config glib-2.0 --cflags` -o messagedemo messagedemo.c\
    `pkg-config glib-2.0 --libs`

messagedemo.out: messagedemo
	rm -f messagedemo.out && \
	./messagedemo >messagedemo.out 2>&1

printhandlerdemo: printhandlerdemo.c
	gcc $(CFLAGS) `pkg-config glib-2.0 --cflags` -o printhandlerdemo printhandlerdemo.c\
    `pkg-config glib-2.0 --libs`


printhandlerdemo.out: printhandlerdemo
	rm -f printhandlerdemo.out && \
	./printhandlerdemo >printhandlerdemo.out 2>&1


clean:
	rm -f messagedemo messagedemo.out printhandlerdemo printhandlerdemo.out core
