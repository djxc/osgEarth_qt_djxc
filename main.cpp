#include <QApplication>

#include <iostream>

#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgViewer/ViewerEventHandlers>
#include <osg/DrawPixels>
#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osgGA/StateSetManipulator>
#include <osg/Vec3d>
#include <osg/ArgumentParser>

#include <osgEarth/Common>
#include <osgEarth/MapNode>
#include <osgEarth/EarthManipulator>
#include <osgEarth/Viewpoint>
#include <osgEarth/TMS>

#include <gdal_priv.h>
#include <ogr_api.h>
#include <ogr_core.h>
#include <ogr_feature.h>
#include <ogr_geometry.h>
#include <ogrsf_frmts.h>



//#include "osgcontainer.h"
#include "layerManager.h"

using namespace std;

int main(int argc, char *argv[])
{
    osgEarth::initialize();

    osg::ArgumentParser args(&argc, argv);
    osgViewer::Viewer viewer(args);

    LayerManager* layerManger = new LayerManager();
    osg::ref_ptr<osgEarth::Layer> baseLayer = layerManger->createBaseLayer();
    osg::ref_ptr<osgEarth::Layer> tifLayer = layerManger->creatTIFLayer();

    auto mapNode = new osgEarth::MapNode();
    mapNode->getMap()->addLayer(baseLayer);
    mapNode->getMap()->addLayer(tifLayer);

    viewer.setSceneData(mapNode);
    viewer.setCameraManipulator(new osgEarth::EarthManipulator(args));
    // manipulator
    osg::ref_ptr<osgEarth::Util::EarthManipulator> mainManipulator = new osgEarth::Util::EarthManipulator;
    viewer.setCameraManipulator(mainManipulator);
    //视点定位北京地区
    mainManipulator->setViewpoint(osgEarth::Viewpoint("", 116, 40, 0.0, -2.50, -90.0, 1.5e6));
    viewer.setUpViewInWindow(100, 100, 800, 600);
    return viewer.run();


//        OGRRegisterAll();
//        GDALAllRegister();
//        CPLSetConfigOption("GDAL_DATA", "../../Data/gdal_data");
//        CPLSetConfigOption("CPL_DEBUG", "YES");
//        CPLSetConfigOption("CPL_LOG", "../LOG/gdal.log");

////    const osgEarth::SpatialReference* geoSRS = mapNode->getMapSRS()->getGeographicSRS();

////    //添加模型
////    {
////        osg::Node* model = osgDB::readNodeFile("D:\\code\\c\\osgearth\\data\\tree.osg");
////        //osg中光照只会对有法线的模型起作用，而模型经过缩放后法线是不会变得，
////        //所以需要手动设置属性，让法线随着模型大小变化而变化。GL_NORMALIZE 或 GL_RESCALE_NORMAL?
////        model->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON);

////        osg::Matrix Lmatrix;
////        //放大一些，方便看到
////        Lmatrix.preMult(osg::Matrix::scale(osg::Vec3(10000, 10000, 10000)));

////        osg::MatrixTransform* mt = new osg::MatrixTransform;
////        mt->setMatrix(Lmatrix);
////        mt->addChild(model);
////        root->addChild(mt);
////    }

}
