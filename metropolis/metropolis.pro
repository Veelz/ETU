#-------------------------------------------------
#
# Project created by QtCreator 2018-03-04T13:55:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

TARGET = metropolis
TEMPLATE = app

SUBDIRS += tests/

SOURCES += main.cpp\
        mainwindow.cpp \
    metromap.cpp \
    train.cpp \
    station.cpp \
    uigraph.cpp \
    metrographicsview.cpp \
    lib/qtmaterialautocomplete.cpp \
    lib/qtmaterialoverlaywidget.cpp \
    lib/qtmaterialtextfield.cpp \
    graph/graphexception.cpp \
    lib/qtmaterialautocomplete_internal.cpp \
    lib/qtmaterialcheckable.cpp \
    lib/qtmaterialcheckable_internal.cpp \
    lib/qtmaterialflatbutton.cpp \
    lib/qtmaterialflatbutton_internal.cpp \
    lib/qtmaterialripple.cpp \
    lib/qtmaterialrippleoverlay.cpp \
    lib/qtmaterialstatetransition.cpp \
    lib/qtmaterialstyle.cpp \
    lib/qtmaterialtextfield_internal.cpp \
    lib/qtmaterialtheme.cpp \

HEADERS  += mainwindow.h \
    metromap.h \
    train.h \
    station.h \
    uigraph.h \
    metrographicsview.h \
    lib/qtmaterialautocomplete.h \
    lib/qtmaterialoverlaywidget.h \
    lib/qtmaterialtextfield.h \
    graph/edge.h \
    graph/graph.h \
    graph/graphallocator.h \
    graph/graphexception.h \
    graph/graphiterator.h \
    lib/qtmaterialautocomplete_internal.h \
    lib/qtmaterialautocomplete_p.h \
    lib/qtmaterialcheckable.h \
    lib/qtmaterialcheckable_internal.h \
    lib/qtmaterialcheckable_p.h \
    lib/qtmaterialflatbutton.h \
    lib/qtmaterialflatbutton_internal.h \
    lib/qtmaterialflatbutton_p.h \
    lib/qtmaterialripple.h \
    lib/qtmaterialrippleoverlay.h \
    lib/qtmaterialstatetransition.h \
    lib/qtmaterialstatetransitionevent.h \
    lib/qtmaterialstyle.h \
    lib/qtmaterialstyle_p.h \
    lib/qtmaterialtextfield_internal.h \
    lib/qtmaterialtextfield_p.h \
    lib/qtmaterialtheme.h \
    lib/qtmaterialtheme_p.h \
    graph/vertex.h

INCLUDEPATH += graph/

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc

win32: LIBS += -L$$PWD/lib/ -lcomponents

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/lib/components.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/lib/libcomponents.a
