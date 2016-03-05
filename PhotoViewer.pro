#-------------------------------------------------
#
# Project created by QtCreator 2016-02-23T11:04:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console

TARGET = PhotoViewer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    optionsdialog.cpp \
    chooserdialog.cpp

HEADERS  += mainwindow.h \
    optionsdialog.h \
    chooserdialog.h

FORMS    += mainwindow.ui \
    optionsdialog.ui \
    chooserdialog.ui
