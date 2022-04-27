#ifndef COURSEMODEWINDOW_H
#define COURSEMODEWINDOW_H

#include <QWidget>
#include "simulationviewwidget.h"

struct level_t{
    b2Vec2 climberHeight;
    float climberWeight;
    b2Vec2 belayerHeight;
    float belayerWeight;
    b2Vec2 boltHeight;
    enum Answer{sit, jump, stay};
    Answer correctAnswer;
    QString answerExplanation;
    QString stageDescription;
};

namespace Ui {
class CourseModeWindow;
}

class CourseModeWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CourseModeWindow(QWidget *parent = nullptr);
    ~CourseModeWindow();
    void setupWindowAsCourseMode();
    void setupWindowAsSandboxMode();

public slots:
    void updateWindow(int currentClimberNewtons, int maxClimberNewtons, int currentBelayerNewtons, int maxBelayerNewtons);

signals:
    void closeCourseModeWindowSignal();
    void simulationStartSignal();
    void simulationStopSignal();
    void climberHeightUpdatedSignal(float);
    void climberWeightUpdatedSignal(float);
    void belayerHeightUpdatedSignal(float);
    void belayerWeightUpdatedSignal(float);
    void pulleyHeightUpdatedSignal(float);
    void pulleyRopeLengthUpdatedSignal(float);
    void resetSimulationDataSignal();

private slots:
    void on_mainMenuButton_clicked();
    void on_jumpButton_clicked();
    void on_sitButton_clicked();
    void on_stayButton_clicked();
    void on_runSimulationButton_clicked();
    void on_leaderHeightValueSpinbox_valueChanged(double arg1);
    void on_belayerHeightValueSpinbox_valueChanged(double arg1);
    void on_leaderWeightValueSpinbox_valueChanged(double arg1);
    void on_belayerWeightValueSpinbox_valueChanged(double arg1);
    void on_boltHeightValueSpinbox_valueChanged(double arg1);
    void on_ropeLengthValueSpinbox_valueChanged(double arg1);

private:
    Ui::CourseModeWindow *ui;
    SimulationViewWidget* simView;
    enum SimState{stopped, running};
    SimState currSimState = stopped;
    void applyStylesheets();
    void showChoiceResults();
    void disableSpinboxes();
    void enableSpinboxes();
    void resetUIElements();
    float calculatePulleyRopeLength();
    QVector<level_t> courseStages;
    int currentCourseStage;
    void readStageData();
    level_t::Answer playerChoice;
};

#endif // COURSEMODEWINDOW_H
