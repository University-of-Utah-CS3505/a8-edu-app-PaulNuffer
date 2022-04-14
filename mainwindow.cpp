#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&introVideoWindow,
            &IntroductionVideoWindow::closeIntroWindowSignal,
            this,
            &MainWindow::closeIntroWindowSlot);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeIntroWindowSlot(){
    show();
}

void MainWindow::on_openIntroVideoButton_clicked()
{
    this->hide();
    introVideoWindow.openIntroVideoWindow();
}

