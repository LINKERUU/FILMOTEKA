#ifndef DETAILFILMWIDGET_H
#define DETAILFILMWIDGET_H

#include <QDialog>
#include "filmdetail.h"
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QAudioOutput>
#include <QBuffer>
#include <QString>
#include <QPixmap>
#include <QCloseEvent>
#include <QTime>
#include "moviewidget.h"
#include <QScrollBar>
#include <QCloseEvent>


namespace Ui {
class DetailFIlmWidget;
}

class DetailFIlmWidget : public QDialog , public FilmDetail
{
    Q_OBJECT

public:
    DetailFIlmWidget(DatabaseManager& dbManager,const QString& title, int year, const QString& genre, double rating, const QByteArray& poster,
                     const QString& description, int duraction, const QString& director, const QByteArray& movie,QWidget *parent = nullptr);
    ~DetailFIlmWidget();

private slots:
    void closeparentEvent(QCloseEvent* event) {
        emit windowClosed();
        QWidget::closeEvent(event);  // Вызываем базовую реализацию
    }

    void durationChanged(qint64 Duration);

    void positionChanged(qint64 Duration);

    void on_pushButton_Volume_clicked();

    void on_horizontalSlider_Duration_valueChanged(int value);

    void on_pushButton_forward_clicked();

    void on_pushButton_play_pause_clicked();

    void on_pushButton_backward_clicked();

    void on_horizontalSlider_Volume_valueChanged(int value);

    void closeEvent(QCloseEvent *event);

    void on_back_button_clicked();

signals:
    void windowClosed();

private:
    DatabaseManager& m_dbManager;
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

#endif // DETAILFILMWIDGET_H
