#-------------------------------------------------
#
# Project created by QtCreator 2018-04-20T11:55:04
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += core
QT       += widgets
QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Statki
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
        firebase.cpp \
    createship.cpp \
    gamevscomputer.cpp \
    gamevsplayer.cpp \
    clickablelabel.cpp \
    audio.cpp

HEADERS += \
        mainwindow.h \
        firebase.h \
    createship.h \
    gamevscomputer.h \
    gamevsplayer.h \
    clickablelabel.h \
    audio.h

FORMS += \
        mainwindow.ui

DISTFILES += \
    background.png

RESOURCES += \
    zasoby.qrc
