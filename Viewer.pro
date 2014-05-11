#-------------------------------------------------
#
# Project created by QtCreator 2013-07-04T22:19:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Viewer
TEMPLATE = app

LIBPATH += /home/mrhase/dev/xml-message-passing
LIBS += -lxmplib -ltinyxml

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp\
        ../fm/myml.cpp\
        ../fm/helper.cpp


HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
