#include "simulator.h"
#include <math.h>
#include <iostream>

// Weights given in kg
// Distances given in meters
simulator::simulator(float climberX, float climberY, float climberWeight,
                     float belayerX, float belayerY, float belayerWeight,
                     float boltX,    float boltY,    float ropeLength)
          : world(b2Vec2(0.0f, -10.0f)), pulleyX(boltX), pulleyY(boltY), pulleyRopeLength(ropeLength)
{
    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2PolygonShape groundBox;

    // The extents are the half-widths of the box.
    groundBox.SetAsBox(50.0f, 10.0f);

    // Add the ground fixture to the ground body.
    groundBody->CreateFixture(&groundBox, 0.0f);


    // Define the dynamic bodies. We set its position and call the body factory.
    b2BodyDef climberBodyDef;
    b2BodyDef belayerBodyDef;
    climberBodyDef.type = b2_dynamicBody;
    belayerBodyDef.type = b2_dynamicBody;
    climberBodyDef.position.Set(climberX, climberY);
    belayerBodyDef.position.Set(belayerX, belayerY);

    climber = world.CreateBody(&climberBodyDef);
    belayer = world.CreateBody(&belayerBodyDef);

    // Define another box shape for our dynamic body.
    b2PolygonShape climberDynamicBox;
    b2PolygonShape belayerDynamicBox;
    climberDynamicBox.SetAsBox(0.25f, 0.25f);
    belayerDynamicBox.SetAsBox(0.25f, 0.25f);

    // Define the dynamic body fixture.
    b2FixtureDef climberFixtureDef;
    b2FixtureDef belayerFixtureDef;
    climberFixtureDef.shape = &climberDynamicBox;
    belayerFixtureDef.shape = &belayerDynamicBox;

    // Set the box density to be non-zero, so it will be dynamic.
    climberFixtureDef.density = climberWeight*4;
    belayerFixtureDef.density = belayerWeight*4;

    // Override the default friction.
    climberFixtureDef.friction = 0.3f;
    belayerFixtureDef.friction = 0.3f;
    climberFixtureDef.restitution = 0.1;
    belayerFixtureDef.restitution = 0.1;

    // Add the shape to the body.
    climber->CreateFixture(&climberFixtureDef);
    belayer->CreateFixture(&belayerFixtureDef);


    // Create a pulley
    // createPulley(boltX, boltY, ropeLength, climber, belayer);

    b2BodyDef boltDef;
    boltDef.position.Set(boltX, boltY);
    bolt = world.CreateBody(&boltDef);

    b2CircleShape boltShape;
    boltShape.m_radius = 0.01f;
    bolt->CreateFixture(&boltShape, 0.0f);
    bolt->GetFixtureList()->SetSensor(true);

    climberRope = connectRopeTo(bolt, climber);
    belayerRope = connectRopeTo(bolt, belayer);

    climber->GetFixtureList()->SetSensor(true);

    hasPulley = false;
}

simulator::~simulator(){
}

void simulator::updateWorld(map<b2Body*, QLabel*>* climberBodyRopeToLabel, map<b2Body*, QLabel*>* belayerRopeBodyToLabel, QWidget* viewAddress) {
    // It is generally best to keep the time step and iterations fixed.
    world.Step(1.0/60.0, 6, 2);
    if (climber->GetPosition().y < bolt->GetPosition().y) {
        updateRopes(climberBodyRopeToLabel, belayerRopeBodyToLabel, viewAddress);
    }

    float climberFromBoltDistance = pow((pow(climber->GetPosition().x - bolt->GetPosition().x, 2),
                                         pow(climber->GetPosition().y - bolt->GetPosition().y, 2)), 0.5);
    float belayerFromBoltDistance = pow((pow(belayer->GetPosition().x - bolt->GetPosition().x, 2),
                                         pow(belayer->GetPosition().y - bolt->GetPosition().y, 2)), 0.5);

    if(climberFromBoltDistance >= pulleyRopeLength - belayerFromBoltDistance && !hasPulley) {
        // Create a pulley
        createPulley(bolt->GetPosition().x, bolt->GetPosition().y, pulleyRopeLength, climber, belayer);
        hasPulley = true;
    }
    if(forceFrameCounter >= 0 && hasPulley){
        belayer->ApplyForceToCenter(b2Vec2(0, FORCE), true);
        forceFrameCounter++;
        if(forceFrameCounter >= 30){
            forceFrameCounter = -1;
        }
    }
}

