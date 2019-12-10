#include "O3DWidget.h"
#include <osg/Camera>
#include <osg/DisplaySettings>
#include <osg/Geode>
#include <osg/Material>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/StateSet>

#include <osgDB/WriteFile>
#include <osgGA/EventQueue>
#include <osgGA/TrackballManipulator>
#include <osgGA/StateSetManipulator>
#include <osgUtil/IntersectionVisitor>
#include <osgUtil/PolytopeIntersector>
#include <osgViewer/View>
#include <osgViewer/ViewerEventHandlers>
#include <osgDB/ReaderWriter>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <cassert>

#include <stdexcept>
#include <vector>

#include <QDebug>
#include <QKeyEvent>
#include <QPainter>
#include <QWheelEvent>

namespace
{

#ifdef WITH_SELECTION_PROCESSING
QRect makeRectangle( const QPoint& first, const QPoint& second )
{
  if( second.x() >= first.x() && second.y() >= first.y() )
    return QRect( first, second );
  else if( second.x() < first.x() && second.y() >= first.y() )
    return QRect( QPoint( second.x(), first.y() ), QPoint( first.x(), second.y() ) );
  else if( second.x() < first.x() && second.y() < first.y() )
    return QRect( second, first );
  else if( second.x() >= first.x() && second.y() < first.y() )
    return QRect( QPoint( first.x(), second.y() ), QPoint( second.x(), first.y() ) );

  return QRect();
}
#endif

}

namespace osgWidget
{
  void Viewer::setupThreading()
  {
    if( _threadingModel == SingleThreaded )
    {
      if(_threadsRunning)
        stopThreading();
    }
    else
    {
      if(!_threadsRunning)
        startThreading();
    }
  }
}

O3DWidget::O3DWidget( QWidget* parent,
                      Qt::WindowFlags f )
  : QOpenGLWidget( parent,
                   f )
  , _compositeViewer( new osgWidget::Viewer )
  , _selectionActive( false )
  , _selectionFinished( true )
{
  osg::GraphicsContext::Traits *traits = new osg::GraphicsContext::Traits;
  QSurfaceFormat fmt = this->format();

  traits->red = fmt.redBufferSize();
  traits->red = fmt.redBufferSize();
  traits->green = fmt.greenBufferSize();
  traits->blue = fmt.blueBufferSize();
  traits->alpha = fmt.alphaBufferSize();
  traits->depth = fmt.depthBufferSize();
  traits->stencil = fmt.stencilBufferSize();

  traits->sampleBuffers = fmt.samples() == 0 ? 0 : 1;
  traits->samples = fmt.samples();

  traits->quadBufferStereo = fmt.stereo();
  traits->doubleBuffer = fmt.swapBehavior() == QSurfaceFormat::DoubleBuffer;

  traits->vsync = fmt.swapInterval() >= 1;

  qDebug() << this->width() << " " << this->height();
  traits->x = this->x();
  traits->y = this->y();
  traits->width = this->width();
  traits->height = this->height();

  _graphicsWindow = new osgViewer::GraphicsWindowEmbedded(traits);

  float aspectRatio = static_cast<float>( this->width() ) / static_cast<float>( this->height() );
  auto pixelRatio   = this->devicePixelRatio();

  osg::Camera* camera = new osg::Camera;
  camera->setViewport( 0, 0, this->width() * pixelRatio, this->height() * pixelRatio );
  camera->setClearColor( osg::Vec4( 0.f, 0.f, 0.f, 1.f ) );
  camera->setProjectionMatrixAsPerspective( 30.f, aspectRatio, 1.f, 1000.f );
  camera->setGraphicsContext( _graphicsWindow );

  _view = new osgViewer::View;
  _view->setCamera( camera );
  _view->addEventHandler( new osgViewer::StatsHandler );
  _view->addEventHandler( new osgViewer::WindowSizeHandler );
  _view->addEventHandler(new osgViewer::ThreadingHandler());
  _view->addEventHandler(new osgViewer::LODScaleHandler());

#ifdef WITH_PICK_HANDLER
  view->addEventHandler( new PickHandler( this->devicePixelRatio() ) );
#endif

  _manipulator = new osgGA::TrackballManipulator;
  _manipulator->setAllowThrow(true);

  _view->setCameraManipulator(_manipulator);
  _view->addEventHandler(new osgGA::StateSetManipulator(_view->getCamera()->getOrCreateStateSet()));

  _compositeViewer->addView(_view);
  _compositeViewer->setThreadingModel(osgViewer::CompositeViewer::SingleThreaded);

  
  //_compositeViewer->realize();
  this->setFocusPolicy( Qt::StrongFocus );
  this->setMinimumSize( 100, 100 );

  this->setMouseTracking( true );
  connect( &_timer, SIGNAL(timeout()), this, SLOT(update()) );
  _timer.start(20);
}

