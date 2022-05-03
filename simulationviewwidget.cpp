#include "simulationviewwidget.h"


SimulationViewWidget::SimulationViewWidget(QWidget *parent)
    : QLabel{parent}, defaultX(5),
      sim(simulator(defaultX,climberHeight,climberWeight,defaultX+1,belayerHeight,belayerWeight,defaultX+.5,boltHeight,ropeLength))
{
    worldUpdateTimer = new QTimer(this);
    connect(worldUpdateTimer, &QTimer::timeout, this, &SimulationViewWidget::updateWorld);

    ropeWidth = 0.25, ropeHeight = 0.5;
    //ropeWidth = sim.ROPE_WIDTH, ropeHeight = sim.ROPE_HEIGHT;

    QLabel* ropeLabel;

    for (b2Body* ropeBody : sim.climberRope) {
        // Add a new label representing the rope segments body
        ropeLabel =  new QLabel(this);
        ropeLabel->setGeometry((ropeBody->GetPosition().x)*COORDINATE_SCALING, GROUND_LEVEL - (ropeBody->GetPosition().y)*COORDINATE_SCALING, (ropeWidth)*COORDINATE_SCALING, (ropeHeight)*COORDINATE_SCALING);

        // Add the body and label to the map
        climberRopeBodyToLabel.emplace(ropeBody, ropeLabel);
    }

    for (b2Body* ropeBody : sim.belayerRope) {
        // Add a new label representing the rope segments body
        ropeLabel =  new QLabel(this);
        ropeLabel->setGeometry((ropeBody->GetPosition().x)*COORDINATE_SCALING, GROUND_LEVEL - (ropeBody->GetPosition().y)*COORDINATE_SCALING, (ropeWidth)*COORDINATE_SCALING, (ropeHeight)*COORDINATE_SCALING);

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
    resetSimulationDataSlot();
    worldUpdateTimer->start(UPDATE_DELAY);
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
    belayerHeight = height;
    sim.setBelayerHeight(belayerHeight);
    drawWorld();
}

/**
 * @brief Updates the climber body's height in the simulation.
 * @param height - the new height of the climber
 */
void SimulationViewWidget::climberHeightUpdatedSlot(float height){
    climberHeight = height;
    sim.setClimberHeight(climberHeight);
    drawWorld();
}

/**
 * @brief Updates the belayer's weight in the simulation object
 * @param weight - the new weight of the belayer
 */
void SimulationViewWidget::belayerWeightUpdatedSlot(float weight){
    belayerWeight = weight;
    sim.setBelayerMass(belayerWeight);
}

/**
 * @brief Updates the climber's weight in the simulation object
 * @param weight - the new weight of the climber
 */
void SimulationViewWidget::climberWeightUpdatedSlot(float weight){
    climberWeight = weight;
    sim.setClimberMass(climberWeight);
}

/**
 * @brief Updates the pulley height to be the new value supplied
 * @param height - the new height of the pulley
 */
void SimulationViewWidget::pulleyHeightUpdatedSlot(float height){
    boltHeight = height;
    sim.setPulleyHeight(boltHeight);
    drawWorld();
}

/**
 * @brief Updates the rope length of the pulley object in the simulation obejct
 * @param length - the new length of the rope
 */
void SimulationViewWidget::pulleyRopeLengthUpdatedSlot(float length){
    ropeLength = length;
    sim.setPulleyRopeLength(ropeLength);
    drawWorld();
}

void SimulationViewWidget::belayerForceSlot(bool direction){
    if(direction) // true == jump, false == sit
        sim.FORCE = 1000;
    else
        sim.FORCE = -1000;

    sim.forceFrameCounter = 0;
}
/**
 * @brief Helper method for resetting the simulation to its inital values. To be called
 * whenever the simulation window is hidden.
 */
void SimulationViewWidget::resetSimulationDataSlot(){
    worldUpdateTimer->stop();
    simRunning = false;
    climberHeightUpdatedSlot(climberHeight);
    climberWeightUpdatedSlot(climberWeight);
    sim.setClimberX(defaultX);
    belayerHeightUpdatedSlot(belayerHeight);
    belayerWeightUpdatedSlot(belayerWeight);
    sim.setBelayerX(defaultX+1);

    if (sim.hasPulley) {
        sim.world.DestroyJoint(sim.pulleyJoint);
    }
    sim.hasPulley = false;
    sim.world.ClearForces();
    sim.climber->SetLinearDamping(0.0f);
    sim.belayer->SetLinearDamping(0.0f);

}

/**
 * @brief Handles all drawing on the simulation widget.
 */
void SimulationViewWidget::drawWorld(){
    QPixmap map(this->width(), this->height());
    QPainter p(&map);
    QImage bg(":/Resources/background.JPG");
    p.drawImage(QRect(0, 0, this->width(), this->height()), bg);;
    b2Vec2 climberPosition(sim.getClimberPos());
    climberPosition.x = climberPosition.x*COORDINATE_SCALING;
    climberPosition.y = climberPosition.y*COORDINATE_SCALING;
    p.drawImage(QPoint(climberPosition.x+X_OFFSET-19, GROUND_LEVEL-climberPosition.y), QImage(":/Resources/climber2.png"));
    b2Vec2 belayerPosition(sim.getBelayerPos());
    belayerPosition.x = belayerPosition.x*COORDINATE_SCALING;
    belayerPosition.y = belayerPosition.y*COORDINATE_SCALING;
    p.drawImage(QPoint(belayerPosition.x+X_OFFSET-19, GROUND_LEVEL-belayerPosition.y), QImage(":/Resources/belayer2.png"));
    //b2Vec2 anchorPosition(sim.getPulleyPos());
    b2Vec2 anchorPosition = sim.bolt->GetPosition();
    p.setPen(QPen(QColor(0,0,255), 3));
    anchorPosition.x = anchorPosition.x*COORDINATE_SCALING;
    anchorPosition.y = anchorPosition.y*COORDINATE_SCALING;
    p.drawEllipse(anchorPosition.x+X_OFFSET-12, GROUND_LEVEL-anchorPosition.y-12, 24 , 24);
    setPixmap(map);

    drawRope(sim.climberRope, climberRopeBodyToLabel);
    drawRope(sim.belayerRope, belayerRopeBodyToLabel);

}

void SimulationViewWidget::drawRope(vector<b2Body*>& rope,  map<b2Body*, QLabel*>& map) {

    for (b2Body* ropeBody : rope) {
        //if(map.count(ropeBody) != 0) {
            // Add a new label representing the rope segments body
            QLabel* ropeLabel =  map.at(ropeBody);
            ropeLabel->setGeometry((ropeBody->GetPosition().x*COORDINATE_SCALING)+X_OFFSET, GROUND_LEVEL - (ropeBody->GetPosition().y)*COORDINATE_SCALING, (ropeWidth)*COORDINATE_SCALING, (ropeHeight)*COORDINATE_SCALING);

            // Create a new image for the label
            QImage imgFill = QImage((ropeWidth)*COORDINATE_SCALING, GROUND_LEVEL - (ropeHeight)*COORDINATE_SCALING, QImage::Format_ARGB32);
            imgFill.fill(QColor(200,0,175,255));
            ropeLabel->setPixmap(QPixmap::fromImage(imgFill));
        //}
    }
}
