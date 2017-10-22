#-------------------------------------------------
#
# Project created by QtCreator 2016-05-12T00:04:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mob
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    src/field.cpp \
    paintwidget.cpp

HEADERS  += mainwindow.h \
    src/field.h \
    src/Path.h \
    paintwidget.h

FORMS    += mainwindow.ui

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    levels.qrc

