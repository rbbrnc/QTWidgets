TEMPLATE = app
TARGET = widgettest
#DEPENDPATH += .
INCLUDEPATH += .

CONFIG += silent

#-- Places for generated files
OBJECTS_DIR = objs_$$QT_ARCH
MOC_DIR = mocs_$$QT_ARCH
UI_DIR = ui_$$QT_ARCH
RCC_DIR = resources_$$QT_ARCH
#----------------------------------

include($$PWD/src/TouchWidgets.pri)

HEADERS += mainwindow.h

SOURCES += \
	main.cpp \
    mainwindow.cpp \

FORMS += mainwindow.ui

QMAKE_CXXFLAGS += -Wall -W -g -ggdb

unix:contains(QT_ARCH, powerpc): {
#	DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT
}

unix:contains(QT_ARCH, i386): {
#	DEFINES +=
	QMAKE_CXXFLAGS += -Wunreachable-code
}

OTHER_FILES += style.qss
