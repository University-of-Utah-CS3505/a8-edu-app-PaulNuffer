#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QTimer>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      world(b2Vec2(0.0f, -10.0f)),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);

    ropeWidth = 0.05, ropeHeight = 0.05;


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

    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(5.0f, 4.0f);

    climber = world.CreateBody(&bodyDef);

    bodyDef.position.Set(3.0f, 0.0f);
    belayer = world.CreateBody(&bodyDef);

    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(0.25f, 0.25f);

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;

    // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.density = 1.0f;

    // Override the default friction.
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.9;
    // Add the shape to the body.
    climber->CreateFixture(&fixtureDef);
    belayer->CreateFixture(&fixtureDef);
    int boltx = 4;
    int bolty = 3;

    createPulley(boltx, bolty, 4, climber, belayer);

    QLabel* boltLabel = new QLabel(ui->centralwidget);
    boltLabel->setText("O");
    boltLabel->setGeometry(convertBox2dX(boltx) - boltLabel->width()/2, convertBox2dY(bolty) - boltLabel->height()/2, boltLabel->width(), boltLabel->height());


    //WALL PEG
    b2BodyDef pegBodyDef;
    pegBodyDef.position.Set(boltx, bolty);

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    bolt = world.CreateBody(&pegBodyDef);

    // Define the ground box shape.
    b2CircleShape boltShape;

    // The extents are the half-widths of the box.

    //pegShape.m_p.Set(2.0f, 3.0f);
    boltShape.m_radius = 0.1f;

    // Add the ground fixture to the ground body.
    bolt->CreateFixture(&boltShape, 0.0f);

    QLabel* pegLabel = new QLabel(ui->centralwidget);
    pegLabel->setText("O");
    pegLabel->setGeometry(convertBox2dX(bolt->GetPosition().x), convertBox2dY(bolt->GetPosition().y), pegLabel->width(), pegLabel->height());

    climberRope = connectRopeTo(bolt, climber);
    belayerRope = connectRopeTo(bolt, belayer);


    for (b2Body* ropeBody : climberRope) {
        // Add a new label representing the rope segments body
        QLabel* ropeLabel =  new QLabel(ui->centralwidget);
        ropeLabel->setGeometry(convertBox2dX(ropeBody->GetPosition().x), convertBox2dY(ropeBody->GetPosition().y), convertBox2dX(ropeWidth), convertBox2dX(ropeHeight));

        // Create a new image for the label
        QImage imgFill = QImage(convertBox2dX(ropeWidth), convertBox2dY(ropeHeight), QImage::Format_ARGB32);
        imgFill.fill(QColor(255,0,0,255));
//        QImage imgFillTrns = imgFill.transformed(QTransform().rotate(45));
        ropeLabel->setPixmap(QPixmap::fromImage(imgFill));

        // Add the body and label to the map
        climberRopeBodyToLabel.emplace(ropeBody, ropeLabel);
    }

    for (b2Body* ropeBody : belayerRope) {
        // Add a new label representing the rope segments body
        QLabel* ropeLabel =  new QLabel(ui->centralwidget);
        ropeLabel->setGeometry(convertBox2dX(ropeBody->GetPosition().x), convertBox2dY(ropeBody->GetPosition().y), convertBox2dX(ropeWidth), convertBox2dX(ropeHeight));

        // Create a new image for the label
        QImage imgFill = QImage(convertBox2dX(ropeWidth), convertBox2dY(ropeHeight), QImage::Format_ARGB32);
        imgFill.fill(QColor(255,0,0,255));
//        QImage imgFillTrns = imgFill.transformed(QTransform().rotate(45));
        ropeLabel->setPixmap(QPixmap::fromImage(imgFill));

        // Add the body and label to the map
        belayerRopeBodyToLabel.emplace(ropeBody, ropeLabel);
    }

    QTimer::singleShot(30, this, &MainWindow::updateWorld);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateWorld() {
    // It is generally best to keep the time step and iterations fixed.
    world.Step(1.0/60.0, 6, 2);

    updateRopes();

    // Now print the position and angle of the body.
    drawRope(climberRope, climberRopeBodyToLabel);
    drawRope(belayerRope, belayerRopeBodyToLabel);

    ui->climber->setGeometry(convertBox2dX(climber->GetPosition().x) - ui->climber->width()/2, convertBox2dY(climber->GetPosition().y) - ui->climber->height()/2, ui->climber->width(), ui->climber->height());
    ui->belayer->setGeometry(belayer->GetPosition().x*100, belayer->GetPosition().y*-100 + 500, ui->belayer->width(), ui->belayer->height());
    QTimer::singleShot(30, this, &MainWindow::updateWorld);
}


