#include "SqlDatabase.h"

DatabaseManager::DatabaseManager(const QString& dbName) {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbName);
    createTable();
}

bool DatabaseManager::open() {
    if (!m_db.open()) {
        qCritical() << "Ошибка при открытии базы данных:" << m_db.lastError().text();
        return false;
    }
    return true;
}

void DatabaseManager::close() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}

bool DatabaseManager::createTable() {

    open();

    QSqlQuery query;

    // Создание таблицы фильмов
    QSqlQuery queryMovies;
    bool moviesTableSuccess = queryMovies.exec(
        "CREATE TABLE IF NOT EXISTS movies ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "title TEXT, "
        "year INTEGER, "
        "genre TEXT, "
        "rating REAL, "
        "poster BLOB, "
        "description TEXT, "
        "duraction INTEGER, "
        "director TEXT, "
        "movie BLOB)"
        );


    if (!moviesTableSuccess) {
        qCritical() << "Ошибка при создании таблиц:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::insertMovie(const QString& title, int year, const QString& genre, double rating,
                                  const QPixmap& poster, const QString& description, int duraction,
                                  const QString& director, const QByteArray& movie) {
    QSqlQuery query;

    QByteArray posterData;
    QBuffer buffer(&posterData);
    if (!buffer.open(QIODevice::WriteOnly) || !poster.save(&buffer, "JPEG")) {
        qCritical() << "Ошибка при обработке постера.";
        return false;
    }

    // Подготовка SQL-запроса
    query.prepare(
        "INSERT INTO movies (title, year, genre, rating, poster, description, duraction, director, movie) "
        "VALUES (:title, :year, :genre, :rating, :poster, :description, :duraction, :director, :movie)"
        );
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
        qCritical() << "Ошибка при добавлении фильма:" << query.lastError().text();
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
    Stack<FilmDetail> films;
    QSqlQuery query;

    query.prepare("SELECT * FROM movies WHERE title = :title");
    query.bindValue(":title", m_title);

    if (!query.exec()) {
        qCritical() << "Ошибка при выполнении запроса:" << query.lastError().text();
        return films;
    }

    while (query.next()) {
        FilmDetail film;
        film.setTitle(query.value("title").toString());
        film.setYear(query.value("year").toInt());
        film.setGenre(query.value("genre").toString());
        film.setRating(query.value("rating").toDouble());
        film.setPoster(query.value("poster").toByteArray());
        film.setDescription(query.value("description").toString());
        film.setDuraction(query.value("duraction").toInt());
        film.setDirector(query.value("director").toString());
        film.setMovie(query.value("movie").toByteArray());
        films.add(film);
    }

    return films;
}

bool DatabaseManager::deleteMovieById(int id) {
    QSqlQuery query;

    query.prepare("DELETE FROM movies WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qCritical() << "Ошибка при удалении фильма с id" << id << ":" << query.lastError().text();
        return false;
    }

    return true;
}

QSqlDatabase DatabaseManager::getDatabase() {
    return m_db;
}

Stack<Movie> DatabaseManager::search(const QString& text) {
    Stack<Movie> movies;
    QSqlQuery query;

    query.exec("PRAGMA case_sensitive_like=OFF");

    query.prepare(
        "SELECT title, year, genre, rating, poster FROM movies WHERE "
        "title LIKE :title OR UPPER(title) LIKE :title_upper"
        );
    query.bindValue(":title", "%" + text + "%");
    query.bindValue(":title_upper", "%" + text.toUpper() + "%");

    if (!query.exec()) {
        qCritical() << "Ошибка при выполнении запроса поиска:" << query.lastError().text();
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

bool DatabaseManager::insertUser(const QString& username, const QString& password) {

    QSqlQuery query;

    query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (!query.exec()) {
        qCritical() << "Ошибка при добавлении пользователя:" << query.lastError().text();
        return false;
    }

    return true;
}


QString DatabaseManager::login(const QString& username) {

    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qCritical() << "Ошибка выполнения запроса:" << query.lastError().text();
        return QString();  // Возвращаем пустую строку в случае ошибки запроса
    }

    if (!query.next()) {
        qCritical() << "Пользователь не найден:" << username;
        return QString();  // Возвращаем пустую строку, если пользователь не найден
    }

    return query.value(0).toString();
}
