TEMPLATE = app
TARGET = jtree
DEPENDPATH += .
INCLUDEPATH += .
QT += gui widgets
CONFIG += silent

include ($$PWD/JsonTree.pri)

SOURCES += \
	main.cpp \

