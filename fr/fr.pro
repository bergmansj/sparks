#-------------------------------------------------
#
# Project created by QtCreator 2011-07-19T09:50:47
#
#-------------------------------------------------

QT       += core gui

TARGET = fr
TEMPLATE = app

INCLUDEPATH += "/opt/local/include/"

LIBS += "/opt/local/lib/libopencv_calib3d.dylib"
LIBS += "/opt/local/lib/libopencv_contrib.dylib"
LIBS += "/opt/local/lib/libopencv_core.dylib"
LIBS += "/opt/local/lib/libopencv_features2d.dylib"
LIBS += "/opt/local/lib/libopencv_flann.dylib"
LIBS += "/opt/local/lib/libopencv_gpu.dylib"
LIBS += "/opt/local/lib/libopencv_highgui.dylib"
LIBS += "/opt/local/lib/libopencv_imgproc.dylib"
LIBS += "/opt/local/lib/libopencv_legacy.dylib"
LIBS += "/opt/local/lib/libopencv_ml.dylib"
LIBS += "/opt/local/lib/libopencv_objdetect.dylib"
LIBS += "/opt/local/lib/libopencv_video.dylib"

SOURCES += main.cpp\
        gui/mainwindow.cpp \
        \
        recognition/facerecognition.cpp \
        recognition/facedetection.cpp \
        \
        \ # The following files are from an external resource, see the files for information
        recognition/recognizer/eigenfacerecognizer.cpp \
    imageconversion.cpp

HEADERS  += \
        gui/mainwindow.h \
        \
        recognition/facerecognition.h \
        recognition/facedetection.h \
        \
        \ # The following files are from an external resource, see the files for information
        recognition/recognizer/recognizer.h \
        recognition/recognizer/eigenfacerecognizer.h \
    imageconversion.h

FORMS    += \
        gui/mainwindow.ui
