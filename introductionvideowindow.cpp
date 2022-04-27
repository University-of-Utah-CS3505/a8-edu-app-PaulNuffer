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

    ui->videoLabel->setStyleSheet("QLabel { background-color : white; color : black }");
    ui->videoLabel->setAlignment(Qt::AlignLeft);
    ui->videoLabel->raise();
    ui->videoLabel->show();

    ui->nextButton->setText("Start");
    videoRunning = false;
    videoIndex = 0;

    setupVideo();

    introVideos.push_back((QUrl::fromLocalFile("../a8-edu-app-PaulNuffer/Resources/FallCompilation.mp4")));
    introVideos.push_back((QUrl::fromLocalFile("../a8-edu-app-PaulNuffer/Resources/GivingAndTakingSlack.mp4")));
    introVideos.push_back((QUrl::fromLocalFile("../a8-edu-app-PaulNuffer/Resources/StayBelay.mp4")));
    introVideos.push_back((QUrl::fromLocalFile("../a8-edu-app-PaulNuffer/Resources/JumpBelay.mp4")));
    introVideos.push_back((QUrl::fromLocalFile("../a8-edu-app-PaulNuffer/Resources/SitBelay.mp4")));
    introVideos.push_back((QUrl::fromLocalFile("../a8-edu-app-PaulNuffer/Resources/StandardWhip.mp4")));
    introVideos.push_back((QUrl::fromLocalFile("../a8-edu-app-PaulNuffer/Resources/ShortSoftFallWithJump.mp4")));
    introVideos.push_back((QUrl::fromLocalFile("../a8-edu-app-PaulNuffer/Resources/ShortHardCatchWithBelay.mp4")));
    introVideos.push_back((QUrl::fromLocalFile("../a8-edu-app-PaulNuffer/Resources/SideBySideCentered.mp4")));
    introVideos.push_back((QUrl::fromLocalFile("../a8-edu-app-PaulNuffer/Resources/LowSoftCatch.mp4")));
    introVideos.push_back((QUrl::fromLocalFile("../a8-edu-app-PaulNuffer/Resources/LowHardCatch.mp4")));
    introVideos.push_back((QUrl::fromLocalFile("../a8-edu-app-PaulNuffer/Resources/VictoryWhip.mp4")));


    dialog.push_back(QString("Hi and welcome to the whipper simulator. This is an app to help climbers who are new \n"
                             "to belaying learn how to do so safely. Belaying is the act of 'catching' climbers, and \n"
                             "belayers use devices that are designed to make managing the rope easy. Belayers not only \n"
                             "catch falling climbers, but also make sure the climber has enough rope to continue \n"
                             "climbing upwards without being in danger. In this application, we'll explore the actions \n"
                             "belayers can take to keep a falling climber safe."));
    dialog.push_back(QString("First, let's define slack. Slack: The amount of excess rope between the climber and \n"
                             "belayer. Typically a belayer pays out or takes in slack by moving the rope through a \n"
                             "belay device. Slack is essential in allowing climbers to continue moving upwards, but \n"
                             "right now we'll consider it as a tool for the belayer to affect falls. Here, the you can \n"
                             "see the belayer feeding out and taking in slack as the climber moves."));
    dialog.push_back(QString("When the climber falls, the belay device locks up and stops the rope, which means no \n"
                             "more slack is given out. So to change the amount of slack in the rope quickly (as the \n"
                             "climber is falling) belayers can move their whole bodies by jumping or sitting down. \n"
                             "Here, the belayer does neither and is just pulled by the climber falling"));
    dialog.push_back(QString("Here the belayer jumps as the cimber falls, which gives out LOTS of slack!"));
    dialog.push_back(QString("And here the belayer sits down as the cimber falls, which takes in all of the slack."));
    dialog.push_back(QString("But why would a belayer want to change the amount of slack in the rope as the climber \n"
                             "falls? Well, let's take a look at a fall, or 'whipper'. Here we have a climber about two \n"
                             "meters above their last piece of protection (in this case, a bolt through which the rope \n"
                             "is clipped). As they fall, they will fall those two meters to the bolt, and then those \n"
                             "two meters again due to the slack in the rope. At this point, the duration of the fall \n"
                             "is affected by the dynamic nature, or stretchiness, of the rope, and what the belayer \n"
                             "does."));
    dialog.push_back(QString("A 'soft' catch is typically the goal of the belayer. Just like airbags in a car crash, \n"
                             "the goal is to INCREASE the duration of the fall and give the climber MORE TIME to slow \n"
                             "down. This decreases the force experienced by the climber, and is safer. It also gives \n"
                             "the climber more time to orient themselves in the air and protect themselves as they \n"
                             "swing into the wall. A belayer can 'soften' a fall by feeding out more slack, or jumping! \n"
                             "This is particularly helpful when climbers are very close to their last bolt, because there \n"
                             "is very little slack already in the rope, so the climber will experience a lot of force. \n"
                             "Another case  where soft catches are essential is when the belayer is substantially heavier \n"
                             "than the lead climber. Typically the climber's weight pulls the belayer up, naturally \n"
                             "creating a softer catch, but in this case, the belayer would simply stop the fall abruptly \n"
                             "and possibly the climber. By jumping or moving toward the wall to create slack, this can \n"
                             "be avoided and give the lead climber a safe fall."));
    dialog.push_back(QString("A 'hard' catch is when the belayer does the opposite, and takes in slack or sits down to \n"
                             "DECREASE the duration of the fall. This can be more dangerous for the climber, as it whips \n"
                             "them back into the wall with more force. However, sometimes this is necessary! Imagine \n"
                             "the climber is very close to the ground, or that the belayer is much lighter than the \n"
                             "climber. Then, it is much better to give the climber a hard catch than it is to let them \n"
                             "hit the ground, or 'deck'."));
    dialog.push_back(QString("If the climber is far from their bolt horizontally, a 'soft' catch will reduce the pendulum \n"
                             "action, creating a soft fall with less swing. A hard catch in this situation swings the \n"
                             "climber into the wallwith a lot more velocity."));
    dialog.push_back(QString("When the climber is very low to the ground, a 'soft' catch can be dangerous. Not only \n"
                             "could the climber fall all the way to the ground, but it is also possible to land on \n"
                             "the belayer. This is good example of when NOT to give a soft catch."));
    dialog.push_back(QString("Here the belayer does the right thing by giving a much harder catch in the same scenario. \n"
                             "The climber does swing into the wall quickly, but this is better than the alternatives."));
    dialog.push_back(QString("You're ready for our course, go try it out!"));


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
    on_nextButton_clicked();
}

