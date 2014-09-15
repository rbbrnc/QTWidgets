TEMPLATE = app
QT += widgets
INCLUDEPATH += $$PWD

include ($$PWD/QSlideSwitch.pri)
include ($$PWD/iOSSwitch.pri)

QMAKE_CXXFLAGS += -Wall -W -Wunreachable-code

#-- Debug/Profiling
#QMAKE_CXXFLAGS += -g -ggdb
#QMAKE_CXXFLAGS += -pg
#QMAKE_LFLAGS += -pg

SOURCES += main.cpp
CONFIG += release