b2Vec2 simulator::getClimberPos() {
    return climber->GetPosition();
}

b2Vec2 simulator::getBelayerPos() {
    return belayer->GetPosition();
}
// Creates a bolt at x,y
// The pulley rope has length length
// Connects A to one end, B to the other
void simulator::createPulley(float x, float y, float length, b2Body* A, b2Body* B) {
    b2PulleyJointDef pulley = b2PulleyJointDef();
    A->SetLinearDamping(3.0f);
    B->SetLinearDamping(3.0f);
    pulley.bodyA = A;
    pulley.bodyB = B;
    pulley.collideConnected = false;
    pulley.localAnchorA.Set(0, 0);
    pulley.localAnchorB.Set(0, 0);
    pulley.lengthA = length/2;
    pulley.lengthB = length/2;
    pulley.groundAnchorA.Set(x, y);
    pulley.groundAnchorB.Set(x, y);

    pulleyJoint = world.CreateJoint(&pulley);
}

/**
 * @brief Creates a new pulley. Since we could not figure out a way to set the joint position after creation, this
 * is our next best workaround.
 * @param y - new height
 */
void simulator::setPulleyHeight(float y){
        b2Vec2 newBoltPos(pulleyX, y);
        bolt->SetTransform(newBoltPos, 0);
}

/**
 * @brief Creates a new pulley. Since we could not figure out a way to set the joint length after creation, this
 * is our next best workaround.
 * @param length - new length
 */
void simulator::setPulleyRopeLength(float length){
    pulleyRopeLength = length;
}

/**
 * @brief Sets the belayer's height to a new value.
 * @param height
 */
void simulator::setBelayerHeight(float height){
    belayer->SetTransform(b2Vec2(belayer->GetPosition().x, height), 0);
}

/**
 * @brief Sets the climber's height to a new value.
 * @param height
 */
void simulator::setClimberHeight(float height){
    climber->SetTransform(b2Vec2(climber->GetPosition().x, height), 0);
}

/**
 * @brief Sets the belayer's height to a new value.
 * @param height
 */
void simulator::setBelayerX(float x){
    belayer->SetTransform(b2Vec2(x, belayer->GetPosition().y), 0);
}

/**
 * @brief Sets the climber's height to a new value.
 * @param height
 */
void simulator::setClimberX(float x){
    climber->SetTransform(b2Vec2(x, climber->GetPosition().y), 0);
}

/**
 * @return the current force of the belayer in newtons.
 */
float simulator::getBelayerForce(){
    return (belayer->GetMass()*belayer->GetLinearVelocity().Length());
}

/**
 * @return the current force of the climber in newtons.
 */
float simulator::getClimberForce(){
    return (climber->GetMass()*climber->GetLinearVelocity().Length());
}

b2Vec2 simulator::getPulleyPos(){
    return b2Vec2(pulleyX, pulleyY);
}

/**
 * @brief simulator::setClimberMass
 * @param mass
 */
void simulator::setClimberMass(float mass){
    b2MassData newMass;
    climber->GetMassData(&newMass);
    newMass.mass=mass;
    climber->SetMassData(&newMass);
}

/**
 * @brief simulator::setClimberMass
 * @param mass
 */
void simulator::setBelayerMass(float mass){
    b2MassData newMass;
    belayer->GetMassData(&newMass);
    newMass.mass=mass;
    belayer->SetMassData(&newMass);
}

/**
 * @return the climber's weight.
 */
