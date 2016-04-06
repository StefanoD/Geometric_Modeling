#-------------------------------------------------
#
# Project created by QtCreator 2016-04-06T17:26:59
#
#-------------------------------------------------

QT       += core opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++14
QMAKE_CXXFLAGS_DEBUG += -O0

LIBS += -lglut

TARGET = Aufgabe1
TEMPLATE = app


SOURCES +=  main.cpp \
            quader.cpp \
            quaternion.cpp \
            viewSystem.cpp

HEADERS  += color.h \
            mat.h \
            quader.h \
            quaternion.h \
            vec.h \
            viewSystem.h
