#include "simulationviewwidget.h"
#include <QPaintEvent>

SimulationViewWidget::SimulationViewWidget(QWidget *parent)
    : QLabel{parent},
      sim(simulator(5,30,50,6,0,10,5.5,20,20))
{
    worldUpdateTimer = new QTimer(this);
    connect(worldUpdateTimer, &QTimer::timeout, this, &SimulationViewWidget::updateWorld);
}

/**
 * @brief Public method to be called for updateing the simulator world state.
 */
void SimulationViewWidget::updateWorld(){
    sim.updateWorld();
    emit worldUpdated((int)sim.getBelayerForce(),(int)sim.getClimberForce());
    drawWorld();
}

/**
 * @brief Gets the belayer's position from the simulator.
 * @return  B2Vec2 containing the belayer's x and y positions in meters.
 */
b2Vec2 SimulationViewWidget::getBelayerPosition(){
    return sim.getBelayerPos();
}

/**
 * @brief Gets the leader's position from the simulator.
 * @return  B2Vec2 containing the leader's x and y positions in meters.
 */
b2Vec2 SimulationViewWidget::getLeaderPosition(){
    return sim.getClimberPos();
}

/**
 * @brief Starts running the simulation for the set duration.
 */
void SimulationViewWidget::startSimulationTimerSlot(){
    worldUpdateTimer->start(updateDelay);
    QTimer::singleShot(simulationDuration, this, &SimulationViewWidget::stopSimulationTimerSlot);
}

/**
 * @brief Stops the simulation update timer, ending the simulation.
 */
void SimulationViewWidget::stopSimulationTimerSlot(){
    worldUpdateTimer->stop();
    emit simulationCompleteSignal();
}

/**
 * @brief Updates the belayer object's height in the simulation.
 * @param height
 */
void SimulationViewWidget::belayerHeightUpdatedSlot(float height){
    sim.setBelayerHeight(height);
}

/**
 * @brief Updates the climber body's height in the simulation.
 * @param height
 */
void SimulationViewWidget::climberHeightUpdatedSlot(float height){
    sim.setClimberHeight(height);
}

/**
 * @brief SimulationViewWidget::belayerWeightUpdatedSlot
 * @param weight
 */
void SimulationViewWidget::belayerWeightUpdatedSlot(float weight){
    sim.setBelayerMass(weight);
}

/**
 * @brief SimulationViewWidget::climberWeightUpdatedSlot
 * @param weight
 */
void SimulationViewWidget::climberWeightUpdatedSlot(float weight){
    sim.setClimberMass(weight);
}

/**
 * @brief Handles all drawing on the simulation widget.
 */
void SimulationViewWidget::drawWorld(){
    QPixmap map(this->width(), this->height());
    map.fill(QColor(255,255,255));
    QPainter p(&map);
    b2Vec2 climberPosition(sim.getClimberPos());
    climberPosition.x = climberPosition.x*10;
    climberPosition.y = climberPosition.y*10;
    p.drawRect(climberPosition.x, groundLevel-climberPosition.y, 45, 45);
    b2Vec2 belayerPosition(sim.getBelayerPos());
    belayerPosition.x = belayerPosition.x*10;
    belayerPosition.y = belayerPosition.y*10;
    p.drawRect(belayerPosition.x, groundLevel-belayerPosition.y, 45, 45);
    setPixmap(map);
}