float simulator::getClimberMass(){
    return climber->GetMass();
}


vector<b2Body*> simulator::createRope(int numSegments, b2Vec2 vecA, b2Vec2 vecB) {
    b2Body *body;
    vector<b2Body*> bodies;
    vector<b2RevoluteJoint*> revoluteJoints;
    vector<b2RopeJoint*> ropeJoints;

    // Defines shape of each rope segment
    b2PolygonShape shape =  b2PolygonShape();
    shape.SetAsBox(ROPE_WIDTH/2, ROPE_HEIGHT/2);

    // Determine how far apart each segment should be
    // from the previous segment
    float offsetX = (vecB.x - vecA.x)/numSegments;
    float offsetY = (vecB.y - vecA.y)/numSegments;

    // Define and create each segment body of the rope
    for(int i = 0; i < numSegments; i++) {
        b2BodyDef bodyDef = b2BodyDef();
        bodyDef.type = b2_dynamicBody;

        // Place the segment with the offsets
        bodyDef.position.Set(vecA.x + offsetX*i, vecA.y + offsetY*i);
        bodyDef.linearDamping = 1.5f;
        body = world.CreateBody((&bodyDef));
        body->CreateFixture(&shape, 0.8f);

        // Revove collisions for the rope body
        body->GetFixtureList()->SetSensor(true);
        bodies.push_back(body);
    }

    // Define revolute joint
    b2RevoluteJointDef jointDef = b2RevoluteJointDef();
    jointDef.localAnchorA.y = -ROPE_HEIGHT/100;
    jointDef.localAnchorB.y = ROPE_HEIGHT/100;

    // Attatch each segment body to the next
    for (int i = 0; i < numSegments - 1; i++) {
        jointDef.bodyA = bodies.at(i);
        jointDef.bodyB = bodies.at(i+1);
        revoluteJoints.push_back((b2RevoluteJoint*) world.CreateJoint(&jointDef));
    }

    // Define rope joint
    b2RopeJointDef ropeJointDef = b2RopeJointDef();
    ropeJointDef.localAnchorA.Set(0, -ROPE_HEIGHT/100);
    ropeJointDef.localAnchorB.Set(0, ROPE_HEIGHT/100);
    ropeJointDef.maxLength = ROPE_HEIGHT;

    // Attatch each segment body to the next
    for (int i = 0; i < numSegments - 1; i++) {
        ropeJointDef.bodyA = bodies.at(i);
        ropeJointDef.bodyB = bodies.at(i+1);
        ropeJoints.push_back((b2RopeJoint*) world.CreateJoint(&jointDef));
    }

    return bodies;
}

vector<b2Body*> simulator::connectRopeTo(b2Body* A, b2Body* B) {

    // Calculate the distance from body A to body B
    float xA = A->GetPosition().x;
    float yA = A->GetPosition().y;
    float xB = B->GetPosition().x;
    float yB = B->GetPosition().y;
    float distance = sqrt(pow(xB-xA, 2) + pow(yB-yA, 2));

    // Determine number of segments to create
    int numRopeSegments = (SEGMENT_DENSITY*distance)/ROPE_HEIGHT;
    if(numRopeSegments == 0){
        vector<b2Body*> empty;
        return empty;
    }
    vector<b2Body*> rope = createRope(numRopeSegments, A->GetPosition(), B->GetPosition());

    // Define a rope joint
    b2RopeJointDef ropeJointDef = b2RopeJointDef();
    ropeJointDef.localAnchorA.Set(0, -ROPE_HEIGHT/100);
    ropeJointDef.localAnchorB.Set(0, ROPE_HEIGHT/100);
    ropeJointDef.maxLength = ROPE_HEIGHT;

    // Attatch each segment body to the next
    ropeJointDef.bodyA = A;
    ropeJointDef.bodyB = rope.at(0);
    world.CreateJoint(&ropeJointDef);

    // Attatch each segment body to the next
    ropeJointDef.bodyA = B;
    ropeJointDef.bodyB = rope.back();
    world.CreateJoint(&ropeJointDef);

    // Define a revolute joint
    b2RevoluteJointDef revoluteJointDef = b2RevoluteJointDef();
    revoluteJointDef.localAnchorA.y = -ROPE_HEIGHT/100;
    revoluteJointDef.localAnchorB.y = ROPE_HEIGHT/100;


    // Joint A and rope head together
    revoluteJointDef.bodyA = A;
    revoluteJointDef.bodyB = rope.at(0);
    world.CreateJoint(&revoluteJointDef);

    // Joint B and rope tail together
    revoluteJointDef.bodyA = B;
    revoluteJointDef.bodyB = rope.back();
    world.CreateJoint(&revoluteJointDef);

    return rope;
}

