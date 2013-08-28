#-------------------------------------------------
#
# Project created by QtCreator 2013-08-23T10:44:53
#
#-------------------------------------------------

QT       += core gui
QT       += widgets

qtHaveModule(opengl): QT += opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = anti-submarine
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    copter.cpp \
    graphicsscene.cpp \
    bomb.cpp \
    pixmapitem.cpp \
    qanimationstate.cpp \
    animationmanager.cpp \
    states.cpp \
    diver.cpp

HEADERS  += mainwindow.h \
    copter.h \
    graphicsscene.h \
    copter_p.h \
    bomb.h \
    pixmapitem.h \
    qanimationstate.h \
    animationmanager.h \
    states.h \
    diver.h \
    diver_p.h

RESOURCES += \
    antisubmarine.qrc
