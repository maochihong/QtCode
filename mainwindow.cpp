#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <myudp.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), ui->progressBar, SLOT(setValue(int)));

    //MyUDP server;
    //server.Init();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_enableUDP_clicked()
{

}
