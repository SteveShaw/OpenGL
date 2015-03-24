#include "depthwidget.h"
#include <QMouseEvent>


DepthWidget::DepthWidget(QWidget *parent):
  QOpenGLWidget(parent),
  m_MeshEng(0),
  m_TrackBall(0.05f, QVector3D(0,1,0), TrackBall::Sphere),
  m_AngularSpeed(0)
{

}

DepthWidget::~DepthWidget()
{
  makeCurrent();
  delete m_MeshEng;
  foreach(const QString& key, m_TexMap.keys())
  {
    m_TexMap[key]->destroy();
    delete m_TexMap[key];
  }
  m_TexMap.clear();
  doneCurrent();
}

void DepthWidget::mousePressEvent(QMouseEvent *e)
{
//  m_MousePressPos = QVector2D(e->localPos());
  QWidget::mousePressEvent(e);
  if(e->isAccepted())
  {
    return;
  }

  if(e->button() & Qt::LeftButton)
  {
    m_TrackBall.push(pixelPosToViewPos(e->localPos()));
    e->accept();
  }
}

void DepthWidget::mouseReleaseEvent(QMouseEvent *e)
{
//  QVector2D diff = QVector2D(e->localPos()) - m_MousePressPos;

//   // Rotation axis is perpendicular to the mouse position difference
//   // vector
//   QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

//   // Accelerate angular speed relative to the length of the mouse sweep
//   qreal acc = diff.length() / 100.0;

//   // Calculate new rotation axis as weighted sum
//   m_RotAxis = (m_RotAxis * m_AngularSpeed + n * acc).normalized();

//   // Increase angular speed
  //   m_AngularSpeed += acc;
  QWidget::mouseReleaseEvent(e);
  if(e->isAccepted())
  {
    return;
  }

  if(e->button() & Qt::LeftButton)
  {
//    m_TrackBall.release(pixelPosToViewPos(e->localPos()),m_QuatRot);
    m_TrackBall.release(pixelPosToViewPos(e->localPos()),m_TrackBall.rotation());
    e->accept();
  }
}

void DepthWidget::mouseMoveEvent(QMouseEvent *e)
{
  QWidget::mouseMoveEvent(e);
  if(e->isAccepted())
  {
    return;
  }

  if(e->button() & Qt::LeftButton)
  {
//    m_TrackBall.move(pixelPosToViewPos(e->localPos()), m_QuatRot);
    m_TrackBall.move(pixelPosToViewPos(e->localPos()),m_TrackBall.rotation());
    e->accept();
  }
  else
  {
    m_TrackBall.move(pixelPosToViewPos(e->localPos()),m_TrackBall.rotation());
//    m_TrackBall.release(pixelPosToViewPos(e->localPos()),m_QuatRot);
  }
}


QPointF DepthWidget::pixelPosToViewPos(const QPointF& p)
{
    return QPointF(2.0 * float(p.x()) / width() - 1.0,
                   1.0 - 2.0 * float(p.y()) / height());
}

void DepthWidget::timerEvent(QTimerEvent *)
{
  // Decrease angular speed (friction)
  m_AngularSpeed *= 0.99;
      update();

  // Stop rotation when speed goes below threshold
//  if (m_AngularSpeed < 0.01) {
//      m_AngularSpeed = 0.0;
//  } else {
//      // Update rotation
//      m_QuatRot = QQuaternion::fromAxisAndAngle(m_RotAxis, m_AngularSpeed) * m_QuatRot;

//      // Request an update
//  }
}

void DepthWidget::initializeGL()
{

  initializeOpenGLFunctions();
  m_MeshEng = new MeshEngine();
  m_MeshEng->InitMeshBuffers();
  initShaders();
  initTextures();


  // Enable depth buffer
  glEnable(GL_DEPTH_TEST);

  // Enable back face culling
//  glEnable(GL_CULL_FACE);


  m_Timer.start(15, this);
  // Compile vertex shader
}

void DepthWidget::resizeGL(int w, int h)
{
  // Calculate aspect ratio
  qreal aspect = qreal(w) / qreal(h ? h : 1);

  // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
  const qreal zNear = 1.0, zFar = 2000.0, fov = 45.0;

  // Reset projection
  m_ProjMatrix.setToIdentity();

  // Set perspective projection
  m_ProjMatrix.perspective(fov, aspect, zNear, zFar);
  m_ProjMatrix.lookAt(QVector3D(0,200,500),QVector3D(0,0,0),QVector3D(0,1,0));
}

void DepthWidget::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //paint jaw
//  m_TexMap["Jaw"]->
  m_ModelViewMatrix.setToIdentity();
//  m_ModelViewMatrix.rotate(m_QuatRot);
  m_ModelViewMatrix.rotate(m_TrackBall.rotation());
  m_Shader.setUniformValue("u_MvpMatrix",m_ProjMatrix*m_ModelViewMatrix);
//  qDebug()<<"Start Render";
  m_MeshEng->RenderObjects(&m_Shader,m_TexMap);

//! [6]
  // Calculate model view transformation
//  QMatrix4x4 matrix;
//  matrix.translate(0.0, 0.0, -5.0);
//  matrix.rotate(rotation);

  // Set modelview-projection matrix
//  program.setUniformValue("mvp_matrix", projection * matrix);
//! [6]

  // Use texture unit 0 which contains cube.png
//  program.setUniformValue("texture", 0);

  // Draw cube geometry
//  geometries->drawCubeGeometry(&program);
}

void DepthWidget::initShaders()
{
  if (!m_Shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertex.glsl"))
    close();

  // Compile fragment shader
  if (!m_Shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment.glsl"))
    close();

  // Link shader pipeline
  if (!m_Shader.link())
    close();

  // Bind shader pipeline for use
  if (!m_Shader.bind())
    close();

}

void DepthWidget::initTextures()
{
  m_TexMap["Head"] = new QOpenGLTexture(QImage(":/Head.png").mirrored());
  m_TexMap["Jaw"] = new QOpenGLTexture(QImage(":/Jaw.png").mirrored());
  m_TexMap["Key"] = new QOpenGLTexture(QImage(":/Key.png").mirrored());
  m_TexMap["Leg"] = new QOpenGLTexture(QImage(":/leg.png").mirrored());

  foreach(const QString& key, m_TexMap.keys())
  {
    m_TexMap[key]->setMinificationFilter(QOpenGLTexture::Nearest);
    m_TexMap[key]->setMagnificationFilter(QOpenGLTexture::Linear);
    m_TexMap[key]->setWrapMode(QOpenGLTexture::Repeat);
  }

}

