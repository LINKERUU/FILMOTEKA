#include "filmdetail.h"

FilmDetail::FilmDetail(const QString& title, int year, const QString& genre, double rating, const QByteArray& poster,
    const QString& description, int duraction,const QString& director ,const QByteArray& movie):
    Movie(title,year,genre,rating,poster),m_description(description),m_duraction(duraction), m_director(director), m_movie(movie){}


QString FilmDetail::description() const{
    return m_description;
}
int FilmDetail::duraction() const{
    return m_duraction;
}
QString FilmDetail::director() const{
    return m_director;
}
QByteArray FilmDetail::movie() const{
    return m_movie;
}

void FilmDetail::setDescription(const QString& description){
    m_description = description;
}
void FilmDetail::setDuraction(int duraction){
    m_duraction = duraction;
}
void FilmDetail::setDirector(const QString& director){
    m_director = director;
}
void FilmDetail::setMovie(const QByteArray& movie){
    m_movie = movie;
}

