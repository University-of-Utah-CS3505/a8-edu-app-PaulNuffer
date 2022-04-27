#include <QMessageBox>
#include "coursemodewindow.h"
#include "ui_coursemodewindow.h"

SimulationViewWidget* simView;

CourseModeWindow::CourseModeWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CourseModeWindow)
{
    ui->setupUi(this);

    simView = new SimulationViewWidget(ui->simulationFrame);
    simView->setGeometry(0,0,691,671);
    simView->setObjectName(QString::fromUtf8("simulationViewWidget"));
    simView->setEnabled(true);
    simView->setStyleSheet(QString("QWidget {background-color: rgb(230,240,255);}"));
    simView->show();
    simView->raise();
    ui->mainMenuButton->raise();

    applyStylesheets();

    //Connections
    connect(simView,
            &SimulationViewWidget::worldUpdated,
            this,
            &CourseModeWindow::updateWindow);
    connect(this,
            &CourseModeWindow::simulationStartSignal,
            simView,
            &SimulationViewWidget::simulationStartSlot);
    connect(this,
            &CourseModeWindow::simulationStopSignal,
            simView,
            &SimulationViewWidget::simulationStopSlot);
    connect(this,
            &CourseModeWindow::belayerHeightUpdatedSignal,
            simView,
            &SimulationViewWidget::belayerHeightUpdatedSlot);
    connect(this,
            &CourseModeWindow::climberHeightUpdatedSignal,
            simView,
            &SimulationViewWidget::climberHeightUpdatedSlot);
    connect(this,
            &CourseModeWindow::belayerWeightUpdatedSignal,
            simView,
            &SimulationViewWidget::belayerWeightUpdatedSlot);
    connect(this,
            &CourseModeWindow::climberWeightUpdatedSignal,
            simView,
            &SimulationViewWidget::climberWeightUpdatedSlot);
    connect(this,
            &CourseModeWindow::resetSimulationDataSignal,
            simView,
            &SimulationViewWidget::resetSimulationDataSlot);
    connect(this,
            &CourseModeWindow::pulleyHeightUpdatedSignal,
            simView,
            &SimulationViewWidget::pulleyHeightUpdatedSlot);
    connect(this,
            &CourseModeWindow::pulleyRopeLengthUpdatedSignal,
            simView,
            &SimulationViewWidget::pulleyRopeLengthUpdatedSlot);

    level_t obj;
    //stage 1
        obj.climberHeight=b2Vec2(5, 35);
        obj.climberWeight=72;
        obj.belayerHeight=b2Vec2(6, 0);
        obj.belayerWeight=70;
        obj.boltHeight=b2Vec2(5.5, 30);
        obj.correctAnswer=level_t::jump;
        obj.answerExplanation="Reason stage 1";
        obj.stageDescription="Stage description 1";
    courseStages.push_back(obj);
    //stage 2
        obj.climberHeight=b2Vec2(5, 50);
        obj.climberWeight=72;
        obj.belayerHeight=b2Vec2(6, 0);
        obj.belayerWeight=70;
        obj.boltHeight=b2Vec2(5.5, 30);
        obj.correctAnswer=level_t::stay;
        obj.answerExplanation="Reason stage 2";
        obj.stageDescription="Stage description 2";
    courseStages.push_back(obj);
    //stage 3
        obj.climberHeight=b2Vec2(5, 35);
        obj.climberWeight=200;
        obj.belayerHeight=b2Vec2(6, 0);
        obj.belayerWeight=70;
        obj.boltHeight=b2Vec2(5.5, 30);
        obj.correctAnswer=level_t::sit;
        obj.answerExplanation="Reason stage 3";
        obj.stageDescription="Stage description 3";
    courseStages.push_back(obj);
    //stage 4
        obj.climberHeight=b2Vec2(5, 35);
        obj.climberWeight=25;
        obj.belayerHeight=b2Vec2(6, 0);
        obj.belayerWeight=99;
        obj.boltHeight=b2Vec2(5.5, 30);
        obj.correctAnswer=level_t::jump;
        obj.answerExplanation="Reason stage 4";
        obj.stageDescription="Stage description 4";
    courseStages.push_back(obj);
    //stage 5
        obj.climberHeight=b2Vec2(5, 10);
        obj.climberWeight=72;
        obj.belayerHeight=b2Vec2(6, 0);
        obj.belayerWeight=70;
        obj.boltHeight=b2Vec2(5.5, 6);
        obj.correctAnswer=level_t::jump;
        obj.answerExplanation="Reason stage 5";
        obj.stageDescription="Stage description 5";
    courseStages.push_back(obj);
}

CourseModeWindow::~CourseModeWindow()
{
    delete ui;
    delete simView;
}

