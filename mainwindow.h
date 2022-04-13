#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Box2D/Box2D.h"
#include "qlabel.h"
#include <vector>
#include <QPainter>

using std::vector;
using std::map;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    b2Body* climber;
    b2Body* belayer;

    void createPulley(int x, int y, int length, b2Body* A, b2Body* B);
    vector<b2Body*> createRope(int length);
    vector<b2Body*> ropeBodies;

    map<b2Body*, QLabel*> ropeBodyToLabel;
signals:
    void newPos(int);
public slots:
    void updateWorld();
   // void setClimberY(int);

private:
    b2World world;
    Ui::MainWindow *ui;

    int convertBox2dX(float input);
    int convertBox2dY(float input);

    float ropeWidth;
    float ropeHeight;
};
#endif // MAINWINDOW_H
