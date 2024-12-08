#include "ListMovies.h"
#include "FavoritesList.h"  // Добавьте это, чтобы подключить класс FavoritesList


ListMovie::ListMovie(DatabaseManager& dbManager, QWidget* parent)
    : QMainWindow(parent), m_dbManager(dbManager) {
    setWindowTitle("Фильмы");
    setupUI();
    loadMovies(true);
}

void ListMovie::setupUI() {
    auto* searchline = new QLineEdit(this);
    auto* exitbutton = new QPushButton(this);
    auto* centralWidget = new QWidget(this);
    auto* likedcatalog = new QPushButton(this);
    setCentralWidget(centralWidget);

    auto* mainLayout = new QVBoxLayout(centralWidget);
    auto* scrollArea = new QScrollArea();
    auto* menulayout = new QHBoxLayout();

    exitbutton->setFixedSize(50, 40);
    exitbutton->setIcon(QIcon(":/icons/back_arrow.png"));
    exitbutton->setStyleSheet("QPushButton{"
                              " border-radius:15px;"
                              " background-color:#3b3b3a;}"
                              "QPushButton:hover{"
                              " background-color:#4b4b4b;"
                              " border:1px solid #303030;}");

    likedcatalog->setFixedSize(100, 40);
    likedcatalog->setIcon(QIcon(":/icons/favorites.png"));
    likedcatalog->setIconSize(QSize(26, 26));
    likedcatalog->setStyleSheet("QPushButton{"
                              " border-radius:15px;"
                              " background-color:#3b3b3a;}"
                              "QPushButton:hover{"
                              " background-color:#4b4b4b;"
                              " border:1px solid #303030;}");

    searchline->setFixedSize(300, 40);
    searchline->setPlaceholderText("Поиск");
    searchline->setFocusPolicy(Qt::ClickFocus);
    searchline->setStyleSheet("QLineEdit{"
                              " border-radius:18px;"
                              " padding:0 10px 0 20px;"
                              " font:17pt;"
                              " background-color:#3b3b3a;}"
                              "QLineEdit::placeholder{"
                              " background-color:#4b4b4b;"
                              " border:1px solid #303030;}"
                              "QLineEdit:focus{"
                              " border:1px solid white;}");

    scrollArea->setWidgetResizable(true);

    menulayout->addWidget(exitbutton);
    menulayout->addWidget(likedcatalog,1, Qt::AlignRight);
    menulayout->addWidget(searchline, 0, Qt::AlignRight);
    mainLayout->addLayout(menulayout);
    mainLayout->addWidget(scrollArea);

    auto* scrollWidget = new QWidget();
    auto* gridLayout = new QGridLayout(scrollWidget);
    scrollArea->setWidget(scrollWidget);

    connect(likedcatalog, &QPushButton::clicked, this, &ListMovie::openFavoriteMovies);
    connect(exitbutton, &QPushButton::clicked, this, &ListMovie::on_exitbutton_click);
    connect(searchline, &QLineEdit::textChanged, this, &ListMovie::search_movie);

    configureGridLayout(gridLayout);
}

void ListMovie::search_movie(const QString& text) {
    if (!m_dbManager.open()) {
        qDebug() << "Не удалось открыть базу данных.";
        return;
    }

    Stack<Movie> movies = m_dbManager.search(text);

    if (movies.hasNext()) {
        updateMovieDisplay(movies,true);
    } else {
        auto* scrollWidget = static_cast<QWidget*>(centralWidget()->findChild<QScrollArea*>()->widget());
        auto* gridLayout = static_cast<QGridLayout*>(scrollWidget->layout());

        if (gridLayout) {
            clearGridLayout(gridLayout);
        }

        QLabel* noResultsLabel = new QLabel("Результатов не найдено", this);
        noResultsLabel->setAlignment(Qt::AlignCenter);
        noResultsLabel->setStyleSheet("font-size: 24px; color: gray;");
        gridLayout->addWidget(noResultsLabel, 0, 0);
    }
}

void ListMovie::configureGridLayout(QGridLayout* gridLayout) {
    gridLayout->setContentsMargins(80, 20, 0, 10);
    gridLayout->setSpacing(20);
    setMinimumSize(1100, 600);
}

void ListMovie::loadMovies(bool flag) {
    if (!m_dbManager.open()) {
        qDebug() << "Не удалось открыть базу данных.";
        return;
    }

    Stack<Movie> movies = m_dbManager.getMovies();

    updateMovieDisplay(movies,flag);
}

void ListMovie::updateMovieDisplay(Stack<Movie>& movies,bool update) {
    auto* scrollWidget = static_cast<QWidget*>(centralWidget()->findChild<QScrollArea*>()->widget());
    auto* gridLayout = static_cast<QGridLayout*>(scrollWidget->layout());

    if (!gridLayout) return;


    static int count=0;
    int width  = centralWidget()->width();
    int iterator=0;


    if (width > 1400 && (count != 5 || update)) {
        count = 5;
        gridLayout->setContentsMargins(30, 20, 0, 10);
        gridLayout->setSpacing(20);
    } else if (width <= 1400 && width > 1300 && (count != 4 || update)) {
        count = 4;
        gridLayout->setContentsMargins(30, 20, 0, 10);
        gridLayout->setSpacing(0);
    } else if (width <= 1200 && (count != 3 || update)) {
        count = 3;
        gridLayout->setContentsMargins(80, 20, 0, 10);
        gridLayout->setSpacing(20);
    } else {
        return;
    }


    clearGridLayout(gridLayout);

    for (auto it = movies.begin(); it != movies.end(); ++it) {
        const Movie& movie = *it;
        gridLayout->addWidget(
            new MovieWidget(
                movie.title(),
                QString::number(movie.year()),
                movie.genre(),
                movie.rating(),
                movie.poster(),
                m_dbManager,
                this),
            iterator / count, iterator % count
            );
        ++iterator;
    }
}

void ListMovie::clearGridLayout(QGridLayout* gridLayout) {
    QLayoutItem* item;
    while ((item = gridLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
}

void ListMovie::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
    loadMovies(false);
}

void ListMovie::on_exitbutton_click() {
    Authentication* login = new Authentication(m_dbManager);
    this->close();
    login->exec();
}

void ListMovie::openFavoriteMovies() {
    auto* likedMovieWindow = new FavoritesList(m_dbManager, this);
    likedMovieWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(likedMovieWindow, &QWidget::destroyed, this, [this]() {
        this->show();
    });
    likedMovieWindow->show();
    this->hide();
}

