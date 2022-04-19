#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "introductionvideowindow.h"
#include "coursemodewindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void closeIntroWindowSlot();
    void closeCourseModeWindowSlot();

private slots:
    void on_openIntroVideoButton_clicked();

    void on_courseModeButton_clicked();

    void on_sandboxModeButton_clicked();

private:
    Ui::MainWindow *ui;
    IntroductionVideoWindow introVideoWindow;
    CourseModeWindow simulatorWindow;
};
#endif // MAINWINDOW_H
