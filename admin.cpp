#include "admin.h"
#include "ui_admin.h"


Admin::Admin(DatabaseManager& dbManager, QWidget *parent)
    : QDialog(parent), m_dbManager(dbManager), ui(new Ui::Admin)
{
    ui->setupUi(this);
    setFixedSize(1384, 634);
    setWindowTitle("Панель администратора");

    connect(ui->pick_poster, &QPushButton::clicked, this, &Admin::on_pick_poster_clicked);
    connect(ui->pick_movie, &QPushButton::clicked, this, &Admin::on_pick_movie_clicked);
    connect(ui->addmovie_button, &QPushButton::clicked, this, &Admin::on_addmovie_button_clicked);
    connect(ui->remove_button, &QPushButton::clicked, this, &Admin::on_remove_button_clicked);
    connect(ui->quit_button, &QPushButton::clicked, this, &Admin::on_quit_button_clicked);

    updateTable();
}

Admin::~Admin() {
    delete ui;
}

void Admin::on_pick_poster_clicked() {
    try {
        QString Filename = QFileDialog::getOpenFileName(this, tr("Select poster"), "", tr("Image Files (*.jpeg *.jpg *.png)"));
        if (Filename.isEmpty()) return;

        QPixmap poster(Filename);
        if (poster.isNull()) {
            throw QString("Не удалось загрузить изображение из файла: %1").arg(Filename);
        }

        int labelWidth = ui->poster_label->width();
        int labelHeight = ui->poster_label->height();
        ui->poster_label->setPixmap(poster.scaled(labelWidth, labelHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } catch (const QString& err) {
        ExceptionHandler::handleException(err, this);
    } catch (...) {
        ExceptionHandler::handleUnknownException(this);
    }
}

void Admin::on_pick_movie_clicked() {
    try {
        QString Filename = QFileDialog::getOpenFileName(this, tr("Select movie"), "", tr("Video Files (*.mp4 *.mov)"));
        if (Filename.isEmpty()) return;

        QFile file(Filename);
        if (!file.exists()) {
            throw QString("Файл не найден: %1").arg(Filename);
        }

        ui->url_movie_label->setText(Filename);
    } catch (const QString& err) {
        ExceptionHandler::handleException(err, this);
    } catch (...) {
        ExceptionHandler::handleUnknownException(this);
    }
}

void Admin::on_addmovie_button_clicked() {
    try {
        QByteArray movie = readVideoFile(ui->url_movie_label->text());
        QString title = ui->title_label->text().trimmed();
        int year = ui->year_box->text().toInt();
        QString genre = ui->genre_label->text();
        double rating = ui->rating_box->text().replace(",", ".").toFloat();
        QPixmap poster = ui->poster_label->pixmap();
        QString description = ui->description_textEdit->toPlainText();
        int duration = ui->duration_label->text().toInt();
        QString director = ui->director_label->text();

        if (movie.isEmpty()) throw QString("Необходимо указать верный путь к фильму");
        if (title.isEmpty()) throw QString("Необходимо указать название фильма");
        if (ui->year_box->text().isEmpty()) throw QString("Необходимо указать год");
        if (genre.isEmpty()) throw QString("Необходимо указать название жанра");
        if (ui->rating_box->text().isEmpty()) throw QString("Необходимо указать рейтинг");
        if (poster.isNull()) throw QString("Необходимо выбрать постер");
        if (description.isEmpty()) throw QString("Необходимо указать описание");
        if (duration <= 0) throw QString("Длительность должна быть больше нуля");
        if (director.isEmpty()) throw QString("Необходимо указать режиссера");


        m_dbManager.open();
        m_dbManager.insertMovie(title, year, genre, rating, poster, description, duration, director, movie);
        m_dbManager.close();

        updateTable();

        // Очистка полей
        ui->title_label->clear();
        ui->year_box->clear();
        ui->genre_label->clear();
        ui->rating_box->clear();
        ui->poster_label->clear();
        ui->duration_label->clear();
        ui->director_label->clear();
        ui->url_movie_label->clear();
        ui->description_textEdit->clear();
        ui->poster_label->setText("Постер");
    } catch (const QString& err) {
        ExceptionHandler::handleException(err, this);
    } catch (...) {
        ExceptionHandler::handleUnknownException(this);
    }
}

void Admin::updateTable() {
    m_dbManager.open();
    try {
        QSqlTableModel* model = new QSqlTableModel(this, m_dbManager.getDatabase());
        model->setTable("movies");
        model->setEditStrategy(QSqlTableModel::OnFieldChange);

        if (!model->select()) {
            throw QString("Не удалось загрузить данные из базы.");
        }

        ui->tableView->setModel(model);
        ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);

        // Устанавливаем делегаты для проверки значений
        ui->tableView->setItemDelegateForColumn(0, new NonEditableDelegate(ui->tableView)); // Название
        ui->tableView->setItemDelegateForColumn(1, new ExceptionHandler()); // Год
        ui->tableView->setItemDelegateForColumn(2, new ExceptionHandler()); // Рейтинг
        ui->tableView->setItemDelegateForColumn(4, new ExceptionHandler()); // Длительность
        ui->tableView->setItemDelegateForColumn(7, new ExceptionHandler()); // Длительность

        // Настройка заголовков столбцов
        model->setHeaderData(0, Qt::Horizontal, tr("ID"));
        model->setHeaderData(1, Qt::Horizontal, tr("Название"));
        model->setHeaderData(2, Qt::Horizontal, tr("Год"));
        model->setHeaderData(3, Qt::Horizontal, tr("Жанр"));
        model->setHeaderData(4, Qt::Horizontal, tr("Рейтинг"));
        model->setHeaderData(6, Qt::Horizontal, tr("Описание"));
        model->setHeaderData(7, Qt::Horizontal, tr("Длительность"));
        model->setHeaderData(8, Qt::Horizontal, tr("Режиссер"));

        ui->tableView->hideColumn(5);
        ui->tableView->hideColumn(9);

    } catch (const QString& err) {
        ExceptionHandler::handleException(err, this);
    } catch (...) {
        ExceptionHandler::handleUnknownException(this);
    }
}



void Admin::on_remove_button_clicked() {
    try {
        int id = ui->removeName_label->text().toInt();
        if (id <= 0) throw QString("Необходимо указать корректный ID.");

        m_dbManager.open();
        m_dbManager.deleteMovieById(id);
        m_dbManager.close();

        ui->removeName_label->clear();
        updateTable();
    } catch (const QString& err) {
        ExceptionHandler::handleException(err, this);
    } catch (...) {
        ExceptionHandler::handleUnknownException(this);
    }
}

QByteArray Admin::readVideoFile(const QString& filePath) {
    try {
        QFile file(filePath);

        if (!file.exists()) throw QString("Файл не найден: %1").arg(filePath);
        if (!file.open(QIODevice::ReadOnly)) throw QString("Ошибка открытия файла: %1").arg(file.errorString());

        QByteArray videoData = file.readAll();
        file.close();
        return videoData;
    } catch (const QString& err) {
        ExceptionHandler::handleException(err, this);
        return QByteArray();
    } catch (...) {
        ExceptionHandler::handleUnknownException(this);
        return QByteArray();
    }
}




void Admin::on_quit_button_clicked()
{
    Authentication *login = new Authentication(m_dbManager);
    this->close();
    login->exec();
}

