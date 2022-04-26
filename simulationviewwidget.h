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
    int updateDelay = 17;
    int simulationDuration = 5000;
    int groundLevel = 600;
    int coordinateScaling = 10;
    int xOffset = 150;
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
