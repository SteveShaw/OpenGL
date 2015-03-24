#include "meshengine.h"
#include <QTextStream>
#include <QDebug>
#include <QFile>

MeshEngine::MeshEngine()
  :m_IndexBuffer(QOpenGLBuffer::IndexBuffer)
{
 initializeOpenGLFunctions();
 m_VertexBuffer.create();
// m_NormalBuffer.create();
// m_ColorBuffer.create();
 m_IndexBuffer.create();
 m_TexBuffer.create();
}

MeshEngine::~MeshEngine()
{
  foreach(const QString& key, m_IndexBufMap.keys())
  {
    m_IndexBufMap[key]->destroy();
    delete m_IndexBufMap[key];
    m_IndexBufMap[key] = NULL;
  }
  m_IndexBufMap.clear();;

  m_TexBuffer.destroy();
  m_IndexBuffer.destroy();
  m_VertexBuffer.destroy();;
}

void MeshEngine::InitMeshBuffers()
{
  QFile data(":/MeshData.txt");
  if(data.open(QFile::ReadOnly)) {
    QTextStream txtStream(&data);
    while(true)
    {
      QString txt = txtStream.readLine();
      if(txt.isNull()) break;
      QStringList dataList = txt.split(",");
      if(dataList.size()>=3)
      {
//        m_Vertices.append(QVector3D(dataList[0].toFloat(),dataList[1].toFloat(),dataList[2].toFloat()));
        m_Vertices.append(dataList[0].toFloat());
        m_Vertices.append(dataList[1].toFloat());
        m_Vertices.append(dataList[2].toFloat());
      }
    }
  }

  data.close();
  data.setFileName(":/MeshTextCoord.txt");
  if(data.open(QFile::ReadOnly)) {
    QTextStream txtStream(&data);
    while(true)
    {
      QString txt = txtStream.readLine();
      if(txt.isNull()) break;
      QStringList dataList = txt.split(",");
      if(dataList.size()>=2)
      {
//        m_TexCoords.append(QVector2D(dataList[0].toFloat(),dataList[1].toFloat()));
        m_TexCoords.append(dataList[0].toFloat());
        m_TexCoords.append(dataList[1].toFloat());
      }
    }
  }

  ParseFaceFile(":/MeshFace.txt");

  qDebug()<<m_Vertices.length();
  qDebug()<<m_TexCoords.length();
//  int total_length = 0;

  m_VertexBuffer.bind();
  m_VertexBuffer.allocate(m_Vertices.data(),m_Vertices.size()*sizeof(GLfloat));
  m_TexBuffer.bind();
  m_TexBuffer.allocate(m_TexCoords.data(),m_TexCoords.size()*sizeof(GLfloat));

  foreach(const QString &key, m_Faces.keys())
  {
//    total_length += m_Faces[key].length();
    m_IndexBufMap[key] = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_IndexBufMap[key]->create();
    m_IndexBufMap[key]->bind();
    m_IndexBufMap[key]->allocate(m_Faces[key].data(),m_Faces[key].size()*sizeof(GLushort));
//    qDebug()<<"Key:"<<m_Faces[key].size();
  }

}

void MeshEngine::RenderObjects(QOpenGLShaderProgram *prog, const QMap<QString, QOpenGLTexture*>&tm)
{
 m_VertexBuffer.bind();
 int a_Position = prog->attributeLocation("a_Position");
 prog->enableAttributeArray(a_Position);
 prog->setAttributeBuffer(a_Position,GL_FLOAT,0,3);

 m_TexBuffer.bind();
 int a_TexCoord = prog->attributeLocation("a_TexCoord");
 prog->enableAttributeArray(a_TexCoord);
 prog->setAttributeBuffer(a_TexCoord,GL_FLOAT,0,2);

// m_IndexBufMap["Jaw"]->bind();
// qDebug()<<m_IndexBufMap["Jaw"]->size();
// qDebug()<<m_Faces["Jaw"].size();
// m_IndexBuffer.bind();
// glDrawElements(GL_TRIANGLE_STRIP,m_Faces["Jaw"].size(),GL_UNSIGNED_SHORT,m_Faces["Jaw"].data());
 tm["Jaw"]->bind(0);
  prog->setUniformValue("u_Sampler",0);
 m_IndexBufMap["Jaw"]->bind();
 glDrawElements(GL_POINTS,m_Faces["Jaw"].size(),GL_UNSIGNED_SHORT,0);

 tm["Leg"]->bind(1);
  prog->setUniformValue("u_Sampler",1);
 m_IndexBufMap["RightLeg"]->bind();
 glDrawElements(GL_TRIANGLES,m_Faces["RightLeg"].size(),GL_UNSIGNED_SHORT,0);
 m_IndexBufMap["LeftLeg"]->bind();
 glDrawElements(GL_TRIANGLES,m_Faces["LeftLeg"].size(),GL_UNSIGNED_SHORT,0);

 tm["Head"]->bind(2);
  prog->setUniformValue("u_Sampler",2);
 m_IndexBufMap["Head"]->bind();
 glDrawElements(GL_TRIANGLES,m_Faces["Head"].size(),GL_UNSIGNED_SHORT,0);

 tm["Key"]->bind(3);
  prog->setUniformValue("u_Sampler",3);
 m_IndexBufMap["SpinKey"]->bind();
 glDrawElements(GL_TRIANGLES,m_IndexBufMap["SpinKey"]->size(),GL_UNSIGNED_SHORT,0);

}

void MeshEngine::ParseFaceFile(const QString &f)
{
  quint8 cur_stat = 0;

  QFile data(f);
  if(data.open(QFile::ReadOnly))
  {
    QString key;
    QTextStream txtStrm(&data);
    while(true)
    {
      QString& txt = txtStrm.readLine();
      if(txt.isNull()) break;
      cur_stat %=3;
      switch(cur_stat)
      {
      case 0:
        key = txt;
        ++cur_stat;
        break;
      case 1:
        if(txt=="Begin")
        {
          ++cur_stat;
        }
        break;
      case 2:
        if(txt=="End")
        {
          ++cur_stat;
        }
        else
        {
          QStringList l = txt.split(",");
          if(l.size()>=3)
          {
//            val = l[0].toUShort(&ok);
//            if(!ok) qDebug()<<"Invalid value";
//            val = l[1].toUShort(&ok);
//            if(!ok) qDebug()<<"Invalid value";
//            val = l[2].toUShort(&ok);
//            if(!ok) qDebug()<<"Invalid value";
            m_Faces[key].append(l[0].toUShort());
            m_Faces[key].append(l[1].toUShort());
            m_Faces[key].append(l[2].toUShort());
          }
        }
        break;
      }
   }
  }
}