vector<b2Body*> MainWindow::createRope(int numSegments, b2Vec2 vecA, b2Vec2 vecB) {
    b2Body *body;
    vector<b2Body*> bodies;
    vector<b2RevoluteJoint*> revoluteJoints;
    vector<b2RopeJoint*> ropeJoints;

    // Defines shape of each rope segment
    b2PolygonShape shape =  b2PolygonShape();
    shape.SetAsBox(ropeWidth/2, ropeHeight/2);

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
        body = world.CreateBody((&bodyDef));
        body->CreateFixture(&shape, 0.01);

        // Revove collisions for the rope body
        body->GetFixtureList()->SetSensor(true);
        bodies.push_back(body);
    }

    // Define revolute joint
    b2RevoluteJointDef jointDef = b2RevoluteJointDef();
    jointDef.localAnchorA.y = -ropeHeight/100;
    jointDef.localAnchorB.y = ropeHeight/100;

    // Attatch each segment body to the next
    for (int i = 0; i < numSegments - 1; i++) {
        jointDef.bodyA = bodies.at(i);
        jointDef.bodyB = bodies.at(i+1);
        revoluteJoints.push_back((b2RevoluteJoint*) world.CreateJoint(&jointDef));
    }

    // Define rope joint
    b2RopeJointDef ropeJointDef = b2RopeJointDef();
    ropeJointDef.localAnchorA.Set(0, -ropeHeight/100);
    ropeJointDef.localAnchorB.Set(0, ropeHeight/100);
    ropeJointDef.maxLength = ropeHeight;

    // Attatch each segment body to the next
    for (int i = 0; i < numSegments - 1; i++) {
        ropeJointDef.bodyA = bodies.at(i);
        ropeJointDef.bodyB = bodies.at(i+1);
        ropeJoints.push_back((b2RopeJoint*) world.CreateJoint(&jointDef));
    }

    return bodies;
}

