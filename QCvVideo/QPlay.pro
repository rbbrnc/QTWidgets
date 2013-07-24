TEMPLATE = app
TARGET =
INCLUDEPATH += .

SLIDERPATH=$$PWD/../Slider
include($$PWD/QCvVideo.pri)
#include($$PWD/../../Slider/QSelectionSlider.pri)
include($$SLIDERPATH/QSelectionSlider.pri)

HEADERS += \
	mainwindow.h \
	cutlistdialog.h \
	filters.h

FORMS   += \
	mainwindow.ui \
	cutlistdialog.ui

SOURCES += \
	main.cpp \
	mainwindow.cpp \
	cutlistdialog.cpp \
	filters.cpp
