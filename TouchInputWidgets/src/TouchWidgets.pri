INCLUDEPATH += $$PWD

RESOURCES += $$PWD/TouchWidgets.qrc
SOURCES += $$PWD/basedialog.cpp
HEADERS += $$PWD/basedialog.h

#-----------------------------------------------------------------------------
#-- UI Slide Switch Button
#-----------------------------------------------------------------------------
SOURCES += $$PWD/uiswitch.cpp
HEADERS += $$PWD/uiswitch.h

#-----------------------------------------------------------------------------
#-- Touch TimeEdit
#-----------------------------------------------------------------------------
SOURCES += \
	$$PWD/timeedit.cpp \
	$$PWD/timeeditdialog.cpp

HEADERS += \
	$$PWD/timeedit.h \
	$$PWD/timeeditdialog.h

FORMS += $$PWD/timeeditdialog.ui

#-----------------------------------------------------------------------------
#-- Touch LineEdit
#-----------------------------------------------------------------------------
HEADERS += \
    $$PWD/keyboarddialog.h \
    $$PWD/touchlineedit.h

SOURCES += \
    $$PWD/keyboarddialog.cpp \
    $$PWD/touchlineedit.cpp

FORMS     += $$PWD/keyboarddialog.ui
RESOURCES += $$PWD/keyboarddialog.qrc

#-----------------------------------------------------------------------------
#-- Touch ListWidget
#-----------------------------------------------------------------------------
SOURCES += \
	$$PWD/touchlistedit.cpp \
	$$PWD/listselectiondialog.cpp

HEADERS += \
	$$PWD/touchlistedit.h \
	$$PWD/listselectiondialog.h

FORMS += $$PWD/listselectiondialog.ui

#-----------------------------------------------------------------------------
#-- Touch Numbers Edit
#-----------------------------------------------------------------------------
SOURCES += \
	$$PWD/numberedit.cpp \
	$$PWD/doubleedit.cpp \
	$$PWD/numbereditdialog.cpp

HEADERS += \
	$$PWD/numberedit.h \
	$$PWD/doubleedit.h \
	$$PWD/numbereditdialog.h

FORMS += $$PWD/numbereditdialog.ui

