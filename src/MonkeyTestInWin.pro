#-------------------------------------------------
#
# Project created by QtCreator 2019-07-23T14:40:28
#
#-------------------------------------------------

QT       += core gui winextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MonkeyTestInWin
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    MonkeyTestExecutor.cpp \
    CaptureFrame.cpp

HEADERS  += mainwindow.h \
    MonkeyTestExecutor.h \
    CaptureFrame.h

FORMS    += mainwindow.ui
DEFINES += _AFXDLL
LIBS +=User32.LIB
