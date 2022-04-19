#ifndef COURSEMODEWINDOW_H
#define COURSEMODEWINDOW_H

#include <QWidget>
#include "simulationviewwidget.h"

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
    void updateWindow(int belayerNewtons, int climberNewtons);
    void simulationCompleteSlot();

signals:
    void closeCourseModeWindowSignal();
    void startSimulationTimerSignal();
    void climberHeightUpdatedSignal(float);
    void climberWeightUpdatedSignal(float);
    void belayerHeightUpdatedSignal(float);
    void belayerWeightUpdatedSignal(float);

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

private:
    Ui::CourseModeWindow *ui;
    void applyStylesheets();
    void showChoiceResults();
    void disableSpinboxes();
    void enableSpinboxes();
    SimulationViewWidget* simView;
};

#endif // COURSEMODEWINDOW_H
