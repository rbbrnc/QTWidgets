TEMPLATE = app
TARGET = app_template
DEPENDPATH += .
INCLUDEPATH += .

QT += core gui widgets
CONFIG += silent

#-- common gcc flags
QMAKE_CXXFLAGS += -Wall -W -Wunreachable-code
QMAKE_CXXFLAGS += -ffunction-sections -fdata-sections
QMAKE_CXXFLAGS += -std=c++11
QMAKE_LFLAGS   += -Wl,--gc-sections,--as-needed

#-- DEBUG Symbols
QMAKE_CXXFLAGS += -g -ggdb
#QMAKE_LFLAGS   += -Wl,--print-gc-sections
#DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT

HEADERS += \
    mainwindow.h \
    basepage.h \
    pages.h \
    form0.h \
    form1.h \
    form2.h

SOURCES += \
	main.cpp \
    mainwindow.cpp \
    basepage.cpp \
    form0.cpp \
    form1.cpp \
    form2.cpp \

FORMS += \
    mainwindow.ui \
    form0.ui \
    form1.ui \
    form2.ui

RESOURCES += application.qrc


