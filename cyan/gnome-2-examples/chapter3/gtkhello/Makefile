PROGRAM = gtkhello
CFLAGS = `pkg-config --cflags gtk+-2.0` -ansi -Wall
LIBS = `pkg-config --libs gtk+-2.0`

$(PROGRAM): $(PROGRAM).c
	gcc -o $(PROGRAM) $(PROGRAM).c $(CFLAGS) $(LIBS)

clean:
	rm -f $(PROGRAM)
