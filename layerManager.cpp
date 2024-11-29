#include "layerManager.h"


#include <osg/Geode>
#include <osg/Texture2D>
#include <osgGA/TrackballManipulator>
#include <osgGA/StateSetManipulator>
#include <osgDB/ReadFile>
#include <osg/DrawPixels>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/ArgumentParser>
#include <osg/MatrixTransform>

//#include <gdal_priv.h>
//#include <ogr_api.h>
//#include <ogr_core.h>
//#include <ogr_feature.h>
//#include <ogr_geometry.h>
//#include <ogrsf_frmts.h>

#include "osgcontainer.h"
#include "O3DWidget.h"


//using namespace osgEarth;

#include <string>

using namespace std;

LayerManager::LayerManager()
{
//    osgEarth::initialize();
//    resize(800, 500);


//    Map* map = new Map();
//    osgEarth::initialize();
//    osgEarth::Drivers::TMSOptions tms;
//    tms.url() =
    "http://labs.metacarta.com/wms-c/Basic.py/1.0.0/satellite/";
//    ImageLayer* layer = new ImageLayer( "NASA", tms );
//    auto layer = new osgEarth::TMSImageLayer();
//    osgEarth::Drivers::GDALOptions tms1;
//    tms1.url() ="/home/djxc/2019/data/world.tif";
//    gdal.url()="D:/Documents/Visual Studio2010/Projects/test3/test3/data/world.tif";
//    ImageLayer* layer1 = new ImageLayer( "NASA", tms1 );
//    map->addLayer( layer1 );

//    auto imagery = new osgEarth::TMSImageLayer();
//    imagery->setURL("https://readymap.org/readymap/tiles/1.0.0/7/");
////    map->addLayer(imagery);
//    auto mapNode = new osgEarth::MapNode();
//    mapNode->getMap()->addLayer(imagery);

    // map(empty)
//    osg::ref_ptr<osgEarth::Map> map = new osgEarth::Map;
//    osg::ref_ptr<osgEarth::MapNode> mapNode = new osgEarth::MapNode(map);

//    // base layer
//    osg::ref_ptr<osgEarth::GDALImageLayer> baselayer = new osgEarth::GDALImageLayer();
//    baselayer->setURL("D:\\code\\c\\osgearth\\data\\world.tif");
//    map->addLayer(baselayer);

    // viewer
//    osgViewer::Viewer viewer;
//    viewer.setSceneData(mapNode);

//    osg::ref_ptr<osgEarth::Util::EarthManipulator> mainManipulator = new osgEarth::Util::EarthManipulator;
//    viewer.setCameraManipulator(mainManipulator);

//    O3DWidget* p3d = new O3DWidget(this);
//    setCentralWidget(p3d);

////    osg::ref_ptr<osg::Node> dat = osgDB::readNodeFile("D:\\code\\c\\simple.earth");
//    osg::Node* globe = osgDB::readNodeFile("D:\\code\\c\\simple.earth");
//    osgEarth::MapNode* mapNode = osgEarth::MapNode::get(globe);
////    p3d->setSceneData(dat);
//    p3d->setSceneData(mapNode);
}

/**
 * 加载底图
 * @brief LayerManager::createBaseLayer
 * @return
 */
osg::ref_ptr<osgEarth::TMSImageLayer> LayerManager::createBaseLayer() {
//    osg::Node* globe = osgDB::readNodeFile("D:\\code\\c\\osgEarth\\tests\\simple.earth");
//    mapNode->addChild(globe);
    string source = "https://readymap.org/readymap/tiles/1.0.0/7/";
//            "https://api.mapbox.com/v4/mapbox.satellite/{z}/{x}/{y}.png?access_token=pk.eyJ1IjoiZGp4YyIsImEiOiJjanlxdzNlbW0wNHNyM29yMzZibHczOHlpIn0.TOUXgB6IHHhnmA6RsL6pWw";
    osg::ref_ptr<osgEarth::TMSImageLayer> someLayer = new osgEarth::TMSImageLayer();
    someLayer->setURL(source);
    return someLayer;
}

void createEarthLayer() {

//    OGRRegisterAll();
//    GDALAllRegister();
//    CPLSetConfigOption("GDAL_DATA", "../../Data/gdal_data");
//    CPLSetConfigOption("CPL_DEBUG", "YES");
//    CPLSetConfigOption("CPL_LOG", "../LOG/gdal.log");

//    osgEarth::initialize();
//    // map
//    osg::Node* globe = osgDB::readNodeFile("D:\\code\\c\\simple.earth");
//    osgEarth::MapNode* mapNode = osgEarth::MapNode::get(globe);

//    // viewer
//    osgViewer::Viewer viewer;
//    viewer.setSceneData(mapNode);

//    // manipulator
//    osg::ref_ptr<osgEarth::Util::EarthManipulator> mainManipulator = new osgEarth::Util::EarthManipulator;
//    viewer.setCameraManipulator(mainManipulator);

//    // run
//    viewer.setUpViewInWindow(100, 100, 800, 600);
//    viewer.run();
}

osg::ref_ptr<osgEarth::GDALImageLayer> LayerManager::creatTIFLayer() {
    // base layer
    osg::ref_ptr<osgEarth::GDALImageLayer> tifLayer = new osgEarth::GDALImageLayer();
    tifLayer->setURL("E:\\Data\\RS\\RSData\\莱西.tif");
    auto extent = tifLayer->getExtent();
    printf("%f", extent.xMax());
    return tifLayer;
}

void LayerManager::initOsg() {
    printf("djxc");
//    OsgContainer* osgViewer=new OsgContainer(this);
//    osgViewer::Viewer* viewer = osgViewer->getOSGViewer();

////    osg::ref_ptr < osg::Node> b25 = osgDB::readNodeFile("/home/djxc/2019/data/cow.osg") ;
//    osg::ref_ptr < osg::MatrixTransform> scale = new osg::MatrixTransform ;

//    scale ->setMatrix(osg::Matrix::rotate(osg::DegreesToRadians(45.0), 1, 0, 0)*osg::Matrix::scale(0.5, 0.5, 0.5)*osg::Matrix::translate(4, 0, -2)) ;


//    osg::Group *root = osgViewer->getRoot();

////    scale ->addChild(b25.get()) ;
//    root->addChild(scale.get());

//    viewer->setSceneData(root);
//    viewer->realize();

//    viewer->run();
//    this->setCentralWidget(osgViewer);
}


LayerManager::~LayerManager()
{

}
