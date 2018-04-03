PACKAGE ?= libaudclient
VERSION ?= 3.5-rc2

DBUS_BINDING_TOOL ?= /usr/bin/dbus-binding-tool

DBUS_CFLAGS ?= -I/usr/include/dbus-1.0 -I/usr/lib/arm-linux-gnueabihf/dbus-1.0/include -I/usr/include/glib-2.0 -I/usr/lib/arm-linux-gnueabihf/glib-2.0/include
DBUS_LIBS ?= -ldbus-glib-1 -ldbus-1 -lgobject-2.0 -lglib-2.0
GLIB_CFLAGS ?= -I/usr/include/glib-2.0 -I/usr/lib/arm-linux-gnueabihf/glib-2.0/include
GLIB_LIBS ?= -lglib-2.0
