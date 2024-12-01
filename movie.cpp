#include "movie.h"

Movie::Movie(const QString& title, int year, const QString& genre, double rating, const QByteArray& poster)
    : m_title(title), m_year(year), m_genre(genre), m_rating(rating), m_poster(poster) {}

QString Movie::title() const {
    return m_title;
}

int Movie::year() const {
    return m_year;
}

QString Movie::genre() const {
    return m_genre;
}

double Movie::rating() const {
    return m_rating;
}

QByteArray Movie::poster() const {
    return m_poster;
}

void Movie::setTitle(const QString& title) {
    m_title = title;
}

void Movie::setYear(int year) {
    m_year = year;
}

void Movie::setGenre(const QString& genre) {
    m_genre = genre;
}

void Movie::setRating(double rating) {
    m_rating = rating;
}

void Movie::setPoster(const QByteArray& poster) {
    m_poster = poster;
}
