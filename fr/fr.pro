#-------------------------------------------------
#
# Project created by QtCreator 2011-07-19T09:50:47
#
#-------------------------------------------------

QT       += core gui

TARGET = fr
TEMPLATE = app

SOURCES += main.cpp \
        imageconversion.cpp\
        gui/mainwindow.cpp \
        \
        recognition/facerecognition.cpp \
        recognition/facedetection.cpp \
        recognition/recognizer/eigenfacerecognizer.cpp  # The following files are from an external resource, see the files for information

HEADERS  += \
        imageconversion.h \
        gui/mainwindow.h \
        \
        recognition/facerecognition.h \
        recognition/facedetection.h \
        recognition/recognizer/recognizer.h \           # The following files are from an external resource, see the files for information
        recognition/recognizer/eigenfacerecognizer.h    # The following files are from an external resource, see the files for information

FORMS    += \
        gui/mainwindow.ui

macx{
    message(macx)
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
}
unix{
    message(unix)
    INCLUDEPATH += "/usr/include/"

    LIBS += -L/usr/lib -lopencv_core
    LIBS += -lopencv_calib3d
    LIBS += -lopencv_contrib
    LIBS += -lopencv_features2d
    LIBS += -lopencv_flann
    LIBS += -lopencv_gpu
    LIBS += -lopencv_highgui
    LIBS += -lopencv_imgproc
    LIBS += -lopencv_legacy
    LIBS += -lopencv_ml
    LIBS += -lopencv_objdetect
    LIBS += -lopencv_video
}
win32{
    message(win32)
   # was not yet needed, so good luck figuring this out
}
