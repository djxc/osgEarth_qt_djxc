#include "osgcontainer.h"
#include <QInputEvent>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgGA/MultiTouchTrackballManipulator>
#include <osgGA/StateSetManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <QApplication>

OsgContainer::OsgContainer(QWidget *parent)
    : QOpenGLWidget(parent)
{
    init3D();
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

OsgContainer::~OsgContainer()
{

}

bool OsgContainer::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::TouchBegin:
    case QEvent::TouchEnd:
    case QEvent::TouchUpdate: {
        QList<QTouchEvent::TouchPoint> touchPoints = static_cast<QTouchEvent *>(event)->touchPoints();
        unsigned int id = 0;
        unsigned int tapCount = touchPoints.size();

        osg::ref_ptr<osgGA::GUIEventAdapter> osgEvent(NULL);
        osgGA::GUIEventAdapter::TouchPhase phase = osgGA::GUIEventAdapter::TOUCH_UNKNOWN;
        foreach (const QTouchEvent::TouchPoint& touchPoint, touchPoints) {
            if (!osgEvent) {
                if (event->type() == QEvent::TouchBegin) {
                    phase = osgGA::GUIEventAdapter::TOUCH_BEGAN;
                    osgEvent = window->getEventQueue()->touchBegan(id, osgGA::GUIEventAdapter::TOUCH_BEGAN, touchPoint.pos().x(), touchPoint.pos().y());
                } else if (event->type() == QEvent::TouchEnd) {
                    phase = osgGA::GUIEventAdapter::TOUCH_ENDED;
                    osgEvent = window->getEventQueue()->touchEnded(id, osgGA::GUIEventAdapter::TOUCH_ENDED, touchPoint.pos().x(), touchPoint.pos().y(), tapCount);
                } else if (event->type() == QEvent::TouchUpdate) {
                    phase = osgGA::GUIEventAdapter::TOUCH_MOVED;
                    osgEvent = window->getEventQueue()->touchMoved(id, osgGA::GUIEventAdapter::TOUCH_MOVED, touchPoint.pos().x(), touchPoint.pos().y());
                }
            } else {
                osgEvent->addTouchPoint(id, osgGA::GUIEventAdapter::TOUCH_ENDED, touchPoint.pos().x(), touchPoint.pos().y());
                osgEvent->addTouchPoint(id, phase, touchPoint.pos().x(), touchPoint.pos().y());
            }
            id++;
        }
        break;
    }
    default:
        break;
    }
    return QOpenGLWidget::event(event);
}

void OsgContainer::setKeyboardModifiers(QInputEvent *event)
{
    int modkey = event->modifiers() & (Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier);
    unsigned int mask = 0;
    if (modkey & Qt::ShiftModifier) {
        mask |= osgGA::GUIEventAdapter::MODKEY_SHIFT;
    }
    if (modkey & Qt::ControlModifier) {
        mask |= osgGA::GUIEventAdapter::MODKEY_CTRL;
    }
    if (modkey & Qt::AltModifier) {
        mask |= osgGA::GUIEventAdapter::MODKEY_ALT;
    }

    window->getEventQueue()->getCurrentEventState()->setModKeyMask(mask);
    update();
}

void OsgContainer::keyPressEvent(QKeyEvent *event)
{
    setKeyboardModifiers(event);
    window->getEventQueue()->keyPress(event->key());
    QOpenGLWidget::keyPressEvent(event);
    update();
}

void OsgContainer::keyReleaseEvent(QKeyEvent *event)
{
    setKeyboardModifiers(event);
    window->getEventQueue()->keyRelease(event->key());
    QOpenGLWidget::keyReleaseEvent(event);
    update();
}

void OsgContainer::mousePressEvent(QMouseEvent *event)
{
    int button = 0;
    switch (event->button()) {
    case Qt::LeftButton: button = 1; break;
    case Qt::MidButton: button = 2; break;
    case Qt::RightButton: button = 3; break;
    case Qt::NoButton: button = 0; break;
    default: button = 0; break;
    }
    setKeyboardModifiers(event);
    window->getEventQueue()->mouseButtonPress(event->x(), event->y(), button);
    update();
}

void OsgContainer::mouseReleaseEvent(QMouseEvent *event)
{
    int button = 0;
    switch (event->button()) {
    case Qt::LeftButton: button = 1; break;
    case Qt::MidButton: button = 2; break;
    case Qt::RightButton: button = 3; break;
    case Qt::NoButton: button = 0; break;
    default: button = 0; break;
    }
    setKeyboardModifiers(event);
    window->getEventQueue()->mouseButtonRelease(event->x(), event->y(), button);

    QOpenGLWidget::mouseReleaseEvent(event);
    update();
}

