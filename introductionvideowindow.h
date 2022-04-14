#ifndef INTRODUCTIONVIDEOWINDOW_H
#define INTRODUCTIONVIDEOWINDOW_H

#include <QWidget>
#include <QStyle>
#include <QFileInfo>
#include <QtMultimedia>
#include <QMediaPlayer>
#include <QVideoWidget>

namespace Ui {
class IntroductionVideoWindow;
}

class IntroductionVideoWindow : public QWidget
{
    Q_OBJECT

public:
    explicit IntroductionVideoWindow(QWidget *parent = nullptr);
    ~IntroductionVideoWindow();
    void openIntroVideoWindow();

public slots:
    void sliderChange();

signals:
    void closeIntroWindowSignal();
    void startVideo();
    void updateVolume(float volume);

private slots:
    void on_exitButton_clicked();

private:
    Ui::IntroductionVideoWindow *ui;
    void updateFrameSlider();
    QMediaPlayer* video;
    QAudioOutput* videoAudio;
    QVideoWidget* outputWidget;

    void setupVideo();
    float fromSliderToAudio();
};

#endif // INTRODUCTIONVIDEOWINDOW_H
