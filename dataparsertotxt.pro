#-------------------------------------------------
#
# Project created by QtCreator 2017-04-17T13:24:00
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += debug_and_release
CONFIG += build_all


CONFIG(debug, debug|release):LIBS += "libParserMemDCSd.lib"
CONFIG(release, debug|release):LIBS += "libParserMemDCS.lib"

TARGET = dataparsertotxt
TEMPLATE = app


SOURCES += main.cpp\
        dataparsertotxt.cpp

HEADERS  += dataparsertotxt.h \
    node.h \
    parameter.h \
    parserMemDCS.h \
    structure.h

FORMS    += dataparsertotxt.ui

RESOURCES += \
    icons.qrc
