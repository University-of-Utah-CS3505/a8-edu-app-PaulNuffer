#include "simulator.h"
#include <math.h>

// Weights given in kg
// Distances given in meters
simulator::simulator(float climberX, float climberY, float climberWeight,
                     float belayerX, float belayerY, float belayerWeight,
                     float boltX,    float boltY,    float ropeLength)
          : world(b2Vec2(0.0f, -10.0f))
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
    createPulley(boltX, boltY, ropeLength, climber, belayer);
}

simulator::~simulator(){

}

void simulator::updateWorld() {
    // It is generally best to keep the time step and iterations fixed.
    world.Step(1.0/60.0, 6, 2);
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
    A->SetLinearDamping(1.0f);
    B->SetLinearDamping(1.0f);
    pulley.bodyA = A;
    pulley.bodyB = B;
    pulley.collideConnected = true;
    pulley.localAnchorA.Set(0, 0);
    pulley.localAnchorB.Set(0, 0);
    pulley.lengthA = length/2;
    pulley.lengthB = length/2;
    pulley.groundAnchorA.Set(x, y);
    pulley.groundAnchorB.Set(x, y);

    world.CreateJoint(&pulley);
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
 * @return the current force of the belayer in newtons.
 */
float simulator::getBelayerForce(){
    return (belayer->GetMass()*belayer->GetLinearVelocity().Length());
}

/**
 * @return the current force of the climber in newtons.
 */
float simulator::getClimberForce(){
    return (climber->GetMass()*belayer->GetLinearVelocity().Length());
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


//Rope code????
vector<b2Body*> simulator::createRope(int length) {
    vector<b2Body*> bodies;
    vector<b2RevoluteJoint*> joints;
    vector<b2RopeJoint*> ropeJoints;

    float offset = 0.05;

    float width = 0.08, height = 0.1;
    b2PolygonShape shape =  b2PolygonShape();
    shape.SetAsBox(width/2, height/2);

    b2Body *body;
    for(int i = 0; i < length; i++) {
        b2BodyDef bodyDef = b2BodyDef();
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(1, 30 - i*offset);


        body = world.CreateBody((&bodyDef));
        bodies.push_back(body);
        bodies.back()->CreateFixture(&shape, 1);
    }

    b2RevoluteJointDef jointDef = b2RevoluteJointDef();
    jointDef.localAnchorA.y = -height/1.3;
    jointDef.localAnchorB.y = height/1.3;

    for (int i = 0; i < length - 1; i++) {
        jointDef.bodyA = bodies.at(i);
        jointDef.bodyB = bodies.at(i+1);
        joints.push_back((b2RevoluteJoint*) world.CreateJoint(&jointDef));
    }

    b2RopeJointDef ropeJointDef = b2RopeJointDef();
    ropeJointDef.localAnchorA.Set(0, -height/1.3);
    ropeJointDef.localAnchorB.Set(0, height/1.3);
    ropeJointDef.maxLength = height;

    for (int i = 0; i < length - 1; i++) {
        ropeJointDef.bodyA = bodies.at(i);
        ropeJointDef.bodyB = bodies.at(i+1);
        ropeJoints.push_back((b2RopeJoint*) world.CreateJoint(&jointDef));
    }

    return bodies;
}
