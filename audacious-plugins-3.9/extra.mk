# overrides setting in buildsys.mk
plugindir = /usr/local/lib/audacious

CONTAINER_PLUGIN_DIR ?= Container
CONTAINER_PLUGINS ?= asx asx3 audpl m3u pls xspf
EFFECT_PLUGINS ?= compressor crossfade crystalizer mixer silence-removal stereo_plugin voice_removal echo_plugin ladspa resample speedpitch
EFFECT_PLUGIN_DIR ?= Effect
GENERAL_PLUGINS ?=  alarm albumart lyricwiki playlist-manager search-tool statusicon gtkui skins delete-files skins-data cd-menu-items gnomeshortcuts mpris2 songchange hotkey aosd
GENERAL_PLUGIN_DIR ?= General
INPUT_PLUGINS ?= adplug metronom psf tonegen vtx xsf cdaudio flac vorbis amidiplug mpg123 aac wavpack console ffaudio
INPUT_PLUGIN_DIR ?= Input
OUTPUT_PLUGINS ?=  alsa oss4 filewriter
OUTPUT_PLUGIN_DIR ?= Output
TRANSPORT_PLUGIN_DIR ?= Transport
TRANSPORT_PLUGINS ?= gio neon
VISUALIZATION_PLUGINS ?=  blur_scope cairo-spectrum
VISUALIZATION_PLUGIN_DIR ?= Visualization

USE_GTK ?= yes
USE_QT ?= no

ALSA_CFLAGS ?= -I/usr/include/alsa
ALSA_LIBS ?= -lasound
AMPACHE_CFLAGS ?= 
AMPACHE_LIBS ?= 
BS2B_CFLAGS ?= 
BS2B_LIBS ?= 
CDIO_LIBS ?= -lcdio -lm -lcdio_cdda -lcdio -lm -lcddb
CDIO_CFLAGS ?= 
CUE_CFLAGS ?= 
CUE_LIBS ?= 
CURL_CFLAGS ?= 
CURL_LIBS ?= 
DBUS_CFLAGS ?= -I/usr/include/dbus-1.0 -I/usr/lib/arm-linux-gnueabihf/dbus-1.0/include -I/usr/include/glib-2.0 -I/usr/lib/arm-linux-gnueabihf/glib-2.0/include
DBUS_LIBS ?= -ldbus-glib-1 -ldbus-1 -lgobject-2.0 -lglib-2.0
FFMPEG_CFLAGS ?= -I/usr/include/arm-linux-gnueabihf
FFMPEG_LIBS ?= -lavcodec -lavformat -lavutil
FILEWRITER_CFLAGS ?=   
FILEWRITER_LIBS ?=  -logg -lvorbis -lvorbisenc -lvorbisfile -lFLAC
FLUIDSYNTH_CFLAGS ?= 
FLUIDSYNTH_LIBS ?= -lfluidsynth
GDKX11_CFLAGS ?= -pthread -I/usr/include/gtk-2.0 -I/usr/lib/arm-linux-gnueabihf/gtk-2.0/include -I/usr/include/gio-unix-2.0/ -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/include/harfbuzz -I/usr/include/pango-1.0 -I/usr/include/cairo -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/libpng16 -I/usr/include/glib-2.0 -I/usr/lib/arm-linux-gnueabihf/glib-2.0/include
GDKX11_LIBS ?= -lgdk-x11-2.0 -lpangocairo-1.0 -lpango-1.0 -lcairo -lgdk_pixbuf-2.0 -lgobject-2.0 -lglib-2.0
GIO_CFLAGS ?= -pthread -I/usr/include/gio-unix-2.0/ -I/usr/include/glib-2.0 -I/usr/lib/arm-linux-gnueabihf/glib-2.0/include
GIO_LIBS ?= -lgio-2.0 -lgobject-2.0 -lglib-2.0
GL_LIBS ?= 
GLIB_CFLAGS ?= -I/usr/include/glib-2.0 -I/usr/lib/arm-linux-gnueabihf/glib-2.0/include
GLIB_LIBS ?= -lglib-2.0
GMODULE_CFLAGS ?= -pthread -I/usr/include/glib-2.0 -I/usr/lib/arm-linux-gnueabihf/glib-2.0/include
GMODULE_LIBS ?= -Wl,--export-dynamic -lgmodule-2.0 -pthread -lglib-2.0
GTK_CFLAGS ?= -pthread -I/usr/include/gtk-2.0 -I/usr/lib/arm-linux-gnueabihf/gtk-2.0/include -I/usr/include/gio-unix-2.0/ -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/pixman-1 -I/usr/include/libpng16 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/libpng16 -I/usr/include/pango-1.0 -I/usr/include/harfbuzz -I/usr/include/pango-1.0 -I/usr/include/glib-2.0 -I/usr/lib/arm-linux-gnueabihf/glib-2.0/include -I/usr/include/freetype2
GTK_LIBS ?= -lgtk-x11-2.0 -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lfontconfig -lfreetype
JACK_CFLAGS ?= 
JACK_LIBS ?= 
LIBFLAC_LIBS ?= -lFLAC
LIBFLAC_CFLAGS ?= 
MMS_CFLAGS ?= 
MMS_LIBS ?= 
MODPLUG_CFLAGS ?= 
MODPLUG_LIBS ?= 
MPG123_CFLAGS ?= 
MPG123_LIBS ?= -lmpg123
NEON_CFLAGS ?= -D_LARGEFILE64_SOURCE -DNE_LFS -I/usr/include/neon
NEON_LIBS ?= -lneon
NOTIFY_CFLAGS ?= 
NOTIFY_LIBS ?= 
OSS_CFLAGS ?= 
SAMPLERATE_CFLAGS ?= 
SAMPLERATE_LIBS ?= -lsamplerate
SDL_CFLAGS ?= 
SDL_LIBS ?= 
SIDPLAYFP_CFLAGS ?= 
SIDPLAYFP_LIBS ?= 
SNDFILE_CFLAGS ?= 
SNDFILE_LIBS ?= 
QT_CFLAGS ?= 
QT_LIBS ?= 
QTMULTIMEDIA_CFLAGS ?= 
QTMULTIMEDIA_LIBS ?= 
QTOPENGL_CFLAGS ?= 
QTOPENGL_LIBS ?= 
VORBIS_CFLAGS ?= 
VORBIS_LIBS ?= -logg -lvorbis -lvorbisenc -lvorbisfile
WAVPACK_CFLAGS ?= 
WAVPACK_LIBS ?= -lwavpack
X11EXT_CFLAGS ?= 
X11EXT_LIBS ?= -lXrender -lX11 -lXcomposite
XML_CFLAGS ?= -I/usr/include/libxml2
XML_LIBS ?= -lxml2

HAVE_LINUX ?= yes
HAVE_MSWINDOWS ?= no
HAVE_DARWIN ?= no