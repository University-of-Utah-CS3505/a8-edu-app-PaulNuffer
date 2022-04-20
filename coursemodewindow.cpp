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
    ui->belayerWeightValueSpinbox->setReadOnly(true);
    ui->belayerHeightValueSpinbox->setReadOnly(true);
    ui->leaderWeightValueSpinbox->setReadOnly(true);
    ui->leaderHeightValueSpinbox->setReadOnly(true);
    ui->boltHeightValueSpinbox->setReadOnly(true);
    ui->ropeLengthValueSpinbox->setReadOnly(true);
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
}

/**
 * @brief Sets the course window to display all widget elements necessary for sandbox mode.
 */
void CourseModeWindow::setupWindowAsSandboxMode(){
    ui->belayerWeightValueSpinbox->setReadOnly(false);
    ui->belayerHeightValueSpinbox->setReadOnly(false);
    ui->leaderWeightValueSpinbox->setReadOnly(false);
    ui->leaderHeightValueSpinbox->setReadOnly(false);
    ui->boltHeightValueSpinbox->setReadOnly(false);
    ui->ropeLengthValueSpinbox->setReadOnly(false);
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
    ui->belayerHeightValueSpinbox->setValue(simView->getBelayerPosition().y);
    ui->leaderHeightValueSpinbox->setValue(simView->getLeaderPosition().y);
    ui->currentLeaderNewtonsOutputLabel->setText(QString::number(currentClimberNewtons));
    ui->maxLeaderNewtonsOutputLabel->setText(QString::number(maxClimberNewtons));
    ui->currentBelayerNewtonsOutputLabel->setText(QString::number(currentBelayerNewtons));
    ui->maxBelayerNewtonsOutputLabel->setText(QString::number(maxBelayerNewtons));
}

/**
 * @brief CourseModeWindow::showChoiceResults
 */
void CourseModeWindow::showChoiceResults(){
    QMessageBox choiceResults;
    choiceResults.setText("In this situation, you should have:");
    choiceResults.exec();
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

void CourseModeWindow::on_jumpButton_clicked()
{
    showChoiceResults();
    emit simulationStartSignal();
}


void CourseModeWindow::on_sitButton_clicked()
{
    showChoiceResults();
    emit simulationStartSignal();
}


void CourseModeWindow::on_stayButton_clicked()
{
    showChoiceResults();
    emit simulationStartSignal();
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
}

/**
 * @brief Slot for when the belayer height spinbox is updated.
 * @param arg1
 */
void CourseModeWindow::on_belayerHeightValueSpinbox_valueChanged(double arg1)
{
    emit belayerHeightUpdatedSignal(arg1);
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
    ui->belayerWeightValueSpinbox->setReadOnly(true);
    ui->belayerHeightValueSpinbox->setReadOnly(true);
    ui->leaderWeightValueSpinbox->setReadOnly(true);
    ui->leaderHeightValueSpinbox->setReadOnly(true);
    ui->boltHeightValueSpinbox->setReadOnly(true);
    ui->ropeLengthValueSpinbox->setReadOnly(true);
}

/**
 * @brief CourseModeWindow::enableSpinboxes
 */
void CourseModeWindow::enableSpinboxes(){
    ui->belayerWeightValueSpinbox->setReadOnly(false);
    ui->belayerHeightValueSpinbox->setReadOnly(false);
    ui->leaderWeightValueSpinbox->setReadOnly(false);
    ui->leaderHeightValueSpinbox->setReadOnly(false);
    ui->boltHeightValueSpinbox->setReadOnly(false);
    ui->ropeLengthValueSpinbox->setReadOnly(false);
}

/**
 * @brief CourseModeWindow::resetUIElements
 */
void CourseModeWindow::resetUIElements(){
    emit resetSimulationDataSignal();
    ui->belayerHeightValueSpinbox->setValue(0);
    ui->leaderHeightValueSpinbox->setValue(35);
    ui->leaderWeightValueSpinbox->setValue(72);
    ui->belayerWeightValueSpinbox->setValue(70);
    ui->currentLeaderNewtonsOutputLabel->setText(QString::number(0));
    ui->maxLeaderNewtonsOutputLabel->setText(QString::number(0));
    ui->currentBelayerNewtonsOutputLabel->setText(QString::number(0));
    ui->maxBelayerNewtonsOutputLabel->setText(QString::number(0));
    ui->boltHeightValueSpinbox->setValue(30);
    ui->ropeLengthValueSpinbox->setValue(35);
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

