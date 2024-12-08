#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H


#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QPixmap>
#include <QByteArray>
#include <QBuffer>
#include <QByteArray>
#include "filmdetail.h"
#include "stack.h"



class DatabaseManager {
public:
    DatabaseManager(const QString& dbName);
    bool open();
    void close();
    bool createTable();
    bool deleteMovieById(int id);
    bool insertMovie(const QString& title, int year, const QString& genre, double rating, const QPixmap& poster,
                     const QString& description, int duraction, const QString& director , const QByteArray& movie);
    Stack<Movie> getMovies();
    Stack<Movie> search(const QString& text);
    Stack<FilmDetail> getFilm(const QString& m_title);
    QSqlDatabase getDatabase();
    bool insertUser(const QString& username, const QString& password);
    QString login(const QString& username);
    bool addFavoriteMovie(int user_id, int movie_id);
    bool removeFavoriteMovie(int user_id, int movie_id);
    Stack<Movie> getFavoriteMovies(int user_id);
    int getUserId();
    int getMovieId(const QString& title);
    bool isMovieLiked(int user_id, int movie_id);


protected:
    int m_currentUserId;
    QSqlDatabase m_db;
};

#endif // DATABASE_MANAGER_H

