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
    connect(&simulatorWindow,
            &CourseModeWindow::closeCourseModeWindowSignal,
            this,
            &MainWindow::closeCourseModeWindowSlot);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeIntroWindowSlot(){
    show();
}

void MainWindow::closeCourseModeWindowSlot(){
    show();
}

void MainWindow::on_openIntroVideoButton_clicked()
{
    this->hide();
    introVideoWindow.openIntroVideoWindow();
}

void MainWindow::on_courseModeButton_clicked()
{
    this->hide();
    simulatorWindow.setupWindowAsCourseMode();
}

