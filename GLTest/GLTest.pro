#-------------------------------------------------
#
# Project created by QtCreator 2015-03-19T11:51:37
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GLTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    depthwidget.cpp \
    meshengine.cpp \
    trackball.cpp

HEADERS  += mainwindow.h \
    depthwidget.h \
    meshengine.h \
    trackball.h

FORMS    += mainwindow.ui

RESOURCES += \
    glsl.qrc

