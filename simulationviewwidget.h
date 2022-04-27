#ifndef SIMULATIONVIEWWIDGET_H
#define SIMULATIONVIEWWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QStyleOption>
#include <QPainter>
#include <QTimer>
#include "simulator.h"
#include <map>
#include <QPixmap>

using std::map;

class SimulationViewWidget : public QLabel
{
    Q_OBJECT
public:
    explicit SimulationViewWidget(QWidget *parent = nullptr);
    b2Vec2 getLeaderPosition();
    b2Vec2 getBelayerPosition();
    int maxClimberNewtons=0;
    int maxBelayerNewtons=0;
    void updateWorld();

    float climberHeight = 35;
    float climberWeight = 72;
    float belayerHeight = 0.1;
    float belayerWeight = 70;
    float boltHeight = 30;
    float ropeLength = 35;

public slots:
    void simulationStartSlot();
    void simulationStopSlot();
    void climberHeightUpdatedSlot(float);
    void climberWeightUpdatedSlot(float);
    void belayerHeightUpdatedSlot(float);
    void belayerWeightUpdatedSlot(float);
    void pulleyHeightUpdatedSlot(float);
    void pulleyRopeLengthUpdatedSlot(float);
    void resetSimulationDataSlot();

signals:
    void worldUpdated(int currentClimberNewtons, int maxClimberNewtons, int currentBelayerNewtons, int maxBelayerNewtons);

private slots:

private:
    enum WindowMode{course, sandbox};
    float defaultX;

    const int UPDATE_DELAY = 17;
    const int GROUND_LEVEL = 600;
    const int COORDINATE_SCALING = 10;
    const int X_OFFSET = 150;

    WindowMode currentWindowMode;
    simulator sim;
    QTimer* worldUpdateTimer;

    float ropeWidth;
    float ropeHeight;

    map<b2Body*, QLabel*> climberRopeBodyToLabel;
    map<b2Body*, QLabel*> belayerRopeBodyToLabel;

    void drawRope(vector<b2Body*>& rope,  map<b2Body*, QLabel*>& map);


    void drawWorld();

    bool simRunning = false;
};

#endif // SIMULATIONVIEWWIDGET_H
