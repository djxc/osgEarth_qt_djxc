#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <osgEarth/Map>
#include <osgEarth/GDAL>
#include <osgEarth/TMS>
#include <osgEarth/MapNode>
#include <osgEarth/ImageLayer>
#include <osgEarth/EarthManipulator>
#include <osgEarth/OGRFeatureSource>
#include <osgEarth/FeatureImageLayer>
#include <osgEarth/GeoTransform>

class LayerManager
{

public:
    LayerManager();
    osg::ref_ptr<osgEarth::TMSImageLayer> createBaseLayer();
    osg::ref_ptr<osgEarth::GDALImageLayer> creatTIFLayer();
    ~LayerManager();

private:
    void initOsg();
};


#endif // MAINWINDOW_H
