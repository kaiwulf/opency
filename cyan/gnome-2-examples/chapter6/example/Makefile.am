## Process this file with automake to produce Makefile.in
SUBDIRS = src pixmaps po

desktopdir = $(datadir)/applications
desktop_in_files = example.desktop.in
desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@

keysdir = $(datadir)/mime-info
keys_in_files = example.keys.in
keys_DATA = $(keys_in_files:.keys.in=.keys)
@INTLTOOL_KEYS_RULE@

mimedir = $(datadir)/mime-info
mime_DATA = example.mime

EXTRA_DIST = example.desktop.in \
	     example.keys.in \
             intltool-extract.in \
             intltool-merge.in \
             intltool-update.in

DISTCLEANFILES = intltool-extract \
                 intltool-merge \
                 intltool-update

clean-local:
	rm -f example.desktop
	rm -f example.keys
	rm -f po/.intltool-merge-cache

