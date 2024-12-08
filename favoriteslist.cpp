#include "FavoritesList.h"

FavoritesList::FavoritesList(DatabaseManager& dbManager, QWidget* parent)
    : ListMovie(dbManager, parent) {
    setWindowTitle("Избранное");
    loadMovies();
    setupUI();

}

void FavoritesList::setupUI() {
    auto* exitbutton = new QPushButton(this);
    auto* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    auto* mainLayout = new QVBoxLayout(centralWidget);
    auto* scrollArea = new QScrollArea();
    auto* menulayout = new QHBoxLayout();

    // Настраиваем кнопку выхода
    exitbutton->setFixedSize(50, 40);
    exitbutton->setIcon(QIcon(":/icons/back_arrow.png"));
    exitbutton->setStyleSheet("QPushButton{"
                              " border-radius:15px;"
                              " background-color:#3b3b3a;} "
                              "QPushButton:hover{"
                              " background-color:#4b4b4b;"
                              " border:1px solid #303030;}");

    scrollArea->setWidgetResizable(true);
    QLabel* name = new QLabel("Понравившиеся фильмы", this);
    name->setAlignment(Qt::AlignCenter);
    name->setStyleSheet("font-size: 28px; color: #ededed; font-weight:700");

    menulayout->addWidget(exitbutton);
    menulayout->addWidget(name);
    mainLayout->addLayout(menulayout);
    mainLayout->addWidget(scrollArea);

    // Связываем кнопку выхода с закрытием окна
    connect(exitbutton, &QPushButton::clicked, this, &FavoritesList::close);

    // Настройка области с фильмами
    auto* scrollWidget = new QWidget();
    auto* gridLayout = new QGridLayout(scrollWidget);
    scrollArea->setWidget(scrollWidget);

    configureGridLayout(gridLayout);
}

void FavoritesList::loadMovies() {
    int userId = m_dbManager.getUserId();

    Stack<Movie> favoriteMovies = m_dbManager.getFavoriteMovies(userId);

    auto* scrollWidget = static_cast<QWidget*>(centralWidget()->findChild<QScrollArea*>()->widget());
    auto* gridLayout = static_cast<QGridLayout*>(scrollWidget->layout());

    if (!gridLayout) return;

    clearGridLayout(gridLayout);

    if (!favoriteMovies.hasNext()) {
        QLabel* noMoviesLabel = new QLabel("Избранных фильмов нет", this);
        noMoviesLabel->setAlignment(Qt::AlignCenter);
        noMoviesLabel->setStyleSheet("font-size: 24px; color: gray;");
        gridLayout->addWidget(noMoviesLabel, 0, 0);
        return;
    }

    updateMovieDisplay(favoriteMovies, true);
}


void FavoritesList::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
    loadMovies();
}

void FavoritesList::on_exitbutton_click() {
    ListMovie* movieListWindow = new ListMovie(m_dbManager, nullptr);
    movieListWindow->show();

    this->close();
    delete this;
}
