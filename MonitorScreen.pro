QT += core gui
QT += core websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MonitorScreen
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
        monitorwindow.cpp

HEADERS += monitorwindow.h

FORMS    += monitorwindow.ui

CONFIG += C++14
