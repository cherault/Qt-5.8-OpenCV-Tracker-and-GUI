#-------------------------------------------------
#
# Project created by QtCreator 2017-05-19T13:30:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = opencvTracker
TEMPLATE = app

INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_videoio\
        -lopencv_imgproc -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d\
        -lopencv_objdetect -lopencv_flann -lopencv_tracking

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
