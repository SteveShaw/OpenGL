#ifndef MESHENGINE_H
#define MESHENGINE_H

#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <QVector2D>
#include <QMap>
#include <QOpenGLTexture>
#include <QVector>
#include <QList>
#include <QMap>

class MeshEngine : protected QOpenGLFunctions
{
public:
  MeshEngine();
  ~MeshEngine();

  void InitMeshBuffers();
//  void RenderObjects(QOpenGLShaderProgram* prog){}
  void RenderObjects(QOpenGLShaderProgram *prog, const QMap<QString, QOpenGLTexture*>&tm);

private:
  QVector<GLfloat> m_Vertices;
  QVector<GLfloat> m_TexCoords;
  QMap<QString, QVector<GLushort>> m_Faces;
  QOpenGLBuffer m_VertexBuffer;
  //  QOpenGLBuffer m_NormalBuffer;
  //  QOpenGLBuffer m_ColorBuffer;
  QOpenGLBuffer m_IndexBuffer;
  QOpenGLBuffer m_TexBuffer;
  QMap<QString, QOpenGLBuffer*> m_IndexBufMap;

protected:
  void ParseFaceFile(const QString& f);
};

#endif // MESHENGINE_H
