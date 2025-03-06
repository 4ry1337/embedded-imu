#include "mythread.h"
#include <QAbstractSocket>
#include <QDebug>
#include <QStringList>
#include <QTcpSocket>
#include <QThread>

MyThread::MyThread(QObject *parent) : QThread(parent) {}

MyThread::~MyThread() {}

void MyThread::run() {
  QTcpSocket socket;
  QString server_ip = "192.168.7.2";
  int port = 54321;

  socket.connectToHost(server_ip, port);

  if (!socket.waitForConnected(3000)) {
    qDebug() << "Failed to connect: " << socket.errorString();
    return;
  }
  qDebug() << "Connected to server.";

  while (run_flag) {
    if (!socket.waitForReadyRead(100)) continue;

    QByteArray data = socket.readAll();
    QString data_str = QString::fromUtf8(data).trimmed();
    if(data_str.isEmpty()) continue;

    QStringList components = data_str.split(",");

    if(components.size() != 4) {
        qDebug() << "Incomplete data received: " << data_str;
        continue;
    }

    float q0 = components[0].toFloat();
    float q1 = components[1].toFloat();
    float q2 = components[2].toFloat();
    float q3 = components[3].toFloat();

    QQuaternion temp_q = QQuaternion(q0, q1, q2, q3);
    temp_q.normalize();
    emit setQuats(temp_q.scalar(), temp_q.x(), temp_q.y(), temp_q.z());
  }

  socket.disconnectFromHost();
  if (socket.state() != QAbstractSocket::UnconnectedState) {
    socket.waitForDisconnected(3000);
  }
}
