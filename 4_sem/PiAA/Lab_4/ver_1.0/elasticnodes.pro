QT += widgets

HEADERS += \
        edge.h \
        node.h \
    mainwindow.h \
    mscene.h \
    mgraphview.h \
    options.h \
    helpbrowser.h \
    chose.h \
    qcompressor.h

SOURCES += \
        edge.cpp \
        main.cpp \
        node.cpp \
    mainwindow.cpp \
    mscene.cpp \
    mgraphview.cpp \
    options.cpp \
    helpbrowser.cpp \
    chose.cpp \
    qcompressor.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/graphicsview/elasticnodes
INSTALLS += target

FORMS += \
    mainwindow.ui \
    options.ui \
    chose.ui

RESOURCES += \
    resources.qrc

LIBS += -L/usr/lib/zlibrary/ui -lz

