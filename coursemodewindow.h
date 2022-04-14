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
    void courseModeWindowClosedSignal();

private slots:
    void on_mainMenuButton_clicked();

private:
    Ui::CourseModeWindow *ui;
    void applyStylesheets();
};

#endif // COURSEMODEWINDOW_H
