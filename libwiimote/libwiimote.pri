INCLUDEPATH += $$PWD
LIBS += -lbluetooth -lm
DEFINES += _ENABLE_TILT _ENABLE_FORCE
#-D_DISABLE_NONBLOCK_UPDATES -D_DISABLE_AUTO_SELECT_DEVICE

HEADERS += $$PWD/bthid.h \
		  $$PWD/wiimote_io.h \
		  $$PWD/wiimote_report.h \
		  $$PWD/wiimote_error.h \
		  $$PWD/wiimote_event.h \
		  $$PWD/wiimote_ir.h \
		  $$PWD/wiimote_link.h \
		  $$PWD/wiimote_util.h \
		  $$PWD/wiimote_api.h \
		  $$PWD/wiimote_speaker.h \
		  $$PWD/wiimote_mii.h \
		  $$PWD/wiimote_nunchuk.h \
		  $$PWD/wiimote.h
		  
SOURCES += $$PWD/wiimote_link.c \
		  $$PWD/wiimote_ir.c \
		  $$PWD/wiimote_event.c \
		  $$PWD/wiimote_io.c \
		  $$PWD/wiimote_report.c \
		  $$PWD/wiimote_api.c \
		  $$PWD/wiimote_error.c \
		  $$PWD/wiimote_speaker.c \
		  $$PWD/wiimote_mii.c \
		  $$PWD/wiimote_nunchuk.c