void MainWindow::createPulley(int x, int y, int length, b2Body* A, b2Body* B) {
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

vector<b2Body*> MainWindow::connectRopeTo(b2Body* A, b2Body* B) {

    // Calculate the distance from body A to body B
    float xA = A->GetPosition().x;
    float yA = A->GetPosition().y;
    float xB = B->GetPosition().x;
    float yB = B->GetPosition().y;
    float distance = sqrt(pow(xB-xA, 2) + pow(yB-yA, 2));

    // Determine number of segments to create
    int numRopeSegments = (SEGMENT_DENSITY*distance)/ropeHeight;

    vector<b2Body*> rope = createRope(numRopeSegments, A->GetPosition(), B->GetPosition());

    // Define a joint
    b2RevoluteJointDef jointDef = b2RevoluteJointDef();
    jointDef.localAnchorA.y = 0.1;
    jointDef.localAnchorB.y = 0.1;

    // Joint A and rope head together
    jointDef.bodyA = A;
    jointDef.bodyB = rope.at(0);
    world.CreateJoint(&jointDef);

    // Joint B and rope tail together
    jointDef.bodyA = B;
    jointDef.bodyB = rope.at(rope.size()-1);
    world.CreateJoint(&jointDef);

    return rope;
}

void MainWindow::drawRope(vector<b2Body*> rope,  map<b2Body*, QLabel*> map) {
    for (b2Body* ropeBody : rope) {

        // Add a new label representing the rope segments body
        QLabel* ropeLabel =  map.at(ropeBody);
        ropeLabel->setGeometry(convertBox2dX(ropeBody->GetPosition().x), convertBox2dY(ropeBody->GetPosition().y), convertBox2dX(ropeWidth), convertBox2dX(ropeHeight));

        // Create a new image for the label
        QImage imgFill = QImage(convertBox2dX(ropeWidth), convertBox2dY(ropeHeight), QImage::Format_ARGB32);
        imgFill.fill(QColor(200,0,175,255));
        QImage imgFillTrns = imgFill.transformed(QTransform().rotate(45));
        ropeLabel->setPixmap(QPixmap::fromImage(imgFill));
    }
}

// UPDATE ROPES HELPER METHOD

// find distance from bolt to climber
// find distance from bolt to belayer
// find # segments for each
// for each extra segment in the rope {linked list style remove}
// OR for each missing segment in the rope {add one}
// Adding a segment needs to cut a body and 2 joints to rope + one joint to climber
    // AND remove the joint to the climber from previous rope
// Removing a segment needs to cut a body and 2 joints to the rope + one to the climber
    // AND add one to the climber

void MainWindow::updateRopes() {

    // Get positions
    float boltX = bolt->GetPosition().x;
    float boltY = bolt->GetPosition().y;
    float climberX = climber->GetPosition().x;
    float climberY = climber->GetPosition().y;

    // Find distance from climber to bolt
    float distanceClimberToBolt = sqrt(pow(boltX-climberX, 2) + pow(boltY-climberY, 2));

    // Find desired number of segments
    int numClimberSegmentsDesired = (SEGMENT_DENSITY*distanceClimberToBolt)/ropeHeight;

    // If climber moves away from bolt
    if(numClimberSegmentsDesired > (int) climberRope.size()) {
        addSegments(climber, climberRope, climberRopeBodyToLabel);
        removeSegments(belayer, belayerRope, belayerRopeBodyToLabel);
    }
    // If climber moves towards bolt
    else {
        addSegments(belayer, belayerRope, belayerRopeBodyToLabel);
        removeSegments(climber, climberRope, climberRopeBodyToLabel);
    }
}

void MainWindow::addSegments(b2Body* body, vector<b2Body*> rope, map<b2Body*, QLabel*> map) {

    // Delete the end of the rope, which is connected to the climber
    b2Body* endRope = rope.at(rope.size()-1);
    // rope.erase(rope.end());
    rope.pop_back();
    map.erase(endRope);
    world.DestroyBody(endRope);

    // List of rope bodies we will add
    vector<b2Body*> ropeBodiesToAdd = connectRopeTo(rope.at(rope.size()-1), body);

    // Update map to connect labels to each body
    for(b2Body* ropeBody: ropeBodiesToAdd){
        rope.push_back(ropeBody);
        // Add a new label representing the rope segments body
        QLabel* ropeLabel =  new QLabel(ui->centralwidget);
        ropeLabel->setGeometry(convertBox2dX(ropeBody->GetPosition().x), convertBox2dY(ropeBody->GetPosition().y), convertBox2dX(ropeWidth), convertBox2dX(ropeHeight));

        // Create a new image for the label
        QImage imgFill = QImage(convertBox2dX(ropeWidth), convertBox2dY(ropeHeight), QImage::Format_ARGB32);
        imgFill.fill(QColor(255,0,0,255));
        ropeLabel->setPixmap(QPixmap::fromImage(imgFill));

        // Add the body and label to the map
        map.emplace(ropeBody, ropeLabel);
    }
}

void MainWindow::removeSegments(b2Body* body, vector<b2Body*> rope, map<b2Body*, QLabel*> map) {
    float boltX = bolt->GetPosition().x;
    float boltY = bolt->GetPosition().y;
    float bodyX = body->GetPosition().x;
    float bodyY = body->GetPosition().y;

    // Find distances of climber to bolt
    float distanceBodyToBolt = sqrt(pow(boltX-bodyX, 2) + pow(boltY-bodyY, 2));

    // Find desired number of segments
    int numSegmentsDesired = (SEGMENT_DENSITY*distanceBodyToBolt)/ropeHeight;

    // Fully delete all of the extra rope bodies
    // Remove one additional rope body because we will need to add one back
    for(int i = 0; i < (int) (numSegmentsDesired - rope.size()) + 1; i++){
        b2Body* endRope = rope.at(rope.size()-1);
        rope.pop_back();
        //rope.erase(rope.end());
        map.erase(endRope);

        world.DestroyBody(endRope);
    }

    // Add one rope body and connect it to the climber
    vector<b2Body*> ropeToAdd = connectRopeTo(rope.at(rope.size()-1), climber);
    rope.push_back(ropeToAdd.at(0));

    // Add a new label representing the rope segments body
    QLabel* ropeLabel =  new QLabel(ui->centralwidget);
    ropeLabel->setGeometry(convertBox2dX(ropeToAdd.at(0)->GetPosition().x), convertBox2dY(ropeToAdd.at(0)->GetPosition().y), convertBox2dX(ropeWidth), convertBox2dX(ropeHeight));

    // Create a new image for the label
    QImage imgFill = QImage(convertBox2dX(ropeWidth), convertBox2dY(ropeHeight), QImage::Format_ARGB32);
    imgFill.fill(QColor(255,0,0,255));
    ropeLabel->setPixmap(QPixmap::fromImage(imgFill));

    // Add the body and label to the map
    map.emplace(ropeToAdd.at(0), ropeLabel);
}

int MainWindow::convertBox2dX(float input){
    return (int)(input * 100);
}

int MainWindow::convertBox2dY(float input){
    return (int)(input*-100)+500;
}