/**
 * @brief Sets the course window to display all widget elements necessary for course mode.
 */
void CourseModeWindow::setupWindowAsCourseMode(){
    ui->belayerWeightValueSpinbox->setEnabled(false);
    ui->belayerHeightValueSpinbox->setEnabled(false);
    ui->leaderWeightValueSpinbox->setEnabled(false);
    ui->leaderHeightValueSpinbox->setEnabled(false);
    ui->boltHeightValueSpinbox->setEnabled(false);
    ui->ropeLengthValueSpinbox->setEnabled(false);
    ui->runSimulationButton->setEnabled(false);
    ui->runSimulationButton->hide();
    ui->jumpButton->setEnabled(true);
    ui->sitButton->setEnabled(true);
    ui->stayButton->setEnabled(true);
    ui->jumpButton->show();
    ui->sitButton->show();
    ui->stayButton->show();
    resetUIElements();
    show();
    currentCourseStage = 0;
    readStageData();
}

/**
 * @brief Sets the course window to display all widget elements necessary for sandbox mode.
 */
void CourseModeWindow::setupWindowAsSandboxMode(){
    ui->belayerWeightValueSpinbox->setEnabled(true);
    ui->belayerHeightValueSpinbox->setEnabled(true);
    ui->leaderWeightValueSpinbox->setEnabled(true);
    ui->leaderHeightValueSpinbox->setEnabled(true);
    ui->boltHeightValueSpinbox->setEnabled(true);
    ui->ropeLengthValueSpinbox->setEnabled(true);
    ui->runSimulationButton->setEnabled(true);
    ui->runSimulationButton->show();
    ui->jumpButton->setEnabled(false);
    ui->sitButton->setEnabled(false);
    ui->stayButton->setEnabled(false);
    ui->jumpButton->hide();
    ui->sitButton->hide();
    ui->stayButton->hide();
    resetUIElements();
    show();
}

/**
 * @brief Main menu private button slot
 */
void CourseModeWindow::on_mainMenuButton_clicked()
{
    hide();
    emit closeCourseModeWindowSignal();
}

/**
 * @brief Updates window widgets, paint objects, and alerts the simulator widget to update its simulation world.
 */
void CourseModeWindow::updateWindow(int currentClimberNewtons, int maxClimberNewtons, int currentBelayerNewtons, int maxBelayerNewtons){
    ui->belayerHeightValueSpinbox->blockSignals(true);
    ui->leaderHeightValueSpinbox->blockSignals(true);
    ui->belayerHeightValueSpinbox->setValue(simView->getBelayerPosition().y);
    ui->leaderHeightValueSpinbox->setValue(simView->getLeaderPosition().y);
    ui->currentLeaderNewtonsOutputLabel->setText(QString::number(currentClimberNewtons));
    ui->maxLeaderNewtonsOutputLabel->setText(QString::number(maxClimberNewtons));
    ui->currentBelayerNewtonsOutputLabel->setText(QString::number(currentBelayerNewtons));
    ui->maxBelayerNewtonsOutputLabel->setText(QString::number(maxBelayerNewtons));
    ui->belayerHeightValueSpinbox->blockSignals(false);
    ui->leaderHeightValueSpinbox->blockSignals(false);
}

void CourseModeWindow::showChoiceResults(){
    emit simulationStopSignal();

    QMessageBox choiceResults;

    if(playerChoice == courseStages.at(currentCourseStage).correctAnswer){
        choiceResults.setText("Correct. \n" + courseStages.at(currentCourseStage).answerExplanation);
        choiceResults.exec();
    }
    else {
        choiceResults.setText("Incorrect. \n"  + courseStages.at(currentCourseStage).answerExplanation);
        choiceResults.exec();
    }

    ui->jumpButton->setEnabled(true);
    ui->sitButton->setEnabled(true);
    ui->stayButton->setEnabled(true);
//    emit simulationStartSignal();
    if(currentCourseStage < courseStages.length()-1){
        currentCourseStage = currentCourseStage + 1;
        readStageData();
    }
    else{
        QMessageBox endScreen;
        endScreen.setText("You win. Woot");
        endScreen.exec();
        hide();
        emit closeCourseModeWindowSignal();
    }
}

/**
 * @brief UI helper method to improve the visuals of various UI elements.
 */
