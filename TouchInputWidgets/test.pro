TEMPLATE = app
TARGET = test
#DEPENDPATH += .
INCLUDEPATH += .

CONFIG += silent

#-- Places for generated files
OBJECTS_DIR = objs_$$QT_ARCH
MOC_DIR = mocs_$$QT_ARCH
UI_DIR = ui_$$QT_ARCH
RCC_DIR = resources_$$QT_ARCH
#----------------------------------

include($$PWD/TouchNumberEdit/TouchNumberEdit.pri)
include($$PWD/TouchDateTimeEdit/TouchDateTimeEdit.pri)
include($$PWD/TouchLineEdit/TouchLineEdit.pri)

HEADERS += mainwindow.h

SOURCES += \
	main.cpp \
    mainwindow.cpp \

FORMS += mainwindow.ui

unix:contains(QT_ARCH, powerpc): {
#	DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT
#	INCLUDEPATH += /opt/eldk4.2/ppc_6xx/usr/include
#	LIBS        += -L/opt/eldk4.2/ppc_6xx/usr/local -lts
}

unix:contains(QT_ARCH, i386): {
#	DEFINES +=
	QMAKE_CXXFLAGS += -Wall -W -Wunreachable-code -g -ggdb
}

