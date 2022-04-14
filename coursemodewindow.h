#ifndef COURSEMODEWINDOW_H
#define COURSEMODEWINDOW_H

#include <QWidget>

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

signals:
    void closeCourseModeWindowSignal();

private slots:
    void on_mainMenuButton_clicked();

    void on_jumpButton_clicked();

    void on_sitButton_clicked();

    void on_stayButton_clicked();

private:
    Ui::CourseModeWindow *ui;
    void applyStylesheets();
    void showChoiceResults();
};

#endif // COURSEMODEWINDOW_H
