TARGET = qsip
TEMPLATE = app

QT += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += silent

#-- Places for generated files
OBJECTS_DIR = objs_$$QT_ARCH
MOC_DIR = mocs_$$QT_ARCH
UI_DIR = ui_$$QT_ARCH
RCC_DIR = resources_$$QT_ARCH
#----------------------------------

include ($$PWD/../QUpdateLabel/QUpdateLabel.pri)

#EXTRA_PATH  = $$PWD/../3rdparty

INCLUDEPATH += $$COMMON_PATH

SOURCES += \
	main.cpp \
	MainWindow.cpp \
	downloadmanager.cpp \
	uploadmanager.cpp \
    authenticationdialog.cpp \
    QLinPhoneCore.cpp \
	chatmessage.cpp \
    chatclientform.cpp

#	sipclient.cpp \

HEADERS += \
	MainWindow.h \
	downloadmanager.h \
	uploadmanager.h \
    authenticationdialog.h \
    QLinPhoneCore.h \
	chatmessage.h \
    chatclientform.h

#	sipclient.h \

FORMS += \
	MainWindow.ui \
    authenticationdialog.ui \
    chatclientform.ui

unix:contains(QT_ARCH, powerpc): {
	DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT
	INCLUDEPATH += /opt/eldk4.2/ppc_6xx/usr/include
	LIBS        += -L/opt/eldk4.2/ppc_6xx/usr/local -lts

	SIPLIBS = $$PWD/ppclibs
	INCLUDEPATH += $$SIPLIBS/include

	LIBS += $$SIPLIBS/liblinphone.a
	LIBS += $$SIPLIBS/libmediastreamer_base.a
	LIBS += $$SIPLIBS/libmediastreamer_voip.a
	LIBS += $$SIPLIBS/libortp.a
	LIBS += $$SIPLIBS/libeXosip2.a
	LIBS += $$SIPLIBS/libosip2.a
	LIBS += $$SIPLIBS/libosipparser2.a
	LIBS += -lresolv -lssl
}

unix:contains(QT_ARCH, i386): {
	LINPHONE_CFLAGS = $$system(pkg-config linphone --cflags)
	LINPHONE_LFLAGS = $$system(pkg-config linphone --libs)

	QMAKE_CXXFLAGS += -Wall -W -Wunreachable-code -g -ggdb
	QMAKE_CXXFLAGS += $$LINPHONE_CFLAGS
	LIBS += $$LINPHONE_LFLAGS
}

RESOURCES += \
    resources.qrc