void simulator::updateRopes(map<b2Body*, QLabel*>* climberRopeBodyToLabel, map<b2Body*, QLabel*>* belayerRopeBodyToLabel, QWidget* viewAddress) {
    // Get positions
    float boltX = bolt->GetPosition().x;
    float boltY = bolt->GetPosition().y;
    float climberX = climber->GetPosition().x;
    float climberY = climber->GetPosition().y;

    // Find distance from climber to bolt
    float distanceClimberToBolt = sqrt(pow(boltX-climberX, 2) + pow(boltY-climberY, 2));

    // Find desired number of segments
    int numClimberSegmentsDesired = (SEGMENT_DENSITY*distanceClimberToBolt)/ROPE_HEIGHT;

    // If climber moves away from bolt
    if(numClimberSegmentsDesired > (int)climberRope.size()) {
        int numSegments = addSegments(climber, climberRope, climberRopeBodyToLabel, viewAddress);
        removeSegments(belayer, belayerRope, belayerRopeBodyToLabel, numSegments, viewAddress);
    }
    // If climber moves towards bolt
    else if(numClimberSegmentsDesired < (int)climberRope.size()){
        int numSegments = addSegments(belayer, belayerRope, belayerRopeBodyToLabel, viewAddress);
        removeSegments(climber, climberRope, climberRopeBodyToLabel, numSegments, viewAddress);
    }
}

int simulator::addSegments(b2Body* body, vector<b2Body*>& rope, map<b2Body*, QLabel*>* map, QWidget* viewAddress) {

    // Delete the end of the rope, which is connected to the climber
    b2Body* endRope = rope.back();
    rope.pop_back();
    map->at(endRope)->~QLabel();
    map->erase(endRope);

    // Destroy the rope segment
    world.DestroyBody(endRope);

    // List of rope bodies we will add
    vector<b2Body*> ropeBodiesToAdd = connectRopeTo(rope.back(), body);

    // CONNECT LAST SEGMENT TO BODY
    // Define a rope joint
    b2RopeJointDef ropeJointDef = b2RopeJointDef();
    ropeJointDef.localAnchorA.Set(0, -ROPE_HEIGHT/100);
    ropeJointDef.localAnchorB.Set(0, ROPE_HEIGHT/100);
    ropeJointDef.maxLength = ROPE_HEIGHT;

    // Attatch each segment body to the next
    ropeJointDef.bodyA = body;
    ropeJointDef.bodyB = rope.back();
    world.CreateJoint(&ropeJointDef);

    // Define a revolute joint
    b2RevoluteJointDef revoluteJointDef = b2RevoluteJointDef();
    revoluteJointDef.localAnchorA.y = -ROPE_HEIGHT/100;
    revoluteJointDef.localAnchorB.y = ROPE_HEIGHT/100;

    // Joint B and rope tail together
    revoluteJointDef.bodyA = body;
    revoluteJointDef.bodyB = rope.back();
    world.CreateJoint(&revoluteJointDef);

    if(ropeBodiesToAdd.size() == 0)
            return -1;

    // Update map to connect labels to each body
    for(b2Body* ropeBody: ropeBodiesToAdd){
        rope.push_back(ropeBody);
        // Add a new label representing the rope segments body
        QLabel* ropeLabel =  new QLabel(viewAddress);
        ropeLabel->setGeometry(15*(ropeBody->GetPosition().x), 600-(ropeBody->GetPosition().y)*10, 15*(ROPE_WIDTH), 15*(ROPE_HEIGHT));

        // Create a new image for the label
        QImage imgFill = QImage(15*(ROPE_WIDTH), 600-(ROPE_HEIGHT)*10, QImage::Format_ARGB32);
        imgFill.fill(QColor(255,0,0,255));
        ropeLabel->setPixmap(QPixmap::fromImage(imgFill));

        // Add the body and label to the map
        map->emplace(ropeBody, ropeLabel);
    }
    return ropeBodiesToAdd.size() - 2;
}

