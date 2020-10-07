# ============== initialization ======================
AC_INIT([Example], [0.1], [example-bugs@example.com], [example])

AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE

# ============== basic compiler settings =============
AC_PROG_CC
AC_HEADER_STDC

# ============== take care of some localization ======
AH_TEMPLATE([GETTEXT_PACKAGE], [Package name for gettext])
GETTEXT_PACKAGE=example	# note that this is a command
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE")
AC_SUBST(GETTEXT_PACKAGE)
ALL_LINGUAS="de es"
AM_GLIB_GNU_GETTEXT
AC_PROG_INTLTOOL

# ============== export compiler/linker options ======
AC_SUBST(CFLAGS)
AC_SUBST(CPPFLAGS)
AC_SUBST(LDFLAGS)

# ============== look for GNOME ======================
GNOME_MODULES="libgnomeui-2.0 >= 2.0.0"
PKG_CHECK_MODULES(GNOME, $GNOME_MODULES)
AC_SUBST(GNOME_CFLAGS)
AC_SUBST(GNOME_LIBS)

# ============== generate files ======================
AC_CONFIG_FILES([
Makefile
src/Makefile
pixmaps/Makefile
po/Makefile.in
])
AC_OUTPUT
