#-------------------------------------------------
#
# Project created by QtCreator 2015-10-01T15:03:47
#
#-------------------------------------------------

include(qtxlsx/qtxlsx.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Team_587_Login_System
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    teammember.cpp \
    teammemberlistmodel.cpp \
    register.cpp

HEADERS  += mainwindow.h \
    teammember.h \
    teammemberlistmodel.h \
    rapidxml/rapidxml_iterators.hpp \
    rapidxml/rapidxml_utils.hpp \
    rapidxml/rapidxml_print.hpp \
    rapidxml/rapidxml.hpp \
    register.h

FORMS    += mainwindow.ui \
    register.ui
