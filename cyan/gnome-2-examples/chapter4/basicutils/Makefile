PREFIX = /opt/gnome/default

CFLAGS = `pkg-config --cflags libgnomeui-2.0` -ansi -Wall \
		-DPREFIX=\""$(PREFIX)"\" \
		-DDATADIR=\""$(PREFIX)/share"\" \
		-DSYSCONFDIR=\""$(PREFIX)/etc"\" \
		-DLIBDIR=\""$(PREFIX)/lib"\" 

LIBS = `pkg-config --libs libgnomeui-2.0`

basicutils: basicutils.c
	gcc -o basicutils basicutils.c $(CFLAGS) $(LIBS)

clean:
	rm -f basicutils

