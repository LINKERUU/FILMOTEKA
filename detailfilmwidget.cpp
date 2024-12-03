#include "detailfilmwidget.h"
#include "ui_detailfilmwidget.h"


DetailFIlmWidget::DetailFIlmWidget(DatabaseManager& dbManager,const QString& title, int year, const QString& genre, double rating,
                                   const QByteArray& poster, const QString& description, int duration,
                                   const QString& director, const QByteArray& movie, QWidget *parent)
    :m_dbManager(dbManager), FilmDetail(title, year, genre, rating, poster, description, duration, director, movie),
    QDialog(parent), ui(new Ui::DetailFIlmWidget),
    player(new QMediaPlayer(this)),
    volume(new QAudioOutput(this)),
    mediaBuffer(new QBuffer(this)),
    video(new QVideoWidget(this)),
    IS_Pause(false), IS_Muted(false)
{
    ui->setupUi(this);

    ui->horizontalSlider_Duration->setRange(0,player->duration()/1000);

    ui->title_label->setText(title);
    ui->year_label->setText(QString::number(year));
    ui->year_label->adjustSize();
    ui->genre_label->setText(genre);
    ui->genre_label->adjustSize();

    ui->back_button->setIcon(QIcon(":/icons/back_arrow.png"));
    ui->back_button->setVisible(false);

    // Load and display poster
    QPixmap pixmap;
    pixmap.loadFromData(poster);
    ui->poster_label->setPixmap(pixmap);
    ui->description_label->setText(description);
    ui->duration_label->setText(QString("%1 мин.").arg(duration));
    ui->director_label->setText(director);
    ui->director_label->adjustSize();
    ui->rating_label->setText(QString::number(rating));
    ui->rating_label->adjustSize();
    ui->groupBox_4->adjustSize();

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
    video->setGeometry(0, 0, ui->groupBox_Video->width(), ui->groupBox_Video->height());
    player->setVideoOutput(video);
    player->setAudioOutput(volume);

    mediaBuffer->setData(movie);
    mediaBuffer->open(QIODevice::ReadOnly);
    player->setSourceDevice(mediaBuffer);

    ui->scrollAreaWidgetContents->setMinimumSize(1000, 2000);
    video->setVisible(true);


    connect(player,&QMediaPlayer::durationChanged,this,&DetailFIlmWidget::durationChanged);
    connect(player,&QMediaPlayer::positionChanged,this,&DetailFIlmWidget::positionChanged);
}

DetailFIlmWidget::~DetailFIlmWidget()
{
    player->stop(); // Остановка воспроизведения
    delete ui;
}

void DetailFIlmWidget::closeEvent(QCloseEvent *event)
{
    if (player->playbackState() == QMediaPlayer::PlayingState) {
        player->stop();
    }
    event->accept();
}

void DetailFIlmWidget::on_pushButton_Volume_clicked()
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

void DetailFIlmWidget::on_horizontalSlider_Duration_valueChanged(int value)
{
    player->setPosition(value * 1000);
}

void DetailFIlmWidget::on_pushButton_forward_clicked()
{
    int newValue = ui->horizontalSlider_Duration->value() + 10;
    ui->horizontalSlider_Duration->setValue(newValue);
    player->setPosition(newValue * 1000);
}

void DetailFIlmWidget::on_pushButton_play_pause_clicked()
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

void DetailFIlmWidget::on_pushButton_backward_clicked()
{
    int newValue = ui->horizontalSlider_Duration->value() - 10;
    ui->horizontalSlider_Duration->setValue(newValue);
    player->setPosition(newValue * 1000);
}

void DetailFIlmWidget::on_horizontalSlider_Volume_valueChanged(int value)
{
    if(ui->horizontalSlider_Volume->value()<50)
        ui->pushButton_Volume->setIcon(QIcon(":/icons/low_sound.png"));
    else
        ui->pushButton_Volume->setIcon(QIcon(":/icons/high_sound.png"));

    volume->setVolume(ui->horizontalSlider_Volume->value() / 100.0);
}

void DetailFIlmWidget::updateDuration(qint64 Duration){

    QTime CurrentTime((Duration/3600)%60 , (Duration/60)%60,Duration%60);
    QTime TotalTime((mDuration/3600)%60 , (mDuration/60)%60,mDuration%60);
    QString Format = "h:mm:ss";

    ui->label_current_time->setText(CurrentTime.toString(Format));
    ui->label_end_time->setText(TotalTime.toString(Format));
}


void DetailFIlmWidget::durationChanged(qint64 Duration){
    mDuration = Duration/1000;
    ui->horizontalSlider_Duration->setMaximum(mDuration);
}

void DetailFIlmWidget::positionChanged(qint64 Duration){
    if(!ui->horizontalSlider_Duration->isSliderDown()){
        ui->horizontalSlider_Duration->setValue(Duration/1000);
    }
    updateDuration(Duration/1000);
}

// void DetailFIlmWidget::on_back_button_clicked()
// {
//     ListMovie* catalogmovie = new ListMovie(m_dbManager);
//     catalogmovie->show();
//     this->close();
// }

