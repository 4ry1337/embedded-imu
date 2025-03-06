#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QTimer>

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT
public:
  MyGLWidget(QWidget *parent = nullptr);
  ~MyGLWidget();
  bool Reversed;

protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int width, int height);

public slots:
  void getQuaternions(const float, const float, const float, const float);
  void updateAll();

private:
  float q0, q1, q2, q3;
  QQuaternion rotation;
  QTimer *timer;
  float _q0_b, _q1_b, _q2_b, _q3_b;
};

#endif // MYGLWIDGET_H
