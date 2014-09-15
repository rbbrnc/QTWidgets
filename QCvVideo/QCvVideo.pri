INCLUDEPATH += $$PWD

HEADERS += QCvVideo.h
SOURCES += QCvVideo.cpp

#-- OpevCV
OPENCV_CXXFLAGS = $$system(pkg-config opencv --cflags)
OPENCV_LDFLAGS  = $$system(pkg-config opencv --libs)
QMAKE_CXXFLAGS += $$OPENCV_CXXFLAGS
LIBS += $$OPENCV_LDFLAGS

#-- Only needed libraries
#OPENCV_PATH = "/opt/opencv"
#INCLUDEPATH += $$OPENCV_PATH/include/opencv
#LIBS += -L$$OPENCV_PATH/lib -lopencv_core -lopencv_highgui -lopencv_imgproc