O3DWidget::~O3DWidget()
{
}

void O3DWidget::paintEvent( QPaintEvent* /* paintEvent */ )
{
  this->makeCurrent();

  QPainter painter( this );
  painter.setRenderHint( QPainter::Antialiasing );

  this->paintGL();

#ifdef WITH_SELECTION_PROCESSING
  if( selectionActive_ && !selectionFinished_ )
  {
    painter.setPen( Qt::black );
    painter.setBrush( Qt::transparent );
    painter.drawRect( makeRectangle( selectionStart_, selectionEnd_ ) );
  }
#endif

  painter.end();
  this->doneCurrent();
}

void O3DWidget::paintGL()
{
	_compositeViewer->frame();
}

void O3DWidget::resizeGL( int width, int height )
{
  this->getEventQueue()->windowResize( this->x(), this->y(), width, height );
  _graphicsWindow->resized( this->x(), this->y(), width, height );

  this->onResize(width, height);
}

void O3DWidget::keyPressEvent( QKeyEvent* event )
{
  QString keyString   = event->text();
  const char* keyData = keyString.toLocal8Bit().data();

  if( event->key() == Qt::Key_S )
  {
#ifdef WITH_SELECTION_PROCESSING
    selectionActive_ = !selectionActive_;
#endif

    // Further processing is required for the statistics handler here, so we do
    // not return right away.
  }
  else if( event->key() == Qt::Key_D )
  {
    osgDB::writeNodeFile( *_view->getSceneData(),
                          "/tmp/sceneGraph.osg" );

    return;
  }
  else if( event->key() == Qt::Key_H )
  {
    this->onHome();
    return;
  }

  this->getEventQueue()->keyPress( osgGA::GUIEventAdapter::KeySymbol( *keyData ) );
}

void O3DWidget::keyReleaseEvent( QKeyEvent* event )
{
  QString keyString   = event->text();
  const char* keyData = keyString.toLocal8Bit().data();

  this->getEventQueue()->keyRelease( osgGA::GUIEventAdapter::KeySymbol( *keyData ) );
}

void O3DWidget::mouseMoveEvent( QMouseEvent* event )
{

  if( _selectionActive && event->buttons() & Qt::LeftButton )
  {
    _selectionEnd = event->pos();

    this->update();
  }
  else
  {
    auto pixelRatio = this->devicePixelRatio();

    this->getEventQueue()->mouseMotion( static_cast<float>( event->x() * pixelRatio ),
                                        static_cast<float>( event->y() * pixelRatio ) );
  }
}

void O3DWidget::mousePressEvent( QMouseEvent* event )
{
  if( _selectionActive && event->button() == Qt::LeftButton )
  {
    _selectionStart    = event->pos();
    _selectionEnd      = _selectionStart;
    _selectionFinished = false;
  }
  else
  {
    unsigned int button = 0;

    switch( event->button() )
    {
    case Qt::LeftButton:
      button = 1;
      break;

    case Qt::MiddleButton:
      button = 2;
      break;

    case Qt::RightButton:
      button = 3;
      break;

    default:
      break;
    }

    auto pixelRatio = this->devicePixelRatio();

    this->getEventQueue()->mouseButtonPress( static_cast<float>( event->x() * pixelRatio ),
                                             static_cast<float>( event->y() * pixelRatio ),
                                             button );
    }
}

void O3DWidget::mouseReleaseEvent(QMouseEvent* event)
{
  if( _selectionActive && event->button() == Qt::LeftButton )
  {
    _selectionEnd      = event->pos();
    _selectionFinished = true;

    this->processSelection();
  }

  else
  {
    unsigned int button = 0;

    switch( event->button() )
    {
    case Qt::LeftButton:
      button = 1;
      break;

    case Qt::MiddleButton:
      button = 2;
      break;

    case Qt::RightButton:
      button = 3;
      break;

    default:
      break;
    }

    auto pixelRatio = this->devicePixelRatio();

    this->getEventQueue()->mouseButtonRelease( static_cast<float>( pixelRatio * event->x() ),
                                               static_cast<float>( pixelRatio * event->y() ),
                                               button );
  }
}

