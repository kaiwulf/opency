PREFIX = /opt/gnome/default

CFLAGS = `pkg-config --cflags libgnomeui-2.0` -ansi -Wall \
		 -DPREFIX=\""$(PREFIX)"\" \
		 -DDATADIR=\""$(PREFIX)/share"\" \
		 -DSYSCONFDIR=\""$(PREFIX)/etc"\" \
		 -DLIBDIR=\""$(PREFIX)/lib"\" 

LIBS = `pkg-config --libs libgnomeui-2.0` \
       `pkg-config --libs libbonoboui-2.0`

appdemo: appdemo.c
	gcc -o appdemo appdemo.c $(CFLAGS) $(LIBS)

clean:
	rm -f appdemo
