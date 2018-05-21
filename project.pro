#-------------------------------------------------
#
# Project created by QtCreator 2018-05-21T19:10:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = project
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    scrollarea.cpp \
    chooseband.cpp \
    hdr.cpp \
    scrollarea.cpp \
    roitool.cpp \
    opencv_qt.cpp \
    func_elm.cpp

HEADERS  += mainwindow.h \
    hdr.h \
    chooseband.h \
    scrollarea.h \
    roitool.h \


FORMS    += mainwindow.ui \
    chooseband.ui \
    scrollarea.ui

INCLUDEPATH += D:\\ProgramFiles\\opencv\\build\\include \
                                        D:\\ProgramFiles\\opencv\\build\\include\\opencv2 \

LIBS +=D:\ProgramFiles\opencv\build\x86\vc12\lib\*.lib \