void O3DWidget::wheelEvent( QWheelEvent* event )
{
  if( _selectionActive )
    return;

  event->accept();
  int delta = event->delta();

  osgGA::GUIEventAdapter::ScrollingMotion motion = delta > 0 ?   osgGA::GUIEventAdapter::SCROLL_UP
                                                               : osgGA::GUIEventAdapter::SCROLL_DOWN;

  this->getEventQueue()->mouseScroll( motion );
}

bool O3DWidget::event( QEvent* event )
{
  bool handled = QOpenGLWidget::event( event );

  // This ensures that the OSG widget is always going to be repainted after the
  // user performed some interaction. Doing this in the event handler ensures
  // that we don't forget about some event and prevents duplicate code.
  switch( event->type() )
  {
  case QEvent::KeyPress:
  case QEvent::KeyRelease:
  case QEvent::MouseButtonDblClick:
  case QEvent::MouseButtonPress:
  case QEvent::MouseButtonRelease:
  case QEvent::MouseMove:
  case QEvent::Wheel:
    this->update();
    break;

  default:
    break;
  }

  return handled;
}

void O3DWidget::setSceneData(osg::Node* node)
{
	osg::Group* root = new osg::Group;
	root->addChild(node);

	_view->setSceneData(root);
	root->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
	root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);
}


void O3DWidget::onHome()
{
  osgViewer::ViewerBase::Views views;
  _compositeViewer->getViews( views );

  for( std::size_t i = 0; i < views.size(); i++ )
  {
    osgViewer::View* view = views.at(i);
    view->home();
  }
}

void O3DWidget::onResize( int width, int height )
{
  auto pixelRatio = this->devicePixelRatio();

  _view->getCamera()->setViewport( 0, 0, width * pixelRatio, height * pixelRatio );
}

osgGA::EventQueue* O3DWidget::getEventQueue() const
{
  osgGA::EventQueue* eventQueue = _graphicsWindow->getEventQueue();

  if( eventQueue )
    return eventQueue;
  else
    throw std::runtime_error( "Unable to obtain valid event queue");
}

void O3DWidget::processSelection()
{
#ifdef WITH_SELECTION_PROCESSING
  QRect selectionRectangle = makeRectangle( selectionStart_, selectionEnd_ );
  auto widgetHeight        = this->height();
  auto pixelRatio          = this->devicePixelRatio();

  double xMin = selectionRectangle.left();
  double xMax = selectionRectangle.right();
  double yMin = widgetHeight - selectionRectangle.bottom();
  double yMax = widgetHeight - selectionRectangle.top();

  xMin *= pixelRatio;
  yMin *= pixelRatio;
  xMax *= pixelRatio;
  yMax *= pixelRatio;

  osgUtil::PolytopeIntersector* polytopeIntersector
      = new osgUtil::PolytopeIntersector( osgUtil::PolytopeIntersector::WINDOW,
                                          xMin, yMin,
                                          xMax, yMax );

  // This limits the amount of intersections that are reported by the
  // polytope intersector. Using this setting, a single drawable will
  // appear at most once while calculating intersections. This is the
  // preferred and expected behaviour.
  polytopeIntersector->setIntersectionLimit( osgUtil::Intersector::LIMIT_ONE_PER_DRAWABLE );

  osgUtil::IntersectionVisitor iv( polytopeIntersector );

  for( unsigned int viewIndex = 0; viewIndex < viewer_->getNumViews(); viewIndex++ )
  {
    qDebug() << "View index:" << viewIndex;

    osgViewer::View* view = viewer_->getView( viewIndex );

    if( !view )
      throw std::runtime_error( "Unable to obtain valid view for selection processing" );

    osg::Camera* camera = view->getCamera();

    if( !camera )
      throw std::runtime_error( "Unable to obtain valid camera for selection processing" );

    camera->accept( iv );

    if( !polytopeIntersector->containsIntersections() )
      continue;

    auto intersections = polytopeIntersector->getIntersections();

    for( auto&& intersection : intersections )
      qDebug() << "Selected a drawable:" << QString::fromStdString( intersection.drawable->getName() );
  }
#endif
}


osgViewer::View* O3DWidget::viewer()
{
    return _view.get();
}


osgGA::TrackballManipulator* O3DWidget::getManipulator()
{
	return _manipulator;
}
