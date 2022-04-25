#include "simulationviewwidget.h"
#include <QPaintEvent>


SimulationViewWidget::SimulationViewWidget(QWidget *parent)
    : QLabel{parent}, defaultX(5),
      sim(simulator(defaultX,35,72,defaultX+1,0,70,5.5,30,35))
{
    worldUpdateTimer = new QTimer(this);
    connect(worldUpdateTimer, &QTimer::timeout, this, &SimulationViewWidget::updateWorld);

    ropeWidth = 0.5, ropeHeight = 0.5;
    //ropeWidth = sim.ROPE_WIDTH, ropeHeight = sim.ROPE_HEIGHT;

    QLabel* ropeLabel;

    for (b2Body* ropeBody : sim.climberRope) {
        // Add a new label representing the rope segments body
        ropeLabel =  new QLabel(this);
        ropeLabel->setGeometry((ropeBody->GetPosition().x)*coordinateScaling, groundLevel - (ropeBody->GetPosition().y)*coordinateScaling, (ropeWidth)*coordinateScaling, (ropeHeight)*coordinateScaling);

        // Add the body and label to the map
        climberRopeBodyToLabel.emplace(ropeBody, ropeLabel);
    }

    for (b2Body* ropeBody : sim.belayerRope) {
        // Add a new label representing the rope segments body
        ropeLabel =  new QLabel(this);
        ropeLabel->setGeometry((ropeBody->GetPosition().x)*coordinateScaling, groundLevel - (ropeBody->GetPosition().y)*coordinateScaling, (ropeWidth)*coordinateScaling, (ropeHeight)*coordinateScaling);

        // Add the body and label to the map
        belayerRopeBodyToLabel.emplace(ropeBody, ropeLabel);
    }
}

/**
 * @brief Public method to be called for updateing the simulator world state.
 */
void SimulationViewWidget::updateWorld(){
    sim.updateWorld(&climberRopeBodyToLabel, &belayerRopeBodyToLabel, this);
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
    resetSimulationDataSlot();
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
    climberHeightUpdatedSlot(sim.pulleyRopeLength);
    climberWeightUpdatedSlot(72);
    sim.setClimberX(defaultX);
    belayerHeightUpdatedSlot(0);
    belayerWeightUpdatedSlot(70);
    sim.setBelayerX(defaultX+1);
    //pulleyHeightUpdatedSlot(30);
    //pulleyRopeLengthUpdatedSlot(35);

//    for (auto map : climberRopeBodyToLabel) {
//        map.second->~QLabel();
//    }

//    for (auto map : belayerRopeBodyToLabel) {
//        map.second->~QLabel();
//    }

//    climberRopeBodyToLabel.clear();
//    belayerRopeBodyToLabel.clear();
//    QLabel* ropeLabel;
//    for (b2Body* ropeBody : sim.climberRope) {
//        // Add a new label representing the rope segments body
//        ropeLabel =  new QLabel(this);
//        ropeLabel->setGeometry((ropeBody->GetPosition().x)*coordinateScaling, groundLevel - (ropeBody->GetPosition().y)*coordinateScaling, (ropeWidth)*coordinateScaling, (ropeHeight)*coordinateScaling);

//        // Add the body and label to the map
//        climberRopeBodyToLabel.emplace(ropeBody, ropeLabel);
//    }

//    for (b2Body* ropeBody : sim.belayerRope) {
//        // Add a new label representing the rope segments body
//        ropeLabel =  new QLabel(this);
//        ropeLabel->setGeometry((ropeBody->GetPosition().x)*coordinateScaling, groundLevel - (ropeBody->GetPosition().y)*coordinateScaling, (ropeWidth)*coordinateScaling, (ropeHeight)*coordinateScaling);

//        // Add the body and label to the map
//        belayerRopeBodyToLabel.emplace(ropeBody, ropeLabel);
//    }

    if (sim.hasPulley) {
        sim.world.DestroyJoint(sim.pulleyJoint);
    }
    sim.hasPulley = false;

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
    climberPosition.x = climberPosition.x*coordinateScaling;
    climberPosition.y = climberPosition.y*coordinateScaling;
    p.drawImage(QPoint(climberPosition.x+xOffset, groundLevel-climberPosition.y), QImage(":/Resources/climber.png"));
    b2Vec2 belayerPosition(sim.getBelayerPos());
    belayerPosition.x = belayerPosition.x*coordinateScaling;
    belayerPosition.y = belayerPosition.y*coordinateScaling;
    p.drawImage(QPoint(belayerPosition.x+xOffset, groundLevel-belayerPosition.y), QImage(":/Resources/belayer.png"));
    //b2Vec2 anchorPosition(sim.getPulleyPos());
    b2Vec2 anchorPosition = sim.bolt->GetPosition();
    p.setPen(QPen(QColor(0,0,255), 3));
    anchorPosition.x = anchorPosition.x*coordinateScaling;
    anchorPosition.y = anchorPosition.y*coordinateScaling;
    p.drawEllipse(anchorPosition.x+xOffset, groundLevel-anchorPosition.y, 25 , 25);
    setPixmap(map);

    drawRope(sim.climberRope, climberRopeBodyToLabel);
    drawRope(sim.belayerRope, belayerRopeBodyToLabel);

}

void SimulationViewWidget::drawRope(vector<b2Body*>& rope,  map<b2Body*, QLabel*>& map) {

    for (b2Body* ropeBody : rope) {
        //if(map.count(ropeBody) != 0) {
            // Add a new label representing the rope segments body
            QLabel* ropeLabel =  map.at(ropeBody);
            ropeLabel->setGeometry((ropeBody->GetPosition().x*coordinateScaling)+xOffset, groundLevel - (ropeBody->GetPosition().y)*coordinateScaling, (ropeWidth)*coordinateScaling, (ropeHeight)*coordinateScaling);

            // Create a new image for the label
            QImage imgFill = QImage((ropeWidth)*10, groundLevel - (ropeHeight)*10, QImage::Format_ARGB32);
            imgFill.fill(QColor(200,0,175,255));
            ropeLabel->setPixmap(QPixmap::fromImage(imgFill));
        //}
    }
}
