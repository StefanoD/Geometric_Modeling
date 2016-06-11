#-------------------------------------------------
#
# Project created by QtCreator 2016-06-11T19:00:30
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++14

LIBS += -lglut

TARGET = Aufgabe3
TEMPLATE = app


SOURCES += main.cpp \
        mainwindow.cpp \
        knots.cpp \
        points.cpp \
        glwidget.cpp

HEADERS  += mainwindow.h \
            knots.h \
            points.h \
            glwidget.h

FORMS    += mainwindow.ui
