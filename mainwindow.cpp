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

    ropeWidth = 0.08, ropeHeight = 0.1;


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

    createPulley(boltx, bolty, 2, climber, belayer);

    QLabel* boltLabel = new QLabel(ui->centralwidget);
    boltLabel->setText("O");
    boltLabel->setGeometry(convertBox2dX(boltx) - boltLabel->width()/2, convertBox2dY(bolty) - boltLabel->height()/2, boltLabel->width(), boltLabel->height());


    //WALL PEG
    b2BodyDef pegBodyDef;
    pegBodyDef.position.Set(1.0f, 4.0f);

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* pegBody = world.CreateBody(&pegBodyDef);

    // Define the ground box shape.
    b2CircleShape pegShape;

    // The extents are the half-widths of the box.

    //pegShape.m_p.Set(2.0f, 3.0f);
    pegShape.m_radius = 0.1f;

    // Add the ground fixture to the ground body.
    pegBody->CreateFixture(&pegShape, 0.0f);

    QLabel* pegLabel = new QLabel(ui->centralwidget);
    pegLabel->setText("O");
    pegLabel->setGeometry(convertBox2dX(pegBody->GetPosition().x), convertBox2dY(pegBody->GetPosition().y), pegLabel->width(), pegLabel->height());


    ropeBodies = createRope(80);
    for (b2Body* ropeBody : ropeBodies) {
        //QLabel* ropeLabel =  new QLabel("#");
        //ui->verticalLayout->addWidget(ropeLabel);

        //b2Vec2 force(5.0f, 5.0f);
        //ropeBody->ApplyForceToCenter(force, true);

        QLabel* ropeLabel =  new QLabel(ui->centralwidget);
        ropeLabel->setGeometry(convertBox2dX(ropeBody->GetPosition().x), convertBox2dY(ropeBody->GetPosition().y), ropeLabel->width(), ropeLabel->height());
        QImage imgFill = QImage(convertBox2dX(ropeWidth), convertBox2dY(ropeHeight), QImage::Format_ARGB32);
        imgFill.fill(QColor(255,0,0,255));
        QImage imgFillTrns = imgFill.transformed(QTransform().rotate(45));
//        pntr.drawRect(0, 0, imgFill.width(), imgFill.height());
        ropeLabel->setPixmap(QPixmap::fromImage(imgFill));
        ropeBodyToLabel.emplace(ropeBody, ropeLabel);
    }

//    connect(this, &MainWindow::newPos,
//            this, &MainWindow::setClimberY);

    QTimer::singleShot(30, this, &MainWindow::updateWorld);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateWorld() {
    // It is generally best to keep the time step and iterations fixed.
    world.Step(1.0/60.0, 6, 2);

    // Now print the position and angle of the body.
    //b2Vec2 position = climber->GetPosition();

    //emit newPos(position.y);

    int i = 0;
    for (b2Body* ropeBody : ropeBodies) {
        //QLabel* ropeLabel =  new QLabel("#");
        //ui->verticalLayout->addWidget(ropeLabel);
        QLabel* ropeLabel = ropeBodyToLabel.at(ropeBody);
        ropeLabel->setGeometry(convertBox2dX(ropeBody->GetPosition().x), convertBox2dY(ropeBody->GetPosition().y), ropeLabel->width(), ropeLabel->height());
        QImage* imgFill = new QImage(convertBox2dX(ropeWidth), convertBox2dY(ropeHeight), QImage::Format_ARGB32);
        imgFill->fill(QColor(255,0,0,255));
        QImage imgFillTrns = imgFill->transformed(QTransform().rotate(45));
//        pntr.drawRect(0, 0, imgFill.width(), imgFill.height());
        ropeLabel->setPixmap(QPixmap::fromImage(*imgFill));
        //std::cout << i++ << ":" << ropeBody->GetAngle() << " : " << pntr.transform().isRotating() << std::endl;
    }

//    for (int i = 0; i < ropeBodies.size()-1; i++) {
//        b2Vec2 pos1 = ropeBodies[i]->GetPosition();
//        b2Vec2 pos2 = ropeBodies[i+1]->GetPosition();
//        QImage image()
//        QPainter painter(ui->centralwidget);


//        painter.setPen(Qt::blue);
//        painter.drawLine(pos1.x, pos1.y, pos2.x, pos2.y);
//    }
    ui->climber->setGeometry(convertBox2dX(climber->GetPosition().x) - ui->climber->width()/2, convertBox2dY(climber->GetPosition().y) - ui->climber->height()/2, ui->climber->width(), ui->climber->height());
    ui->belayer->setGeometry(belayer->GetPosition().x*100, belayer->GetPosition().y*-100 + 500, ui->belayer->width(), ui->belayer->height());
    QTimer::singleShot(30, this, &MainWindow::updateWorld);
}

//void MainWindow::setClimberY(int y) {
//    ui->climber->setGeometry(climber->GetPosition().x*100, y*-100 + 500, ui->climber->width(), ui->climber->height());

//}

vector<b2Body*> MainWindow::createRope(int length) {
    vector<b2Body*> bodies;
    vector<b2RevoluteJoint*> joints;
    vector<b2RopeJoint*> ropeJoints;

    float offset = 0.05;

//    b2BodyDef bodyDef = b2BodyDef();
//    bodyDef.type = b2_dynamicBody;

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

int MainWindow::convertBox2dX(float input){
    return (int)(input * 100);
}

int MainWindow::convertBox2dY(float input){
    return (int)(input*-100)+500;
}
