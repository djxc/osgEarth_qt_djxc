#-------------------------------------------------
#
# Project created by QtCreator 2019-11-06T13:50:45
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = demoOSG
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += /usr/local/include/
LIBS +=/usr/local/lib64/libosgViewer.so.158
LIBS +=/usr/local/lib64/libosgDB.so.158
LIBS +=/usr/local/lib64/libosg.so.158
LIBS +=/usr/local/lib64/libosgGA.so.158
LIBS +=/usr/local/lib64/libosgEarth.so.2.10.1

SOURCES += \
        main.cpp \
        mainwindow.cpp \
#    osgqt/GraphicsWindowQt.cpp \
#    osgqt/QFontImplementation.cpp \
#    osgqt/QGraphicsViewAdapter.cpp \
#    osgqt/QWidgetImage.cpp \
#    osgqt/ReaderQFont.cpp \
    osgcontainer.cpp \
    O3DWidget.cpp

HEADERS += \
        mainwindow.h \  
#    osgqt/GraphicsWindowQt.h \
#    osgqt/QFontImplementation.h \
#    osgqt/QGraphicsViewAdapter.h \
#    osgqt/QWidgetImage.h \
    osgcontainer.h \
    O3DWidget.h

DISTFILES += \
    README.md