void simulator::removeSegments(b2Body* body, vector<b2Body*>& rope, map<b2Body*, QLabel*>* map, int numSegmentsToRemove, QWidget* viewAddress) {
    if(numSegmentsToRemove <= 0)
        return;
    // Fully delete all of the extra rope bodies
    // Remove one additional rope body because we will need to add one back
    for(int i = 0; i <= numSegmentsToRemove; i++){
        // Remove the rope segment from the rope
        b2Body* endRope = rope.back();
        rope.pop_back();

        // Clear the label and delete it
        map->at(endRope)->~QLabel();
        map->erase(endRope);

        // Destroy and remove the segment from the world
        world.DestroyBody(endRope);
    }

    // Add one rope body and connect it to the climber
    //vector<b2Body*> ropeToAdd = connectRopeTo(rope.back(), climber);
    //rope.push_back(ropeToAdd.at(0));
    vector<b2Body*> ropeToAdd = createRope(1, rope.back()->GetPosition(), body->GetPosition());
    b2Body* newRopeBody = ropeToAdd.front();

    // Define a joint
    b2RevoluteJointDef revoluteJointDef = b2RevoluteJointDef();
    revoluteJointDef.localAnchorA.y = -ROPE_HEIGHT/100;
    revoluteJointDef.localAnchorB.y = ROPE_HEIGHT/100;

    // Joint A and rope head together
    revoluteJointDef.bodyA = rope.back();
    revoluteJointDef.bodyB = newRopeBody;
    world.CreateJoint(&revoluteJointDef);

    // Joint B and rope tail together
    revoluteJointDef.bodyA = body;
    revoluteJointDef.bodyB = newRopeBody;
    world.CreateJoint(&revoluteJointDef);

    // Define rope joint
    b2RopeJointDef ropeJointDef = b2RopeJointDef();
    ropeJointDef.localAnchorA.Set(0, -ROPE_HEIGHT/100);
    ropeJointDef.localAnchorB.Set(0, ROPE_HEIGHT/100);
    ropeJointDef.maxLength = ROPE_HEIGHT;

    ropeJointDef.bodyA = rope.back();
    ropeJointDef.bodyB = newRopeBody;
    world.CreateJoint(&ropeJointDef);

    ropeJointDef.bodyA = body;
    ropeJointDef.bodyB = newRopeBody;
    world.CreateJoint(&ropeJointDef);

    // Add a new label representing the rope segments body
    QLabel* ropeLabel =  new QLabel(viewAddress);
    ropeLabel->setGeometry(15*(newRopeBody->GetPosition().x), 600-(newRopeBody->GetPosition().y)*10, 15*(ROPE_WIDTH), 15*(ROPE_HEIGHT));

    // Create a new image for the label
    QImage imgFill = QImage(15*(ROPE_WIDTH), 600-(ROPE_HEIGHT)*10, QImage::Format_ARGB32);
    imgFill.fill(QColor(255,0,0,255));
    ropeLabel->setPixmap(QPixmap::fromImage(imgFill));

    // Add the body and label to the map
    map->emplace(newRopeBody, ropeLabel);
    ropeToAdd.clear();
}