void CourseModeWindow::applyStylesheets(){
    ui->jumpButton->setStyleSheet(QString("QPushButton {background-color: rgb(225,225,225);"
                                          "border-style: outset;"
                                          "border-width: 2px;"
                                          "border-radius: 3px;"
                                          "border-color: rgb(50,50,50)}"
                                          "QPushButton:hover {background-color: rgb(245,245,245);"
                                          "border-color: rgb(75,75,75);"
                                          "}"
                                          "QPushButton:pressed {background-color: rgb(220,240,240);"
                                          "border-color: rgb(75,75,100);"
                                          "border-style: inset;"
                                          "}"));
    ui->sitButton->setStyleSheet(QString("QPushButton {background-color: rgb(225,225,225);"
                                          "border-style: outset;"
                                          "border-width: 2px;"
                                          "border-radius: 3px;"
                                          "border-color: rgb(50,50,50)}"
                                          "QPushButton:hover {background-color: rgb(245,245,245);"
                                          "border-color: rgb(75,75,75);"
                                          "}"
                                          "QPushButton:pressed {background-color: rgb(220,240,240);"
                                          "border-color: rgb(75,75,100);"
                                          "border-style: inset;"
                                          "}"));
    ui->stayButton->setStyleSheet(QString("QPushButton {background-color: rgb(225,225,225);"
                                          "border-style: outset;"
                                          "border-width: 2px;"
                                          "border-radius: 3px;"
                                          "border-color: rgb(50,50,50)}"
                                          "QPushButton:hover {background-color: rgb(245,245,245);"
                                          "border-color: rgb(75,75,75);"
                                          "}"
                                          "QPushButton:pressed {background-color: rgb(220,240,240);"
                                          "border-color: rgb(75,75,100);"
                                          "border-style: inset;"
                                          "}"));
    ui->mainMenuButton->setStyleSheet(QString("QPushButton {background-color: rgb(200, 200, 200);"
                                              "border-color: rgb(130, 130, 130);"
                                              "border-radius: 5px;"
                                              "border-width: 2px;}"
                                              "QPushButton:hover {background-color: rgb(220,230,240);"
                                              "}"
                                              "QPushButton:pressed {background-color: rgb(180,210,230);"
                                              "border-style: inset;"
                                              "}"));
}

void CourseModeWindow::readStageData(){
    ui->belayerHeightValueSpinbox->setValue(courseStages.at(currentCourseStage).belayerHeight.y);
    ui->belayerWeightValueSpinbox->setValue(courseStages.at(currentCourseStage).belayerWeight);
    ui->leaderHeightValueSpinbox->setValue(courseStages.at(currentCourseStage).climberHeight.y);
    ui->leaderWeightValueSpinbox->setValue(courseStages.at(currentCourseStage).climberWeight);
    ui->boltHeightValueSpinbox->setValue(courseStages.at(currentCourseStage).boltHeight.y);
    ui->ropeLengthValueSpinbox->setValue(calculatePulleyRopeLength());
    ui->maxLeaderNewtonsOutputLabel->setText("0");
    ui->currentLeaderNewtonsOutputLabel->setText("0");
    simView->maxClimberNewtons=0;
    ui->maxBelayerNewtonsOutputLabel->setText("0");
    ui->currentBelayerNewtonsOutputLabel->setText("0");
    simView->maxBelayerNewtons=0;

    QMessageBox choiceResults;
    choiceResults.setText(courseStages.at(currentCourseStage).stageDescription);
    choiceResults.exec();
}

void CourseModeWindow::on_jumpButton_clicked()
{
    ui->jumpButton->setEnabled(false);
    ui->sitButton->setEnabled(false);
    ui->stayButton->setEnabled(false);
    emit simulationStartSignal();
    playerChoice = level_t::Answer::jump;
    QTimer::singleShot(5000, Qt::PreciseTimer, this, &CourseModeWindow::showChoiceResults);
}


void CourseModeWindow::on_sitButton_clicked()
{
    ui->jumpButton->setEnabled(false);
    ui->sitButton->setEnabled(false);
    ui->stayButton->setEnabled(false);
    emit simulationStartSignal();
    playerChoice = level_t::Answer::sit;
    QTimer::singleShot(5000, Qt::PreciseTimer, this, &CourseModeWindow::showChoiceResults);
}


void CourseModeWindow::on_stayButton_clicked()
{
    ui->jumpButton->setEnabled(false);
    ui->sitButton->setEnabled(false);
    ui->stayButton->setEnabled(false);
    emit simulationStartSignal();
    playerChoice = level_t::Answer::stay;
    QTimer::singleShot(5000, Qt::PreciseTimer, this, &CourseModeWindow::showChoiceResults);
}

/**
 * @brief Slot for when the run button is clicked. Signals the simulation widget to start.
 */
void CourseModeWindow::on_runSimulationButton_clicked()
{
    switch(currSimState){
    case stopped:
        disableSpinboxes();
        emit simulationStartSignal();
        ui->runSimulationButton->setText("Stop");
        currSimState = running;
        break;
    case running:
        enableSpinboxes();
        emit simulationStopSignal();
        ui->runSimulationButton->setText("Run");
        currSimState = stopped;
        break;
    }
}

