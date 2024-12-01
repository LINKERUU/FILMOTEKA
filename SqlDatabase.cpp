#include "SqlDatabase.h"


DatabaseManager:: DatabaseManager(const QString& dbName) {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbName);
}

bool DatabaseManager::open() {
    if (!m_db.open()) {
        qDebug() << "Ошибка при открытии базы данных:" << m_db.lastError().text();
        return false;
    }
    return true;
}

void DatabaseManager:: close() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}

bool DatabaseManager:: createTable() {
    QSqlQuery query;
    bool success = query.exec("CREATE TABLE IF NOT EXISTS movies ("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                              "title TEXT, "
                              "year INTEGER, "
                              "genre TEXT, "
                              "rating REAL, "
                              "poster BLOB, "
                              "description TEXT, "
                              "duraction INTEGER, "
                              "director TEXT, "
                              "movie BLOB)");

    if (!success) {
        qDebug() << "Ошибка при создании таблицы:" << query.lastError().text();
    }
    return success;
}

bool DatabaseManager:: insertMovie(const QString& title, int year, const QString& genre, double rating, const QPixmap& poster,
                                  const QString& description, int duraction, const QString& director , const QByteArray& movie) {
    QSqlQuery query;
    QByteArray posterData;
    QBuffer buffer(&posterData);
    buffer.open(QIODevice::WriteOnly);
    poster.save(&buffer, "JPEG");

    query.prepare("INSERT INTO movies (title, year, genre, rating, poster, description, duraction, director, movie)"
                  "VALUES (:title, :year, :genre, :rating, :poster, :description, :duraction, :director, :movie)");
    query.bindValue(":title", title);
    query.bindValue(":year", year);
    query.bindValue(":genre", genre);
    query.bindValue(":rating", rating);
    query.bindValue(":poster", posterData);
    query.bindValue(":description", description);
    query.bindValue(":duraction", duraction);
    query.bindValue(":director", director);
    query.bindValue(":movie", movie);

    if (!query.exec()) {
        qDebug() << "Ошибка при вставке фильма:" << query.lastError().text();
        return false;
    }
    return true;
}

Stack<Movie> DatabaseManager::getMovies() {
    Stack<Movie> movies;
    QSqlQuery query("SELECT title, year, genre, rating, poster FROM movies");

    while (query.next()) {
        Movie movie;
        movie.setTitle(query.value(0).toString());
        movie.setYear(query.value(1).toInt());
        movie.setGenre(query.value(2).toString());
        movie.setRating(query.value(3).toDouble());
        movie.setPoster(query.value(4).toByteArray());
        movies.add(movie);
    }
    return movies;
}

Stack<FilmDetail> DatabaseManager::getFilm(const QString& m_title) {
    QSqlQuery query;
    query.prepare("SELECT * FROM movies WHERE title = :title");
    query.bindValue(":title", m_title);

    Stack<FilmDetail> films;

    if (!query.exec()) {
        qWarning() << "Ошибка выполнения запроса:" << query.lastError().text();
        return films;
    }

    while (query.next()) {
        FilmDetail film;
        film.setTitle(query.value(1).toString());
        film.setYear(query.value(2).toInt());
        film.setGenre(query.value(3).toString());
        film.setRating(query.value(4).toDouble());
        film.setPoster(query.value(5).toByteArray());
        film.setDescription(query.value(6).toString());
        film.setDuraction(query.value(7).toInt());
        film.setDirector(query.value(8).toString());
        film.setMovie(query.value(9).toByteArray());
        films.add(film);
    }

    return films;
}

bool DatabaseManager::deleteMovieById(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM movies WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Ошибка при удалении фильма с id" << id << ":" << query.lastError().text();
        return false;
    }
    return true;
}

QSqlDatabase DatabaseManager::getDatabase() {
    return QSqlDatabase::database();
}


Stack<Movie> DatabaseManager::search(const QString& text) {
    Stack<Movie> movies;

    QSqlQuery query;

    query.prepare("SELECT title, year, genre, rating, poster FROM movies WHERE UPPER(title) LIKE :title");
    query.bindValue(":title", "%" + text.toUpper() + "%");

    if (!query.exec()) {
        qDebug() << "Ошибка выполнения запроса поиска: " << query.lastError().text();
        return movies;
    }

    while (query.next()) {
        Movie movie;
        movie.setTitle(query.value("title").toString());
        movie.setYear(query.value("year").toInt());
        movie.setGenre(query.value("genre").toString());
        movie.setRating(query.value("rating").toDouble());
        movie.setPoster(query.value("poster").toByteArray());
        movies.add(movie);
    }

    return movies;
}



