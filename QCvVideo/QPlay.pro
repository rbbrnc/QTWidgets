TEMPLATE = app
TARGET =
INCLUDEPATH += .

SLIDERPATH=$$PWD/../Slider
include($$PWD/QCvVideo.pri)
#include($$PWD/../../Slider/QSelectionSlider.pri)
include($$SLIDERPATH/QSelectionSlider.pri)

HEADERS += \
	mainwindow.h \
	cutlistdialog.h

FORMS   += \
	mainwindow.ui \
	cutlistdialog.ui

SOURCES += \
	main.cpp \
	mainwindow.cpp \
	cutlistdialog.cpp
