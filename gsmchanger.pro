#-------------------------------------------------
#
# Project created by QtCreator 2015-08-27T17:04:33
#
#-------------------------------------------------

QT       += core gui serialport
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mu609_kit
TEMPLATE = app


SOURCES += main.cpp\
        inputtest.cpp \
    serialdialog.cpp

HEADERS  += inputtest.h \
    serialdialog.h

FORMS    += inputtest.ui \
    serialdialog.ui

