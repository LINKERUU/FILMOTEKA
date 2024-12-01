#include "videoplayer.h"
#include "ui_detailfilmwidget.h"
#include <QStyle>
#include <QTime>

VideoPlayer::VideoPlayer(const QByteArray& movie,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DetailFIlmWidget)
    , player(new QMediaPlayer(this)) // Инициализация player
    , volume(new QAudioOutput(this)) // Инициализация audio output
    , video(new QVideoWidget(this)) // Инициализация video widget
    , IS_Pause(true), IS_Muted(false) // Инициализация состояния
{
    ui->setupUi(this);
    ui->horizontalSlider_Duration->setRange(0,player->duration()/1000);


    // Set up video player UI
    ui->pushButton_play_pause->setIcon(QIcon(":/icons/play.png"));
    ui->pushButton_backward->setIcon(QIcon(":/icons/seek_back.png"));
    ui->pushButton_forward->setIcon(QIcon(":/icons/seek_forward.png"));
    ui->pushButton_Volume->setIcon(QIcon(":/icons/low_sound.png"));
    // Volume slider setup
    ui->horizontalSlider_Volume->setMinimum(0);
    ui->horizontalSlider_Volume->setMaximum(100);
    ui->horizontalSlider_Volume->setValue(30);
    volume->setVolume(ui->horizontalSlider_Volume->value() / 100.0);

    // Setting up video widget
    video->setParent(ui->groupBox_Video);
    video->setGeometry(0, 0, ui->groupBox_3->width() - 0, ui->groupBox_3->height());
    player->setVideoOutput(video);
    player->setAudioOutput(volume);

    mediaBuffer->setData(movie);
    mediaBuffer->open(QIODevice::ReadOnly);
    player->setSourceDevice(mediaBuffer);

    video->setVisible(true);

    connect(player,&QMediaPlayer::durationChanged,this,&VideoPlayer::durationChanged);
    connect(player,&QMediaPlayer::positionChanged,this,&VideoPlayer::positionChanged);

}

VideoPlayer::~VideoPlayer()
{
    delete ui;
}

void VideoPlayer::on_pushButton_Volume_clicked()
{
    IS_Muted = !IS_Muted;
    ui->pushButton_Volume->setIcon(IS_Muted ? QIcon(":/icons/muted.png") : QIcon(":/icons/low_sound"));
    ui->horizontalSlider_Volume->setValue(IS_Muted ? 0 : 30);
    volume->setMuted(IS_Muted);
    if(!IS_Muted){
        volume->setVolume(ui->horizontalSlider_Volume->value() / 100.0);
        ui->pushButton_Volume->setIcon(QIcon(":/icons/low_sound"));
    }
    else{
        ui->pushButton_Volume->setIcon(QIcon(":/icons/muted.png"));
    }
}

void VideoPlayer::on_horizontalSlider_Duration_valueChanged(int value)
{
    player->setPosition(value * 1000);
}

void VideoPlayer::on_pushButton_forward_clicked()
{
    int newValue = ui->horizontalSlider_Duration->value() + 10;
    ui->horizontalSlider_Duration->setValue(newValue);
    player->setPosition(newValue * 1000);
}

void VideoPlayer::on_pushButton_play_pause_clicked()
{
    if (!IS_Pause) {
        player->play();
        ui->pushButton_play_pause->setIcon(QIcon(":/icons/pause.png"));
    }
    else {
        player->pause();
        ui->pushButton_play_pause->setIcon(QIcon(":/icons/play.png"));
    }

    IS_Pause=!IS_Pause;

}

void VideoPlayer::on_pushButton_backward_clicked()
{
    int newValue = ui->horizontalSlider_Duration->value() - 10;
    ui->horizontalSlider_Duration->setValue(newValue);
    player->setPosition(newValue * 1000);
}

void VideoPlayer::on_horizontalSlider_Volume_valueChanged(int value)
{
    if(ui->horizontalSlider_Volume->value()<50)
        ui->pushButton_Volume->setIcon(QIcon(":/icons/low_sound.png"));
    else
        ui->pushButton_Volume->setIcon(QIcon(":/icons/high_sound.png"));

    volume->setVolume(ui->horizontalSlider_Volume->value() / 100.0);
}

void VideoPlayer::updateDuration(qint64 Duration){

    QTime CurrentTime((Duration/3600)%60 , (Duration/60)%60,Duration%60);
    QTime TotalTime((mDuration/3600)%60 , (mDuration/60)%60,mDuration%60);
    QString Format = "h:mm:ss";

    ui->label_current_time->setText(CurrentTime.toString(Format));
    ui->label_end_time->setText(TotalTime.toString(Format));
}


void VideoPlayer::durationChanged(qint64 Duration){
    mDuration = Duration/1000;
    ui->horizontalSlider_Duration->setMaximum(mDuration);
}

void VideoPlayer::positionChanged(qint64 Duration){
    if(!ui->horizontalSlider_Duration->isSliderDown()){
        ui->horizontalSlider_Duration->setValue(Duration/1000);
    }
    updateDuration(Duration/1000);
}
