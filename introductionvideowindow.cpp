#include "introductionvideowindow.h"
#include "ui_introductionvideowindow.h"

IntroductionVideoWindow::IntroductionVideoWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IntroductionVideoWindow)
{
    ui->setupUi(this);
    ui->videoFrame->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->videoFrame->setBackgroundRole(QPalette::Dark);
    ui->videoFrame->setAutoFillBackground(true);

    ui->volumeIconDisplay->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    ui->exitButton->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));

    setupVideo();
    connect(this,
            &IntroductionVideoWindow::startVideo,
            video,
            &QMediaPlayer::play);
    connect(this,
            &IntroductionVideoWindow::stopVideo,
            video,
            &QMediaPlayer::stop);
    connect(ui->volumeSlider,
            &QSlider::valueChanged,
            this,
            &IntroductionVideoWindow::sliderChange);
    connect(this,
            &IntroductionVideoWindow::updateVolume,
            videoAudio,
            &QAudioOutput::setVolume);
}

/**
 * @brief Helper method for starting up the video from the beginning, as well as handling any other
 * ui changes
 */
void IntroductionVideoWindow::openIntroVideoWindow(){
    ui->volumeSlider->setValue(50);
    show();
    emit startVideo();
}

/**
 * @brief Helper method to setup the video player for the introduction.
 */
void IntroductionVideoWindow::setupVideo(){
    video = new QMediaPlayer(this);
    videoAudio = new QAudioOutput(this);
    outputWidget = new QVideoWidget(this);

    outputWidget->setGeometry(0, 0, 500, 500);
    outputWidget->resize(500, 500);
    video->setSource((QUrl::fromLocalFile("C:/Users/Owner/Desktop/test2.mp4")));

    video->setAudioOutput(videoAudio);
    video->setVideoOutput(outputWidget);
}

IntroductionVideoWindow::~IntroductionVideoWindow()
{
    delete ui;
}

/**
 * @brief Performs all necessary operations before hiding the introduction video window.
 */
void IntroductionVideoWindow::on_exitButton_clicked()
{
    emit stopVideo();
    hide();
    emit closeIntroWindowSignal();
}

/**
 * @brief Emits the converted slider value.
 */
void IntroductionVideoWindow::sliderChange(){
    emit updateVolume(fromSliderToAudio());
}

/**
 * @brief Converts the value from the volume slider to the appropriate scaling for QAudio.
 * @return The float value of the volume scaled for lineaer volume increase.
 */
float IntroductionVideoWindow::fromSliderToAudio(){
    qreal audioObjectVolume = QAudio::convertVolume(ui->volumeSlider->value() / qreal(100),
                                                    QAudio::LogarithmicVolumeScale,
                                                    QAudio::LinearVolumeScale);
    return audioObjectVolume;
}
