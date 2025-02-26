lib_LTLIBRARIES += zbar/libzbar.la
zbar_libzbar_la_CPPFLAGS = -I$(srcdir)/zbar $(AM_CPPFLAGS)
zbar_libzbar_la_LDFLAGS = -no-undefined -version-info $(LIB_VERSION) \
    -export-symbols-regex "^(zbar|_zbar.*_error)_.*" $(AM_LDFLAGS)
zbar_libzbar_la_LIBADD = $(LTLIBICONV)

zbar_libzbar_la_SOURCES = zbar/debug.h zbar/config.c \
    zbar/error.h zbar/error.c zbar/symbol.h zbar/symbol.c \
    zbar/image.h zbar/image.c zbar/convert.c \
    zbar/processor.c zbar/processor.h zbar/processor/lock.c \
    zbar/refcnt.h zbar/refcnt.c zbar/timer.h zbar/mutex.h \
    zbar/event.h zbar/thread.h \
    zbar/window.h zbar/window.c zbar/video.h zbar/video.c \
    zbar/img_scanner.h zbar/img_scanner.c zbar/scanner.c \
    zbar/decoder.h zbar/decoder.c

EXTRA_zbar_libzbar_la_SOURCES = zbar/svg.h zbar/svg.c

if ENABLE_EAN
zbar_libzbar_la_SOURCES += zbar/decoder/ean.h zbar/decoder/ean.c
endif
if ENABLE_DATABAR
zbar_libzbar_la_SOURCES += zbar/decoder/databar.h zbar/decoder/databar.c
endif
if ENABLE_CODE128
zbar_libzbar_la_SOURCES += zbar/decoder/code128.h zbar/decoder/code128.c
endif
if ENABLE_CODE93
zbar_libzbar_la_SOURCES += zbar/decoder/code93.h zbar/decoder/code93.c
endif
if ENABLE_CODE39
zbar_libzbar_la_SOURCES += zbar/decoder/code39.h zbar/decoder/code39.c
endif
if ENABLE_CODABAR
zbar_libzbar_la_SOURCES += zbar/decoder/codabar.h zbar/decoder/codabar.c
endif
if ENABLE_I25
zbar_libzbar_la_SOURCES += zbar/decoder/i25.h zbar/decoder/i25.c
endif
if ENABLE_PDF417
zbar_libzbar_la_SOURCES += zbar/decoder/pdf417.h zbar/decoder/pdf417.c \
    zbar/decoder/pdf417_hash.h
endif

if ENABLE_QRCODE
zbar_libzbar_la_SOURCES += zbar/qrcode.h \
    zbar/decoder/qr_finder.h zbar/decoder/qr_finder.c \
    zbar/qrcode/qrdec.h zbar/qrcode/qrdec.c zbar/qrcode/qrdectxt.c \
    zbar/qrcode/rs.h zbar/qrcode/rs.c \
    zbar/qrcode/isaac.h zbar/qrcode/isaac.c \
    zbar/qrcode/bch15_5.h zbar/qrcode/bch15_5.c \
    zbar/qrcode/binarize.h zbar/qrcode/binarize.c \
    zbar/qrcode/util.h zbar/qrcode/util.c
endif


if HAVE_V4L2
zbar_libzbar_la_SOURCES += zbar/video/v4l.c zbar/video/v4l2.c
endif


if HAVE_JPEG
zbar_libzbar_la_SOURCES += zbar/jpeg.c
endif

zbar_libzbar_la_SOURCES += zbar/processor/posix.h zbar/processor/posix.c
zbar_libzbar_la_SOURCES += zbar/processor/fb.c zbar/window/fb.c

zbar_libzbar_la_LDFLAGS += $(AM_LDFLAGS)
zbar_libzbar_la_LIBADD += $(AM_LIBADD)
