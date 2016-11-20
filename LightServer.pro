QT += core qml websockets
#QT -= gui

CONFIG += c++11

TARGET = LightServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    lightserver.cpp \
    backlight.cpp

DISTFILES +=

RESOURCES +=

HEADERS += \
    lightserver.h \
    backlight.h
