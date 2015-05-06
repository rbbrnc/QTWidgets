TEMPLATE = app
TARGET = keyboardtest
#DEPENDPATH += .
INCLUDEPATH += .
#QT += network webkit

CONFIG += silent

#-- Places for generated files
OBJECTS_DIR = objs_$$QT_ARCH
MOC_DIR = mocs_$$QT_ARCH
UI_DIR = ui_$$QT_ARCH
RCC_DIR = resources_$$QT_ARCH
#----------------------------------

#######include($$PWD/SoftKeyboard/SoftKeyboard.pri)

HEADERS += \
    mainwindow.h \
    keyboarddialog.h \

SOURCES += \
	main.cpp \
    mainwindow.cpp \
    keyboarddialog.cpp \

FORMS += \
    mainwindow.ui \
    keyboarddialog.ui \

unix:contains(QT_ARCH, powerpc): {
#	DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT
#	INCLUDEPATH += /opt/eldk4.2/ppc_6xx/usr/include
#	LIBS        += -L/opt/eldk4.2/ppc_6xx/usr/local -lts
}

unix:contains(QT_ARCH, i386): {
#	DEFINES +=
	QMAKE_CXXFLAGS += -Wall -W -Wunreachable-code -g -ggdb
}

#RESOURCES += resources.qrc

RESOURCES += \
    pixmaps.qrc

