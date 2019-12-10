#include "mainwindow.h"
#include <QApplication>

#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/DrawPixels>
#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>
#include "osgcontainer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
//    o2dosgwidget w;
    w.show();

    return a.exec();
}
