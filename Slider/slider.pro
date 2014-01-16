TEMPLATE = app
TARGET = slider
INCLUDEPATH += .

include($$PWD/QSelectionSlider.pri)

HEADERS += mainwindow.h
SOURCES += main.cpp mainwindow.cpp
FORMS   += mainwindow.ui

#-- Check QT version
message(Qt version: $$[QT_VERSION])
greaterThan(QT_MAJOR_VERSION, 4) {
	QT += widgets
}

