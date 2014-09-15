TEMPLATE = app
TARGET = QPlay
INCLUDEPATH += .

#QMAKE_CXXFLAGS += -g -ggdb

#-- Check QT version
message(Qt version: $$[QT_VERSION])
greaterThan(QT_MAJOR_VERSION, 4) {
	QT += widgets
}

SLIDERPATH=$$PWD/../Slider
include($$PWD/QCvVideo.pri)
#include($$PWD/../../Slider/QSelectionSlider.pri)
include($$SLIDERPATH/QSelectionSlider.pri)

HEADERS += \
	mainwindow.h \
	cutlistdialog.h \
	filters.h \
    framemarkerdialog.h

FORMS   += \
	mainwindow.ui \
	cutlistdialog.ui \
    framemarkerdialog.ui

SOURCES += \
	main.cpp \
	mainwindow.cpp \
	cutlistdialog.cpp \
	filters.cpp \
    framemarkerdialog.cpp

RESOURCES += \
	qplay.qrc

