PROGRAM = href
PREFIX = /opt/gnome/default
CFLAGS = `pkg-config --cflags libgnomeui-2.0` -ansi -Wall \
         -DPREFIX=\""$(PREFIX)\"" \
		 -DSYSCONFDIR=\""$(PREFIX)/etc"\" \
		 -DDATADIR=\""$(PREFIX)/share"\" \
		 -DLIBDIR=\""$(PREFIX)/lib"\"
LIBS = `pkg-config --libs libgnomeui-2.0`

$(PROGRAM): $(PROGRAM).c
	gcc -o $(PROGRAM) $(PROGRAM).c $(CFLAGS) $(LIBS)

clean:
	rm -f $(PROGRAM)
