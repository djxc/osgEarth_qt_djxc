#include "mainwindow.h"
#include <osg/Texture2D>


#include <osgGA/TrackballManipulator>
#include <osgGA/StateSetManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/DrawPixels>
#include <osg/Geode>
#include <osg/MatrixTransform>

#include <osgEarth/Map>
#include <osgEarth/MapNode>
#include <osgEarthDrivers/tms/TMSOptions>
#include <osgEarth/ImageLayer>
#include <osgEarthDrivers/gdal/GDALOptions>

#include "osgcontainer.h"
#include "O3DWidget.h"

using namespace osgEarth;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    resize(800, 500);
    Map* map = new Map();
    osgEarth::Drivers::TMSOptions tms;
    tms.url() =
    "http://labs.metacarta.com/wms-c/Basic.py/1.0.0/satellite/";
    ImageLayer* layer = new ImageLayer( "NASA", tms );
    osgEarth::Drivers::GDALOptions tms1;
    tms1.url() ="/home/djxc/2019/data/world.tif";
//    gdal.url()="D:/Documents/Visual Studio2010/Projects/test3/test3/data/world.tif";
    ImageLayer* layer1 = new ImageLayer( "NASA", tms1 );
    map->addLayer( layer1 );
    MapNode* mapNode = new MapNode( map );

    //O2DWidget* p2d = new O2DWidget(this);
    O3DWidget* p3d = new O3DWidget(this);
    setCentralWidget(p3d);

    osg::ref_ptr<osg::Node> dat = osgDB::readNodeFile("/home/djxc/2019/data/feature_geom.earth");
//    p3d->setSceneData(dat);
    p3d->setSceneData(mapNode);


}

void MainWindow::initOsg() {
    printf("djxc");
    OsgContainer* osgViewer=new OsgContainer(this);
    osgViewer::Viewer* viewer = osgViewer->getOSGViewer();

//    osg::ref_ptr < osg::Node> b25 = osgDB::readNodeFile("/home/djxc/2019/data/cow.osg") ;
    osg::ref_ptr < osg::MatrixTransform> scale = new osg::MatrixTransform ;

    scale ->setMatrix(osg::Matrix::rotate(osg::DegreesToRadians(45.0), 1, 0, 0)*osg::Matrix::scale(0.5, 0.5, 0.5)*osg::Matrix::translate(4, 0, -2)) ;


    osg::Group *root = osgViewer->getRoot();

//    scale ->addChild(b25.get()) ;
    root->addChild(scale.get());

    viewer->setSceneData(root);
    viewer->realize();

    viewer->run();
    this->setCentralWidget(osgViewer);
}


MainWindow::~MainWindow()
{

}
