QT += core gui
QT += core websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WebSocketTest
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp \
        websockettest.cpp

HEADERS += websockettest.h

FORMS += websockettest.ui

CONFIG += C++14;
