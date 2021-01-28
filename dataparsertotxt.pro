#-------------------------------------------------
#
# Project created by QtCreator 2017-04-17T13:24:00
#
#-------------------------------------------------

QT       += core gui xml
CONFIG += debug_and_release
CONFIG +=build_all
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


CONFIG(debug, debug|release):LIBS += "D:\MyProg\226\ui\ConvertBinToText\libParserMemDCSd.lib"
CONFIG(release, debug|release):LIBS += "D:\MyProg\226\ui\ConvertBinToText\libParserMemDCS.lib"

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
