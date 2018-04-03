HAVE_MSWINDOWS ?= no
USE_DBUS ?= yes
USE_GTK ?= yes
USE_QT ?= no

GIO_CFLAGS ?= -pthread -I/usr/include/gio-unix-2.0/ -I/usr/include/glib-2.0 -I/usr/lib/arm-linux-gnueabihf/glib-2.0/include
GIO_LIBS ?= -lgio-2.0 -lgobject-2.0 -lglib-2.0
GLIB_CFLAGS ?= -I/usr/include/glib-2.0 -I/usr/lib/arm-linux-gnueabihf/glib-2.0/include
GLIB_LIBS ?= -lglib-2.0
GMODULE_CFLAGS ?= -pthread -I/usr/include/glib-2.0 -I/usr/lib/arm-linux-gnueabihf/glib-2.0/include
GMODULE_LIBS ?= -Wl,--export-dynamic -lgmodule-2.0 -pthread -lglib-2.0
GTK_CFLAGS ?= -pthread -I/usr/include/gtk-2.0 -I/usr/lib/arm-linux-gnueabihf/gtk-2.0/include -I/usr/include/gio-unix-2.0/ -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/pixman-1 -I/usr/include/libpng16 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/libpng16 -I/usr/include/pango-1.0 -I/usr/include/harfbuzz -I/usr/include/pango-1.0 -I/usr/include/glib-2.0 -I/usr/lib/arm-linux-gnueabihf/glib-2.0/include -I/usr/include/freetype2
GTK_LIBS ?= -lgtk-x11-2.0 -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lfontconfig -lfreetype
QTCORE_CFLAGS ?= 
QTCORE_LIBS ?= 
QT_CFLAGS ?= 
QT_LIBS ?= 
