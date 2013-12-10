include(../libwiimote/libwiimote.pri)

TARGET = test
TEMPLATE = app
QT += core gui phonon
SOURCES += \
    MainWin.cpp \
    Main.cpp

HEADERS += \
    MainWin.h

FORMS += \
    MainWin.ui

