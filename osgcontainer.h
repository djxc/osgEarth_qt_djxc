#ifndef OSGCONTAINER_H
#define OSGCONTAINER_H

#include <QOpenGLWidget>
#include <osgViewer/Viewer>

class QInputEvent;

class OsgContainer : public QOpenGLWidget, public osgViewer::Viewer
{
    Q_OBJECT

public:
    OsgContainer(QWidget *parent = 0);
    ~OsgContainer();

    bool event(QEvent* event);

    void setKeyboardModifiers(QInputEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent* event);
    void timerEvent(QTimerEvent *);

    osgViewer::Viewer* getOSGViewer() { return this; }
    osg::Group* getRoot(){ return root; }

protected:
    virtual void paintGL();

private:
    void init3D();
    osg::ref_ptr<osg::Camera> createCamera(int x, int y, int w, int h);

private:
    osg::ref_ptr<osg::Group> root;
    osgViewer::GraphicsWindow* window;
};

//class OsgContainer : public QOpenGLWidget
//{
//public:
//    OsgContainer(QWidget *parent = 0);
//    ~OsgContainer();

//    void initializeGL();
//    void paintGL();
//    void resizeGL(int w, int h);
//};

#endif // OSGCONTAINER_H
