#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

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

protected:
    QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H
