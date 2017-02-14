QT += core gui
QT += core websockets
QT += widgets

TARGET = MonitorScreen
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
        monitorwindow.cpp

HEADERS += monitorwindow.h

FORMS    += monitorwindow.ui

CONFIG += C++14
