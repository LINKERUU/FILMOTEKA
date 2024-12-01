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
    bool m_isLiked = true;
    DatabaseManager& m_dbManager; // Добавлено поле для менеджера базы данных

    // Методы для настройки интерфейса
    void setupUI(const QString& title, const QString& year, const QString& genre, double rating, const QByteArray& posterData);
    QVBoxLayout* createMainLayout();
    void setupPoster(QVBoxLayout* mainLayout, const QByteArray& posterData, QString title);
    QLabel* createImageLabel(const QByteArray& posterData,QString title);
    void setupRatingAndLikeButtons(QVBoxLayout* mainLayout, double rating);
    QHBoxLayout* createRatingLikeLayout(double rating);
    QLabel* createRatingLabel(double rating);
    QPushButton* createLikeButton();
    void setupTitleAndInfo(QVBoxLayout* mainLayout, const QString& title, const QString& year, const QString& genre);
    QVBoxLayout* createInfoLayout(const QString& title, const QString& year, const QString& genre);
    QHBoxLayout* createTitleLabel(const QString& title);
    QHBoxLayout* createYearGenreLayout(const QString& year, const QString& genre);
    void toggleLikeButton();
    void openNewWindow(const QString& title);
    DetailFIlmWidget *film_window;
};

#endif // MOVIEWIDGET_H
