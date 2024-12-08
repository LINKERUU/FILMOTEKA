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
    if (!open()) {
        qCritical() << "Не удалось открыть базу данных для создания таблиц.";
        return false;
    }

    QSqlQuery query;

    // Создание таблицы movies
    QString createMoviesTableQuery =
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
        "movie BLOB)";

    if (!query.exec(createMoviesTableQuery)) {
        qCritical() << "Ошибка при создании таблицы movies:" << query.lastError().text();
        return false;
    }

    // Создание таблицы users
    QString createUsersTableQuery =
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username TEXT UNIQUE NOT NULL, "
        "password TEXT NOT NULL, "
        "liked_movies_id TEXT)";

    if (!query.exec(createUsersTableQuery)) {
        qCritical() << "Ошибка при создании таблицы users:" << query.lastError().text();
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
    query.prepare("SELECT id, password FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qCritical() << "Ошибка выполнения запроса:" << query.lastError().text();
        return QString();
    }

    if (!query.next()) {
        qCritical() << "Пользователь не найден:" << username;
        return QString();
    }

    m_currentUserId = query.value(0).toInt();

    return query.value(1).toString();
}

bool DatabaseManager::addFavoriteMovie(int user_id, int movie_id) {
    QSqlQuery query;

    query.prepare("SELECT liked_movies_id FROM users WHERE id = :user_id");
    query.bindValue(":user_id", user_id);

    if (!query.exec() || !query.next()) {
        qCritical() << "Ошибка при получении списка понравившихся фильмов:" << query.lastError().text();
        return false;
    }

    QString likedMovies = query.value(0).toString();
    QStringList moviesList = likedMovies.split(",", Qt::SkipEmptyParts);

    if (moviesList.contains(QString::number(movie_id))) {
        qDebug() << "Фильм уже добавлен в понравившиеся.";
        return true;
    }

    moviesList.append(QString::number(movie_id));
    likedMovies = moviesList.join(",");

    query.prepare("UPDATE users SET liked_movies_id = :liked_movies_id WHERE id = :user_id");
    query.bindValue(":liked_movies_id", likedMovies);
    query.bindValue(":user_id", user_id);

    if (!query.exec()) {
        qCritical() << "Ошибка при обновлении списка понравившихся фильмов:" << query.lastError().text();
        return false;
    }

    qDebug()<<"фильм добавлен";

    return true;
}

bool DatabaseManager::removeFavoriteMovie(int user_id, int movie_id) {
    QSqlQuery query;

    query.prepare("SELECT liked_movies_id FROM users WHERE id = :user_id");
    query.bindValue(":user_id", user_id);

    if (!query.exec() || !query.next()) {
        qCritical() << "Ошибка при получении списка понравившихся фильмов:" << query.lastError().text();
        return false;
    }

    QString likedMovies = query.value(0).toString();
    QStringList moviesList = likedMovies.split(",", Qt::SkipEmptyParts);

    if (!moviesList.contains(QString::number(movie_id))) {
        qDebug() << "Фильм отсутствует в списке понравившихся.";
        return true;
    }

    moviesList.removeAll(QString::number(movie_id));
    likedMovies = moviesList.join(",");

    query.prepare("UPDATE users SET liked_movies_id = :liked_movies_id WHERE id = :user_id");
    query.bindValue(":liked_movies_id", likedMovies);
    query.bindValue(":user_id", user_id);

    if (!query.exec()) {
        qCritical() << "Ошибка при обновлении списка понравившихся фильмов:" << query.lastError().text();
        return false;
    }

    qDebug()<<"фильм удален";

    return true;
}

Stack<Movie> DatabaseManager::getFavoriteMovies(int user_id) {
    QSqlQuery query;

    // Получаем строку с ID понравившихся фильмов
    query.prepare("SELECT liked_movies_id FROM users WHERE id = :user_id");
    query.bindValue(":user_id", user_id);

    if (!query.exec() || !query.next()) {
        qCritical() << "Ошибка при получении списка понравившихся фильмов:" << query.lastError().text();
        return Stack<Movie>();
    }

    QString likedMovies = query.value(0).toString();
    QStringList moviesList = likedMovies.split(",", Qt::SkipEmptyParts);

    if (moviesList.isEmpty()) {
        return Stack<Movie>(); // Нет понравившихся фильмов
    }

    // Получаем информацию о фильмах по их ID
    query.prepare(
        "SELECT id, title, year, genre, rating, poster FROM movies WHERE id IN (" +
        moviesList.join(",") + ")"
        );

    Stack<Movie> movies;

    if (!query.exec()) {
        qCritical() << "Ошибка при получении информации о фильмах:" << query.lastError().text();
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

int DatabaseManager::getMovieId(const QString& title) {
    QSqlQuery query;

    query.prepare("SELECT id FROM movies WHERE title = :title");
    query.bindValue(":title", title);

    if (!query.exec()) {
        qCritical() << "Ошибка при выполнении запроса для получения ID фильма:" << query.lastError().text();
        return -1;
    }

    if (query.next()) {
        return query.value(0).toInt();
    } else {
        qWarning() << "Фильм с названием" << title << "не найден.";
        return -1;
    }
}

int DatabaseManager::getUserId() {
    if (m_currentUserId == -1) {
        qWarning() << "Пользователь не авторизован.";
    }
    return m_currentUserId;
}

bool DatabaseManager::isMovieLiked(int user_id, int movie_id) {

    QSqlQuery query;

    // Получаем список понравившихся фильмов для пользователя
    query.prepare("SELECT liked_movies_id FROM users WHERE id = :user_id");
    query.bindValue(":user_id", user_id);

    if (!query.exec()) {
        qCritical() << "Ошибка при выполнении запроса для проверки понравившихся фильмов:" << query.lastError().text();
        return false;
    }

    if (!query.next()) {
        qWarning() << "Пользователь с id" << user_id << "не найден.";
        return false;
    }

    // Получаем строку с ID понравившихся фильмов
    QString likedMovies = query.value(0).toString();
    QStringList moviesList = likedMovies.split(",", Qt::SkipEmptyParts);

    // Проверяем наличие ID фильма в списке
    return moviesList.contains(QString::number(movie_id));
}
