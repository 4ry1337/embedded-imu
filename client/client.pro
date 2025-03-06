#-------------------------------------------------
#
# Project created by QtCreator 2016-11-16T22:48:02
#
#-------------------------------------------------

CONFIG   += thread console
QT       += core gui widgets openglwidgets network

TARGET = client
TEMPLATE = app


SOURCES += main.cpp mainwindow.cpp myglwidget.cpp mythread.cpp

HEADERS += mainwindow.h myglwidget.h mythread.h

FORMS += mainwindow.ui

# MAKE SURE THAT THE QT is same version, or change version here
# LIBS += -LC:/Qt/5.5/mingw492_32/lib/libQt5OpenGL.a -lopengl32
