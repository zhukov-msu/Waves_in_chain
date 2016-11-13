#-------------------------------------------------
#
# Project created by QtCreator 2013-10-16T22:44:38
#
#-------------------------------------------------

QT       += core gui \
            widgets \
            printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Wave
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    demowidget.cpp \
    qcustomplot.cpp \
    spring.cpp \
    mass.cpp \
    chain.cpp \
    wavetrainchain.cpp \
    monochain.cpp \
    doublechain.cpp \
    monochaindemo.cpp \
    doublechaindemo.cpp \
    wavetraindemo.cpp \
    rungekuttablechain.cpp \
    authors.cpp

HEADERS  += mainwindow.h \
    demowidget.h \
    qcustomplot.h \
    spring.h \
    mass.h \
    chain.h \
    wavetrainchain.h \
    monochain.h \
    doublechain.h \
    monochaindemo.h \
    doublechaindemo.h \
    wavetraindemo.h \
    rungekuttablechain.h \
    authors.h

FORMS    += mainwindow.ui \
    demowidget.ui \
    authors.ui

RESOURCES += \
    1.qrc

RC_FILE = Wave.rc

QMAKE_CXXFLAGS += -std=gnu++11
