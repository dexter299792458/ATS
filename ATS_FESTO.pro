#-------------------------------------------------
#
# Project created by QtCreator 2015-09-29T10:46:07
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ATS_FESTO
TEMPLATE = app


SOURCES += main.cpp\
        userinterface.cpp \
    programeditor.cpp \
    serialportmanager.cpp \
    console.cpp \
    enterprogramname.cpp \
    createcoordinates.cpp

HEADERS  += userinterface.h \
    programeditor.h \
    serialportmanager.h \
    console.h \
    enterprogramname.h \
    createcoordinates.h

FORMS    += userinterface.ui \
    enterprogramname.ui \
    createcoordinates.ui