void OsgContainer::mouseDoubleClickEvent(QMouseEvent *event)
{
    int button = 0;
    switch (event->button()) {
    case Qt::LeftButton: button = 1; break;
    case Qt::MidButton: button = 2; break;
    case Qt::RightButton: button = 3; break;
    case Qt::NoButton: button = 0; break;
    default: button = 0; break;
    }
    setKeyboardModifiers(event);
    window->getEventQueue()->mouseDoubleButtonPress(event->x(), event->y(), button);

    QOpenGLWidget::mouseDoubleClickEvent(event);
    update();
}

void OsgContainer::mouseMoveEvent(QMouseEvent *event)
{
    setKeyboardModifiers(event);
    window->getEventQueue()->mouseMotion(event->x(), event->y());
    QOpenGLWidget::mouseMoveEvent(event);
    update();
}

void OsgContainer::wheelEvent(QWheelEvent *event)
{
    setKeyboardModifiers(event);
    window->getEventQueue()->mouseScroll(
                event->orientation() == Qt::Vertical ?
                    (event->delta() > 0 ? osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN) :
                    (event->delta() > 0 ? osgGA::GUIEventAdapter::SCROLL_LEFT : osgGA::GUIEventAdapter::SCROLL_RIGHT));
    QOpenGLWidget::wheelEvent(event);
    update();
}

void OsgContainer::resizeEvent(QResizeEvent *event)
{
    const QSize& size = event->size();
    window->resized(x(), y(), size.width(), size.height());
    window->getEventQueue()->windowResize(x(), y(), size.width(), size.height());
    window->requestRedraw();

//    const QSize& oldSize = event->oldSize();
//    int oldWidth = oldSize.width();
//    int oldHeight = oldSize.height();

//    int newWidth = size.width();
//    int newHeight = size.height();

//    double widthChangeRatio = double(newWidth) / double(oldWidth);
//    double heigtChangeRatio = double(newHeight) / double(oldHeight);
//    double aspectRatioChange = widthChangeRatio / heigtChangeRatio;
    QOpenGLWidget::resizeEvent(event);
}

void OsgContainer::moveEvent(QMoveEvent *event)
{
    const QPoint& pos = event->pos();
    window->resized(pos.x(), pos.y(), width(), height());
    window->getEventQueue()->windowResize(pos.x(), pos.y(), width(), height());

    QOpenGLWidget::moveEvent(event);
}

void OsgContainer::timerEvent(QTimerEvent *)
{
    update();
}

void OsgContainer::paintGL()
{
    if (isVisibleTo(QApplication::activeWindow())) {
        frame();
    }
}

void OsgContainer::init3D()
{
    root = new osg::Group;
    root->setName("Root");

//    setCamera(createCamera(0, 0, width(), height()));
    osg::ref_ptr<osgGA::TrackballManipulator> manipulator = new osgGA::TrackballManipulator;

    setCameraManipulator(manipulator);
    addEventHandler(new osgViewer::StatsHandler);
    addEventHandler(new osgViewer::ThreadingHandler());
    addEventHandler(new osgViewer::HelpHandler);
    addEventHandler(new osgGA::StateSetManipulator(this->getCamera()->getOrCreateStateSet()));
    addEventHandler(new osgViewer::WindowSizeHandler);   //窗口大小变化事件
    setThreadingModel(osgViewer::Viewer::SingleThreaded);

    root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);
    root->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
    setSceneData(root);

//    setRunFrameScheme(ON_DEMAND);

    startTimer(10);
}

osg::ref_ptr<osg::Camera> OsgContainer::createCamera(int x, int y, int w, int h)
{
    window = new osgViewer::GraphicsWindowEmbedded(x, y, w, h);
//    osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->windowDecoration = true;
    traits->x = x;
    traits->y = y;
    traits->width = w;
    traits->height = h;
    traits->doubleBuffer = true;
    traits->sharedContext = 0;

    osg::ref_ptr<osg::Camera> camera = new osg::Camera;
    camera->setGraphicsContext(window);
    camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
    camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera->setProjectionMatrixAsPerspective(
                30.0f, double(traits->width) / double(traits->height), 1.0f, 10000.0f);
    camera->setClearColor(osg::Vec4(0.3, 0.3, 0.6, 0.1));

    return camera.release();
}
