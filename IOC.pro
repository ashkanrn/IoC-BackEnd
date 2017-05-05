#-------------------------------------------------
#
# Project created by QtCreator 2016-11-06T12:35:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled
TEMPLATE = app

target.path = /home/pi
INSTALLS += target

SOURCES += main.cpp\
        mainwindow.cpp \
    requestmapper.cpp \
    servercall.cpp \
    presenter.cpp \
    pagerenderer.cpp

HEADERS  += mainwindow.h \
    requestmapper.h \
    servercall.h \
    presenter.h \
    pagerenderer.h

RESOURCES += \
    res.qrc

include(QtWebApp/logging/logging.pri)
include(QtWebApp/httpserver/httpserver.pri)
include(QtWebApp/templateengine/templateengine.pri)
# Not used: include(../QtWebApp/qtservice/qtservice.pri)

DISTFILES += \
    errorlist
INCLUDEPATH  += /usr/include/poppler/qt5
LIBS         += -L/usr/lib -lpoppler-qt5
