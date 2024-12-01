#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include "detailfilmwidget.h"
#include <QDialog>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QAudioOutput>
#include <QBuffer>



namespace Ui {
class DetailFIlmWidget;
}

class VideoPlayer : public QDialog {

public:
    VideoPlayer(const QByteArray& movie, QWidget *parent = nullptr);
    ~VideoPlayer();

private slots:
    void on_pushButton_play_pause_clicked();
    void on_pushButton_forward_clicked();
    void on_pushButton_Volume_clicked();
    void on_pushButton_backward_clicked();
    void on_horizontalSlider_Duration_valueChanged(int value);
    void on_horizontalSlider_Volume_valueChanged(int value);
    void durationChanged(qint64 Duration);
    void positionChanged(qint64 Duration);

private:
    Ui::DetailFIlmWidget *ui;
    QMediaPlayer *player;
    QVideoWidget *video;
    qint64 mDuration;
    QAudioOutput *volume;
    QBuffer *mediaBuffer;
    bool IS_Pause = true;
    bool IS_Muted = false;
    void updateDuration(qint64 Duration);
};

#endif // VIDEOPLAYER_H
