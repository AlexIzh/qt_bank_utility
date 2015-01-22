#-------------------------------------------------
#
# Project created by QtCreator 2015-01-11T21:46:20
#
#-------------------------------------------------

QT       += core gui\
            printsupport\
            sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = banknew
TEMPLATE = app
CONFIG += c++11
APP_IMAGE_FILES.files = 1.jpg
APP_IMAGE_FILES.files += __db__
APP_IMAGE_FILES.files += sprav.txt
APP_IMAGE_FILES.path = Contents/MacOS

QMAKE_BUNDLE_DATA += APP_IMAGE_FILES

SOURCES += main.cpp\
        mainwindow.cpp \
    easyform.cpp \
    pixmapform.cpp \
    dialog.cpp

HEADERS  += mainwindow.h \
    easyform.h \
    pixmapform.h \
    dialog.h

FORMS    += mainwindow.ui \
    easyform.ui \
    pixmapform.ui \
    dialog.ui

DISTFILES += \
    1.jpg \
    __db__ \
    sprav.txt
