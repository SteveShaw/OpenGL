#ifndef DEPTHWIDGET_H
#define DEPTHWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMap>
#include <QMatrix4x4>
#include <QBasicTimer>
#include "trackball.h"
#include "meshengine.h"

class DepthWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
  Q_OBJECT
public:
  explicit DepthWidget(QWidget* parent = 0);
  ~DepthWidget();

protected:
  void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
  void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
  void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
  void timerEvent(QTimerEvent *e) Q_DECL_OVERRIDE;

  void initializeGL() Q_DECL_OVERRIDE;
  void resizeGL(int w, int h) Q_DECL_OVERRIDE;
  void paintGL() Q_DECL_OVERRIDE;

  void initShaders();
  void initTextures();
  QPointF pixelPosToViewPos(const QPointF& p);

private:
  QBasicTimer m_Timer;
  QMap<QString, QOpenGLTexture*> m_TexMap;
  QOpenGLShaderProgram m_Shader;
  MeshEngine* m_MeshEng;
  QOpenGLTexture *m_Texture;
  QMatrix4x4 m_ProjMatrix;
  QMatrix4x4 m_ModelViewMatrix;
  TrackBall m_TrackBall;
  QVector3D m_RotAxis;
  qreal m_AngularSpeed;
  QVector2D m_MousePressPos;
  QQuaternion m_QuatRot;
};

#endif // DEPTHWIDGET_H
