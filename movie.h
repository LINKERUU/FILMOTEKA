#ifndef MOVIE_H
#define MOVIE_H

#include <QString>
#include <QByteArray>

class Movie {
public:
    Movie() = default;
    Movie(const QString& title, int year, const QString& genre, double rating, const QByteArray& poster);

    QString title() const;
    int year() const;
    QString genre() const;
    double rating() const;
    QByteArray poster() const;

    void setTitle(const QString& title);
    void setYear(int year);
    void setGenre(const QString& genre);
    void setRating(double rating);
    void setPoster(const QByteArray& poster);

protected:
    QString m_title;
    int m_year;
    QString m_genre;
    double m_rating;
    QByteArray m_poster;
};

#endif // MOVIE_H
