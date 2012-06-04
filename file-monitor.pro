
PROJECT_NAME = file-monitor

QMAKEVERSION = $$[QMAKE_VERSION]
ISQT4 = $$find(QMAKEVERSION, ^[2-9])
isEmpty( ISQT4 ) {
    error("Use the qmake include with Qt4.6 or greater, on Debian that is qmake-qt4");
}

TEMPLATE = app

TARGET = $${PROJECT_NAME}

mac {
    #TARGET = $${PROJECT_NAME}
    #QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.4
}

DEFINES += QT_NO_CAST_TO_ASCII QT_STRICT_ITERATORS

QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -Wall

DESTDIR = _build/.compiled

RESOURCES += resources.qrc

include(translations/translations.pri)
include(source/source.pri)
include(deployment/deployment.pri)

