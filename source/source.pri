
CONFIG += qt warn_on
contains(QT_BUILD_PARTS, tools): CONFIG += uitools
else : DEFINES += QT_NO_UITOOLS

win32 : Debug : CONFIG += console

INCLUDEPATH += $${PWD}
DEPENDPATH += $${PWD}

QT += svg \
    webkit \
    network \
    xml

# Share object files for faster compiling
RCC_DIR     = $${DESTDIR}
UI_DIR      = $${DESTDIR}
MOC_DIR     = $${DESTDIR}
OBJECTS_DIR = $${DESTDIR}

FORMS += mainwindow.ui \
    aboutdialog.ui

HEADERS += mainwindow.hpp \
    tabwidget.hpp \
    tabpage.hpp \
    view.hpp \
    svgview.hpp \
    textview.hpp \
    htmlview.hpp \
    imageview.hpp \
    utility.hpp \
    customgraphicsview.hpp \
    filesystemwatcher.hpp \
    aboutdialog.hpp \
    zoom.hpp

SOURCES += main.cpp \
    mainwindow.cpp \
    tabwidget.cpp \
    tabpage.cpp \
    view.cpp \
    svgview.cpp \
    textview.cpp \
    htmlview.cpp \
    imageview.cpp \
    utility.cpp \
    customgraphicsview.cpp \
    filesystemwatcher.cpp \
    aboutdialog.cpp \
    zoom.cpp

OTHER_FILES += \
    settings.txt \
    zoom.txt
