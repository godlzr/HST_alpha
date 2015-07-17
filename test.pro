#-------------------------------------------------
#
# Project created by QtCreator 2015-02-20T20:25:18
#
#-------------------------------------------------

QT       += core gui
QT       += testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    imageviewer.cpp \
    cvimage.cpp \
    gabor.cpp \
    enhancedgabor.cpp \
    matlabhelper.cpp

HEADERS  += mainwindow.h \
    croprect.h \
    imageviewer.h \
    cvimage.h \
    gabor.h \
    enhancedgabor.h \
    matlabhelper.h

FORMS    += mainwindow.ui \


INCLUDEPATH += /usr/local/include \
                /usr/local/include/opencv \
                /usr/local/include/opencv2

LIBS += -L/usr/local/lib \
        -lopencv_core \
        -lopencv_imgproc \
        -lopencv_features2d \
        -lopencv_highgui\
        -lopencv_calib3d \
        -lopencv_contrib \
        -lopencv_features2d \
        -lopencv_flann \
        -lopencv_gpu \
        -lopencv_legacy \
        -lopencv_ml \
        -lopencv_objdetect \
        -lopencv_video
