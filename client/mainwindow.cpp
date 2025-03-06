#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  connect(&mThread, SIGNAL(setQuats(float, float, float, float)), &mGL,
          SLOT(getQuaternions(float, float, float, float)));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_pushButton_clicked() {
  mThread.run_flag = true;
  mThread.start();

  if (!mGL.isVisible()) {
    mGL.resize(640, 480);
    mGL.show();
  }
}

void MainWindow::on_pushButton_2_clicked() {
  mThread.run_flag = false;
  mThread.wait();
}
