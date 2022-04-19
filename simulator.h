#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Box2D/Box2D.h"
#include <vector>

using std::vector;

class simulator
{
public:
    simulator(float climberX, float climberY, float climberWeight,
              float belayerX, float belayerY, float belayerWeight,
              float boltX,    float boltY,    float ropeLength);
    ~simulator();

    void updateWorld();
    b2Vec2 getClimberPos();
    b2Vec2 getBelayerPos();
    void createPulley(float x, float y, float length, b2Body* A, b2Body* B);
    void setClimberHeight(float height);
    void setBelayerHeight(float height);
    void setClimberMass(float mass);
    void setBelayerMass(float mass);
    float getClimberMass();
    float getClimberForce();
    float getBelayerForce();

    //Rope code??
    vector<b2Body*> createRope(int length);
    vector<b2Body*> ropeBodies;
private:
    b2World world;
    b2Body* climber;
    b2Body* belayer;

    //Rope code??
    float ropeWidth;
    float ropeHeight;
};

#endif // SIMULATOR_H
