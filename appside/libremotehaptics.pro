#-------------------------------------------------
#
# Project created by QtCreator 2012-06-14T16:24:57
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = remotehaptics
CONFIG += console


# By default build an example executable,
# uncomment the following two and comment away TEMPLATE=app to
# build the library!

#CONFIG += staticlib
#TEMPLATE = lib
TEMPLATE = app

SOURCES += libremotehaptics.cpp \
    networkhandler.cpp \
    main.cpp

HEADERS += libremotehaptics.h \
    networkhandler.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
