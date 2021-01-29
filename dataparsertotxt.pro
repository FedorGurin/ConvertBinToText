#-------------------------------------------------
#
# Project created by QtCreator 2017-04-17T13:24:00
#
#-------------------------------------------------

TEMPLATE = app
CONFIG += qt thread
CONFIG += debug_and_release c++11


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets xml
CONFIG += debug_and_release
CONFIG += build_all


CONFIG(debug, debug|release):LIBS += "D:\MyProg\226\ui\ConvertBinToText\libParserMemDCSd.lib"
CONFIG(release, debug|release):LIBS += "D:\MyProg\226\ui\ConvertBinToText\libParserMemDCS.lib"

TARGET = dataparsertotxt
TEMPLATE = app


SOURCES += main.cpp\
        dataparsertotxt.cpp

HEADERS  += dataparsertotxt.h \
    node.h \
    parameter.h \
    structure.h \
    mppm/IEngineData.h \
    mppm/libmppm.h \
    mppm/libmppm_global.h \
    mppm/libmppmSpec.h

FORMS    += dataparsertotxt.ui

RESOURCES += \
    icons.qrc
