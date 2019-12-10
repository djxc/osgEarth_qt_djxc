#ifndef OSGWidget_h__
#define OSGWidget_h__

#include <QPoint>
#include <QOpenGLWidget>
#include <QTimer>

#include <osg/ref_ptr>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/CompositeViewer>
#include <osgViewer/Viewer>

#include <osgGA/TrackballManipulator>

namespace osgWidget
{
  class Viewer : public osgViewer::CompositeViewer
  {
    public:
	    virtual void setupThreading();
  };
}

class O3DWidget : public QOpenGLWidget
{
  Q_OBJECT
public:
  O3DWidget( QWidget* parent = 0,
             Qt::WindowFlags f = 0 );

  virtual ~O3DWidget();

public:
  osgViewer::View* viewer();
  osgGA::TrackballManipulator* getManipulator();
protected:

  virtual void paintEvent( QPaintEvent* paintEvent );
  virtual void paintGL();
  virtual void resizeGL( int width, int height );

  virtual void keyPressEvent( QKeyEvent* event );
  virtual void keyReleaseEvent( QKeyEvent* event );

  virtual void mouseMoveEvent( QMouseEvent* event );
  virtual void mousePressEvent( QMouseEvent* event );
  virtual void mouseReleaseEvent( QMouseEvent* event );
  virtual void wheelEvent( QWheelEvent* event );

  virtual bool event( QEvent* event );

public slots:
	void setSceneData(osg::Node * node);


private:
  virtual void onHome();
  virtual void onResize( int width, int height );
  void processSelection();
  osgGA::EventQueue* getEventQueue() const;

  osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> _graphicsWindow;

  osg::ref_ptr<osgWidget::Viewer> _compositeViewer;
  osg::ref_ptr<osgViewer::View> _view;
  osg::ref_ptr<osg::Group> _dataRoot;

  osg::ref_ptr<osgGA::TrackballManipulator> _manipulator;


  QPoint _selectionStart;
  QPoint _selectionEnd;

  bool _selectionActive;
  bool _selectionFinished;

  QTimer _timer;
};

#endif
