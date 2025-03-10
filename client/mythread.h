#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QQuaternion>
#include <QThread>
#include <QVector3D>
#include <QtMath>

class MyThread : public QThread {
  Q_OBJECT
public:
  MyThread(QObject *parent = 0);
  ~MyThread();
  bool run_flag;

signals:
  void setQuats(const float, const float, const float, const float);

private:
  void run();
  float q0, q1, q2, q3;
};

#endif // MYTHREAD_H
