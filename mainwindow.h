#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Box2D/Box2D.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void newPos(int);
public slots:
    void updateWorld();
    void setClimberY(int);

private:
    b2World world;
    b2Body* body;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
