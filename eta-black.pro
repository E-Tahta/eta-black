QT       += core gui widgets

RESOURCES += images.qrc

TARGET = eta-black
TEMPLATE = app


SOURCES += main.cpp\
        screenblackout.cpp

HEADERS  += screenblackout.h

target.path = /usr/bin/

INSTALLS += target
