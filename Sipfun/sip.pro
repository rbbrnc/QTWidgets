TARGET = qsip
TEMPLATE = app
QT += network

CONFIG += silent

#-- Places for generated files
OBJECTS_DIR = objs_$$QT_ARCH
MOC_DIR = mocs_$$QT_ARCH
UI_DIR = ui_$$QT_ARCH
RCC_DIR = resources_$$QT_ARCH
#----------------------------------

include ($$PWD/../QUpdateLabel/QUpdateLabel.pri)

EXTRA_PATH  = $$PWD/../3rdparty

INCLUDEPATH += $$COMMON_PATH

SOURCES += \
	main.cpp \
	sipclient.cpp \
	MainWindow.cpp \
	downloadmanager.cpp

HEADERS += \
	sipclient.h \
	MainWindow.h \
	downloadmanager.h

FORMS += \
	MainWindow.ui \

#RESOURCES += pixmaps.qrc

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
	QMAKE_CXXFLAGS += -Wall -W -Wunreachable-code -g -ggdb

	INCLUDEPATH += /opt/linphone.minimal/include

	LIBS += /opt/linphone.minimal/lib/liblinphone.a
	LIBS += /opt/linphone.minimal/lib/libmediastreamer_base.a
	LIBS += /opt/linphone.minimal/lib/libmediastreamer_voip.a
	LIBS += /opt/linphone.minimal/lib/libortp.a
	LIBS += /usr/local/lib/libeXosip2.a
	LIBS += /usr/local/lib/libosip2.a
	LIBS += /usr/local/lib/libosipparser2.a
	LIBS += -lssl -lresolv
}