/**
 * @brief Helper method to setup the video player for the introduction.
 */
void IntroductionVideoWindow::setupVideo(){
    video = new QMediaPlayer(this);
    videoAudio = new QAudioOutput(this);
    outputWidget = new QVideoWidget(this);

    outputWidget->setGeometry(0, 0, 500, 300);
    outputWidget->resize(500, 300);

    video->setAudioOutput(videoAudio);
    video->setVideoOutput(outputWidget);
}
void IntroductionVideoWindow::setupVideoLink(QUrl introVideo) {
    video->setSource(introVideo);
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
    videoIndex = 0;
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

void IntroductionVideoWindow::on_nextButton_clicked()
{
    ui->nextButton->setText("Next");

    if (videoIndex == (int)introVideos.size() - 1) {
        ui->nextButton->setText("Restart Course");
        videoIndex = 0;
    }

    ui->videoLabel->setText(QString("%1 ").arg(dialog.at(videoIndex)));

    outputWidget->raise();
    ui->videoFrame->lower();
    setupVideoLink(introVideos.at(videoIndex));
    emit startVideo();
    videoRunning = true;
    videoIndex++;
}

void IntroductionVideoWindow::on_replayButton_clicked()
{
    if(!videoRunning){
        outputWidget->raise();
        ui->videoFrame->lower();
        setupVideoLink(introVideos.at(videoIndex));
        emit startVideo();
        videoRunning = true;
    }
    else{
        emit stopVideo();
        emit startVideo();
        videoRunning = true;
    }
}

