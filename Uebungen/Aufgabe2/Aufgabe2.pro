#-------------------------------------------------
#
# Project created by QtCreator 2016-05-17T17:44:53
#
#-------------------------------------------------

QT       += core opengl gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++14
QMAKE_CXXFLAGS_DEBUG += -O0

LIBS += -lglut

TARGET = Aufgabe2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        points.cpp \
        glwidget.cpp \
    bezier_calc.cpp


HEADERS  += mainwindow.h \
            points.h \
            glwidget.h \
    bezier_calc.h

FORMS    += mainwindow.ui
