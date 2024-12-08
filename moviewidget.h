#ifndef MOVIEWIDGET_H
#define MOVIEWIDGET_H

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <QFont>
#include <QGridLayout>
#include <QBuffer>
#include <QPainter>
#include <QTransform>
#include <QRect>
#include <QMessageBox>
#include <QMouseEvent>
#include "ClickableLabel.h"
#include "ListMovies.h"
#include "detailfilmwidget.h"


class DetailFIlmWidget;

class MovieWidget : public QWidget {
    Q_OBJECT

public:
    MovieWidget(const QString& title, const QString& year, const QString& genre, double rating,
                const QByteArray& posterData, DatabaseManager& dbManager, QWidget* parent = nullptr);

private:
    bool m_isLiked = false;
    DatabaseManager& m_dbManager; // Добавлено поле для менеджера базы данных

    // Методы для настройки интерфейса
    void setupUI(const QString& title, const QString& year, const QString& genre, double rating, const QByteArray& posterData);
    QVBoxLayout* createMainLayout();
    void setupPoster(QVBoxLayout* mainLayout, const QByteArray& posterData, QString title);
    QLabel* createImageLabel(const QByteArray& posterData,QString title);
    void setupRatingAndLikeButtons(QVBoxLayout* mainLayout, double rating , const QString& title);
    QLabel* createRatingLabel(double rating);
    QPushButton* createLikeButton(const QString& title);
    void setupTitleAndInfo(QVBoxLayout* mainLayout, const QString& title, const QString& year, const QString& genre);
    QVBoxLayout* createInfoLayout(const QString& title, const QString& year, const QString& genre);
    QHBoxLayout* createTitleLabel(const QString& title);
    QHBoxLayout* createYearGenreLayout(const QString& year, const QString& genre);
    void toggleLikeButton(const QString& title, QPushButton* likeButton);
    void openNewWindow(const QString& title);
    DetailFIlmWidget *film_window;
    void checkLikedMovie(const QString& title, QPushButton* likeButton);
    QHBoxLayout* createRatingLikeLayout(double rating, const QString& title);
    QPushButton* createDeleteButton(const QString& title);
    void deleteMovieFromFavorites(const QString& title);

};

#endif // MOVIEWIDGET_H
