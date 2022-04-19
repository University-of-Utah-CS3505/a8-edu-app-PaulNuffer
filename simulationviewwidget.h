#ifndef SIMULATIONVIEWWIDGET_H
#define SIMULATIONVIEWWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QStyleOption>
#include <QPainter>
#include <QTimer>
#include "simulator.h"

class SimulationViewWidget : public QLabel
{
    Q_OBJECT
public:
    explicit SimulationViewWidget(QWidget *parent = nullptr);
    b2Vec2 getLeaderPosition();
    b2Vec2 getBelayerPosition();
    void updateWorld();

public slots:
    void startSimulationTimerSlot();
    void climberHeightUpdatedSlot(float);
    void climberWeightUpdatedSlot(float);
    void belayerHeightUpdatedSlot(float);
    void belayerWeightUpdatedSlot(float);

signals:
    void worldUpdated(int belayerNewtons, int climberNewtons);
    void simulationCompleteSignal();

private slots:
    void stopSimulationTimerSlot();

private:
    enum WindowMode{course, sandbox};
    WindowMode currentWindowMode;
    simulator sim;
    QTimer* worldUpdateTimer;
    int updateDelay = 17;
    int simulationDuration = 5000;
    int groundLevel = 600;

    void drawWorld();
};

#endif // SIMULATIONVIEWWIDGET_H
