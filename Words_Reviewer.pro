#-------------------------------------------------
#
# Project created by QtCreator 2018-10-19T14:13:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Words_Reviewer
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
    appoption.cpp \
        main.cpp \
        mainwindow.cpp \
    windowsettings.cpp \
    word.cpp \
    wordgroup.cpp \
    wordsfile.cpp \
    customstring.cpp \
    inputform.cpp \
    customtimer.cpp \
    wordshowingmode.cpp

HEADERS += \
    appoption.h \
        mainwindow.h \
    windowsettings.h \
    word.h \
    wordgroup.h \
    wordsfile.h \
    customstring.h \
    inputform.h \
    customtimer.h \
    wordshowingmode.h

FORMS += \
        mainwindow.ui

DISTFILES +=
