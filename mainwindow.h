#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Box2D/Box2D.h"
#include "qlabel.h"
#include <vector>
#include <QPainter>
#include <cmath>

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
    vector<b2Body*> climberRope;
    vector<b2Body*> belayerRope;

    map<b2Body*, QLabel*> climberRopeBodyToLabel;
    map<b2Body*, QLabel*> belayerRopeBodyToLabel;

    void createPulley(int x, int y, int length, b2Body* A, b2Body* B);
    vector<b2Body*> connectRopeTo(float segmentDensity, b2Body* A, b2Body* B);
    vector<b2Body*> createRope(int numSegments, b2Vec2 vecA, b2Vec2 vecB);
    void drawRope(vector<b2Body*> rope,  map<b2Body*, QLabel*> map);
    void updateRopes(b2Body* bolt)

signals:
    void newPos(int);
public slots:
    void updateWorld();
   // void setClimberY(int);

private:
    b2World world;
    Ui::MainWindow *ui;

    float ropeWidth;
    float ropeHeight;
    int convertBox2dX(float input);
    int convertBox2dY(float input);
};
#endif // MAINWINDOW_H
