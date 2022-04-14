#include <QMessageBox>
#include "coursemodewindow.h"
#include "ui_coursemodewindow.h"

CourseModeWindow::CourseModeWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CourseModeWindow)
{
    ui->setupUi(this);

    applyStylesheets();
}

CourseModeWindow::~CourseModeWindow()
{
    delete ui;
}

/**
 * @brief Sends a signal to reset all appropriate model data so that the course window starts fresh.
 */
void CourseModeWindow::setupWindowAsCourseMode(){
    ui->belayerWeightValueSpinbox->setReadOnly(true);
    ui->leaderWeightValueSpinbox->setReadOnly(true);
    ui->leaderHeightValueSpinbox->setReadOnly(true);
    show();
}

/**
 * @brief CourseModeWindow::on_mainMenuButton_clicked
 */
void CourseModeWindow::on_mainMenuButton_clicked()
{
    ui->belayerWeightValueSpinbox->setReadOnly(false);
    ui->leaderWeightValueSpinbox->setReadOnly(false);
    hide();
    emit closeCourseModeWindowSignal();
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
    ui->mainMenuButton->setStyleSheet(QString("QPushButton {background-color: rgb(255, 255, 255);"
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
}


void CourseModeWindow::on_sitButton_clicked()
{
    showChoiceResults();
}


void CourseModeWindow::on_stayButton_clicked()
{
    showChoiceResults();
}

