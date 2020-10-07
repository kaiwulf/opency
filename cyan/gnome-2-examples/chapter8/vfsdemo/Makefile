PROGRAM = vfsdemo
CFLAGS = `pkg-config --cflags gnome-vfs-2.0 gnome-vfs-module-2.0 libgnome-2.0` -ansi -Wall
LIBS = `pkg-config --libs gnome-vfs-2.0 gnome-vfs-module-2.0 libgnome-2.0`

$(PROGRAM): $(PROGRAM).c
	gcc -o $(PROGRAM) $(PROGRAM).c $(CFLAGS) $(LIBS)

clean:
	rm -f $(PROGRAM)
