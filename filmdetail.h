#ifndef FILMDETAIL_H
#define FILMDETAIL_H

#include "movie.h"

class FilmDetail : public Movie {
public:
    FilmDetail() = default;
    FilmDetail(const QString& title, int year, const QString& genre, double rating, const QByteArray& poster,
               const QString& description, int duraction, const QString& director, const QByteArray& movie);

    QString description() const;
    int duraction() const;
    QString director() const;
    QByteArray movie() const;

    void setDescription(const QString& description);
    void setDuraction(int duraction);
    void setDirector(const QString& director);
    void setMovie(const QByteArray& movie);

protected:
    QString m_description;
    int m_duraction;
    QString m_director;
    QByteArray m_movie;
};

#endif // FILMDETAIL_H