/**
 * @brief Slot for when the climber height spinbox is updated.
 * @param arg1
 */
void CourseModeWindow::on_leaderHeightValueSpinbox_valueChanged(double arg1)
{
    emit climberHeightUpdatedSignal(arg1);
    ui->ropeLengthValueSpinbox->setValue(calculatePulleyRopeLength());
}

/**
 * @brief Slot for when the belayer height spinbox is updated.
 * @param arg1
 */
void CourseModeWindow::on_belayerHeightValueSpinbox_valueChanged(double arg1)
{
    emit belayerHeightUpdatedSignal(arg1);
    ui->ropeLengthValueSpinbox->setValue(calculatePulleyRopeLength());
}

/**
 * @brief Slot for when the climber weight spinbox is updated.
 * @param arg1
 */
void CourseModeWindow::on_leaderWeightValueSpinbox_valueChanged(double arg1)
{
    emit climberWeightUpdatedSignal(arg1);
}

/**
 * @brief Slot for when the belayer weight spinbox is updated.
 * @param arg1
 */
void CourseModeWindow::on_belayerWeightValueSpinbox_valueChanged(double arg1)
{
    emit belayerWeightUpdatedSignal(arg1);
}

/**
 * @brief CourseModeWindow::disableSpinboxes
 */
void CourseModeWindow::disableSpinboxes(){
    ui->belayerWeightValueSpinbox->setEnabled(false);
    ui->belayerHeightValueSpinbox->setEnabled(false);
    ui->leaderWeightValueSpinbox->setEnabled(false);
    ui->leaderHeightValueSpinbox->setEnabled(false);
    ui->boltHeightValueSpinbox->setEnabled(false);
    ui->ropeLengthValueSpinbox->setEnabled(false);
}

/**
 * @brief CourseModeWindow::enableSpinboxes
 */
void CourseModeWindow::enableSpinboxes(){
    ui->belayerWeightValueSpinbox->setEnabled(true);
    ui->belayerHeightValueSpinbox->setEnabled(true);
    ui->leaderWeightValueSpinbox->setEnabled(true);
    ui->leaderHeightValueSpinbox->setEnabled(true);
    ui->boltHeightValueSpinbox->setEnabled(true);
    ui->ropeLengthValueSpinbox->setEnabled(true);
}

/**
 * @brief CourseModeWindow::resetUIElements
 */
void CourseModeWindow::resetUIElements(){
    emit resetSimulationDataSignal();
    ui->belayerHeightValueSpinbox->setValue(simView->belayerHeight);
    ui->leaderHeightValueSpinbox->setValue(simView->climberHeight);
    ui->leaderWeightValueSpinbox->setValue(simView->climberWeight);
    ui->belayerWeightValueSpinbox->setValue(simView->belayerWeight);
    ui->currentLeaderNewtonsOutputLabel->setText(QString::number(0));
    ui->maxLeaderNewtonsOutputLabel->setText(QString::number(0));
    ui->currentBelayerNewtonsOutputLabel->setText(QString::number(0));
    ui->maxBelayerNewtonsOutputLabel->setText(QString::number(0));
    // ADDED
    ui->boltHeightValueSpinbox->blockSignals(true);
    ui->ropeLengthValueSpinbox->blockSignals(true);
    ui->boltHeightValueSpinbox->setValue(simView->boltHeight);
    ui->ropeLengthValueSpinbox->setValue(simView->ropeLength);
    ui->boltHeightValueSpinbox->blockSignals(false);
    ui->ropeLengthValueSpinbox->blockSignals(false);
    if(currSimState==running){
        enableSpinboxes();
        emit simulationStopSignal();
        ui->runSimulationButton->setText("Run");
        currSimState = stopped;
    }
}

/**
 * @brief CourseModeWindow::on_boltHeightValueSpinbox_valueChanged
 * @param arg1
 */
void CourseModeWindow::on_boltHeightValueSpinbox_valueChanged(double arg1){
    emit pulleyHeightUpdatedSignal(arg1);
}

/**
 * @brief CourseModeWindow::on_ropeLengthValueSpinbox_valueChanged
 * @param arg1
 */
void CourseModeWindow::on_ropeLengthValueSpinbox_valueChanged(double arg1)
{
    emit pulleyRopeLengthUpdatedSignal(arg1);
}

/**
 * @brief CourseModeWindow::calculatePulleyRopeLength
 */
float CourseModeWindow::calculatePulleyRopeLength(){
    float heightA = ui->belayerHeightValueSpinbox->value();
    float heightB = ui->leaderHeightValueSpinbox->value();
    float boltHeight = ui->boltHeightValueSpinbox->value();
    float lengthA = abs(boltHeight - heightA);
    float lenghtB = abs(boltHeight - heightB);
    return lengthA + lenghtB;
}
