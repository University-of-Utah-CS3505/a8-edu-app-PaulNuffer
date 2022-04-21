#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Box2D/Box2D.h"
#include <vector>
#include <QLabel>
#include <map>

using std::map;
using std::vector;

class simulator
{
public:
    simulator(float climberX, float climberY, float climberWeight,
              float belayerX, float belayerY, float belayerWeight,
              float boltX,    float boltY,    float ropeLength);
    ~simulator();

    void updateWorld(map<b2Body*, QLabel*>& climberBodyRopeToLabel, map<b2Body*, QLabel*>& belayerRopeBodyToLabel, QWidget* viewAddress);
    b2Vec2 getClimberPos();
    b2Vec2 getBelayerPos();
    b2Vec2 getPulleyPos();
    void createPulley(float x, float y, float length, b2Body* A, b2Body* B);
    void setClimberHeight(float height);
    void setBelayerHeight(float height);
    void setClimberMass(float mass);
    void setBelayerMass(float mass);
    void setClimberX(float x);
    void setBelayerX(float x);
    float getClimberMass();
    float getClimberForce();
    float getBelayerForce();
    b2Vec2 getPulleyPosition();
    void setPulleyHeight(float y);
    void setPulleyRopeLength(float length);

    //Rope code??
    vector<b2Body*> createRope(int length);
    vector<b2Body*> ropeBodies;

    b2World world;
    b2Body* climber;
    b2Body* belayer;
    b2Joint* pulleyJoint;

    b2Body* bolt;
    vector<b2Body*> climberRope;
    vector<b2Body*> belayerRope;
    const float SEGMENT_DENSITY = 1.5;

    //Rope code??
    float ropeWidth;
    float ropeHeight;

    float pulleyX;
    float pulleyY;
    float pulleyRopeLength;

    vector<b2Body*> connectRopeTo(b2Body* A, b2Body* B);
    vector<b2Body*> createRope(int numSegments, b2Vec2 vecA, b2Vec2 vecB);

    void updateRopes(map<b2Body*, QLabel*>& climberBodyRopeToLabel, map<b2Body*, QLabel*>& belayerRopeBodyToLabel, QWidget* viewAddress);

    int addSegments(b2Body* body, vector<b2Body*>& rope, map<b2Body*, QLabel*>& map, QWidget* viewAddress);

    void removeSegments(b2Body* body, vector<b2Body*>& rope, map<b2Body*, QLabel*>& map, int numSegmentsToRemove, QWidget* viewAddress);

};

#endif // SIMULATOR_H
