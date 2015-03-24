#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  m_KinectWnd = new DepthWidget(this);
  ui->verticalLayout->addWidget(m_KinectWnd);
//  m_KinectWnd->initializeGL();
}

MainWindow::~MainWindow()
{
  delete ui;
}
