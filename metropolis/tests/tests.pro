QT += core testlib
QT -= gui

CONFIG += c++14

TARGET = tests
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    testgraph.cpp

HEADERS += \
    testgraph.h
