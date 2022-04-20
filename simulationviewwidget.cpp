#include "simulationviewwidget.h"
#include <QPaintEvent>

SimulationViewWidget::SimulationViewWidget(QWidget *parent)
    : QLabel{parent}, defaultX(5),
      sim(simulator(defaultX,35,72,defaultX+1,0,70,5.5,30,35))
{
    worldUpdateTimer = new QTimer(this);
    connect(worldUpdateTimer, &QTimer::timeout, this, &SimulationViewWidget::updateWorld);
}

/**
 * @brief Public method to be called for updateing the simulator world state.
 */
void SimulationViewWidget::updateWorld(){
    sim.updateWorld();
    int currentBelayerNewtons = (int)sim.getBelayerForce();
    int currentClimberNewtons = (int)sim.getClimberForce();
    if(currentClimberNewtons>maxClimberNewtons){
        maxClimberNewtons=currentClimberNewtons;
    }
    if(currentBelayerNewtons>maxBelayerNewtons){
        maxBelayerNewtons=currentClimberNewtons;
    }
    emit worldUpdated(currentClimberNewtons, maxClimberNewtons, currentBelayerNewtons, maxBelayerNewtons);
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
void SimulationViewWidget::simulationStartSlot(){
    worldUpdateTimer->start(updateDelay);
}

/**
 * @brief Stops the simulation update timer, ending the simulation.
 */
void SimulationViewWidget::simulationStopSlot(){
    worldUpdateTimer->stop();
}

/**
 * @brief Updates the belayer object's height in the simulation.
 * @param height - the new height of the belayer
 */
void SimulationViewWidget::belayerHeightUpdatedSlot(float height){
    sim.setBelayerHeight(height);
    drawWorld();
}

/**
 * @brief Updates the climber body's height in the simulation.
 * @param height - the new height of the climber
 */
void SimulationViewWidget::climberHeightUpdatedSlot(float height){
    sim.setClimberHeight(height);
    drawWorld();
}

/**
 * @brief Updates the belayer's weight in the simulation object
 * @param weight - the new weight of the belayer
 */
void SimulationViewWidget::belayerWeightUpdatedSlot(float weight){
    sim.setBelayerMass(weight);
    drawWorld();
}

/**
 * @brief Updates the climber's weight in the simulation object
 * @param weight - the new weight of the climber
 */
void SimulationViewWidget::climberWeightUpdatedSlot(float weight){
    sim.setClimberMass(weight);
}

/**
 * @brief Updates the pulley height to be the new value supplied
 * @param height - the new height of the pulley
 */
void SimulationViewWidget::pulleyHeightUpdatedSlot(float height){
    sim.setPulleyHeight(height);
    drawWorld();
}

/**
 * @brief Updates the rope length of the pulley object in the simulation obejct
 * @param length - the new length of the rope
 */
void SimulationViewWidget::pulleyRopeLengthUpdatedSlot(float length){
    sim.setPulleyRopeLength(length);
    drawWorld();
}

/**
 * @brief Helper method for resetting the simulation to its inital values. To be called
 * whenever the simulation window is hidden.
 */
void SimulationViewWidget::resetSimulationDataSlot(){
    worldUpdateTimer->stop();
    simRunning = false;
    climberHeightUpdatedSlot(35);
    climberWeightUpdatedSlot(72);
    sim.setClimberX(defaultX);
    belayerHeightUpdatedSlot(0);
    belayerWeightUpdatedSlot(70);
    sim.setBelayerX(defaultX+1);
    pulleyHeightUpdatedSlot(30);
    pulleyRopeLengthUpdatedSlot(35);
    drawWorld();
}

/**
 * @brief Handles all drawing on the simulation widget.
 */
void SimulationViewWidget::drawWorld(){
    QPixmap map(this->width(), this->height());
    map.fill(QColor(255,255,255));
    QPainter p(&map);
    b2Vec2 climberPosition(sim.getClimberPos());
    climberPosition.x = climberPosition.x*xScaling;
    climberPosition.y = climberPosition.y*10;
    p.drawImage(QPoint(climberPosition.x, groundLevel-climberPosition.y), QImage(":/Resources/climber.png"));
    b2Vec2 belayerPosition(sim.getBelayerPos());
    belayerPosition.x = belayerPosition.x*xScaling;
    belayerPosition.y = belayerPosition.y*10;
    p.drawImage(QPoint(belayerPosition.x, groundLevel-belayerPosition.y), QImage(":/Resources/belayer.png"));
    b2Vec2 anchorPosition(sim.getPulleyPos());
    p.setPen(QPen(QColor(0,0,255), 3));
    anchorPosition.x = anchorPosition.x*xScaling;
    anchorPosition.y = anchorPosition.y*10;
    p.drawEllipse(anchorPosition.x, groundLevel-anchorPosition.y, 25 , 25);
    setPixmap(map);
}
