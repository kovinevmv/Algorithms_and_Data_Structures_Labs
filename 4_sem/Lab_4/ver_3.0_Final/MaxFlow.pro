#-------------------------------------------------
#
# Project created by @kovinevmv
#
#-------------------------------------------------

QT             += core gui
QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -Werror

CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = MaxFlow
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    graphwindow.cpp \
    mgraphview.cpp \
    node.cpp \
    edge.cpp \
    mscene.cpp \
    informationdialog.cpp \
    qcompressor.cpp

HEADERS += \
    graphwindow.h \
    mgraphview.h \
    node.h \
    edge.h \
    mscene.h \
    informationdialog.h \
    qcompressor.h

FORMS += \
    graphwindow.ui

RESOURCES += \
    icons.qrc

DISTFILES +=

LIBS += -L/usr/lib/zlibrary/ui -lz
