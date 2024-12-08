#include "moviewidget.h"
#include "favoriteslist.h"


MovieWidget::MovieWidget(const QString& title, const QString& year, const QString& genre, double rating,
                         const QByteArray& posterData, DatabaseManager& dbManager, QWidget* parent)
    : QWidget(parent), m_dbManager(dbManager) {

    setupUI(title, year, genre, rating, posterData);
}

void MovieWidget::setupUI(const QString& title, const QString& year, const QString& genre, double rating, const QByteArray& posterData) {
    auto* mainLayout = createMainLayout();
    setupPoster(mainLayout, posterData,title);
    setupRatingAndLikeButtons(mainLayout, rating ,title);
    setupTitleAndInfo(mainLayout, title, year, genre);
    setLayout(mainLayout);
}

QVBoxLayout* MovieWidget::createMainLayout() {
    return new QVBoxLayout();
}

void MovieWidget::setupPoster(QVBoxLayout* mainLayout, const QByteArray& posterData,QString title) {
    auto* imageLabel = createImageLabel(posterData,title);

    mainLayout->addWidget(imageLabel);
}

QLabel* MovieWidget::createImageLabel(const QByteArray& posterData, QString title) {
    ClickableLabel* imageLabel = new ClickableLabel("",this);;

    imageLabel->setFixedSize(210,300);
    QPixmap image;
    image.loadFromData(posterData, "JPEG");

    QFrame* frame = new QFrame();
    frame->setFrameShape(QFrame::Box);
    frame->setFrameShadow(QFrame::Raised);
    frame->setLineWidth(2);

    QBitmap roundedMask(image.size());
    roundedMask.fill(Qt::white);
    QPainter painter(&roundedMask);
    painter.setBrush(Qt::black);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawRoundedRect(roundedMask.rect(), 15, 15);

    image.setMask(roundedMask);

    imageLabel->setPixmap(image.scaled(300, 300, Qt::KeepAspectRatio));

    imageLabel->setCursor(Qt::PointingHandCursor);

    connect(imageLabel, &ClickableLabel::clicked, this, [this, title]() { openNewWindow(title); });



    return imageLabel;
}


void MovieWidget::setupRatingAndLikeButtons(QVBoxLayout* mainLayout, double rating , const QString& title) {

    auto* ratingLikeLayout = createRatingLikeLayout(rating, title);
    mainLayout->addLayout(ratingLikeLayout);
}

QLabel* MovieWidget::createRatingLabel(double rating) {
    QLabel* ratingLabel = new QLabel(QString::number(rating, 'f', 1));
    QString color;

    if (rating >= 0 && rating < 5) {
        color = "red";
    } else if (rating >= 5 && rating < 7) {
        color = "orange";
    } else if (rating >= 7 && rating <= 10) {
        color = "green";
    }
    ratingLabel->setStyleSheet("QLabel { background-color:"+color+";"
                                                                      " color:white;border-radius:6px; font-weight:bold;font-size:18px;"
                                                                      " padding: 0px 10px;}");
    return ratingLabel;
}

QPushButton* MovieWidget::createLikeButton(const QString& title) {
    QPushButton* likeButton = new QPushButton("♥");
    QFont font = likeButton->font();
    likeButton->setFixedSize(30, 30);
    likeButton->setFont(font);
    checkLikedMovie(title,likeButton);

    return likeButton;
}

void MovieWidget::checkLikedMovie(const QString& title,QPushButton* likeButton){
    int user_id = m_dbManager.getUserId();
    int movie_id = m_dbManager.getMovieId(title);

    if (!m_dbManager.isMovieLiked(user_id,movie_id)) {
        likeButton->setStyleSheet("QPushButton { color:white; border-radius:60px; font-size:36px; }"
                                  "QPushButton:hover { color:gray; }");
    }
    else {
        likeButton->setStyleSheet("QPushButton { color:red; border-radius:60px; font-size:36px; }");
    }
}


void MovieWidget::setupTitleAndInfo(QVBoxLayout* mainLayout, const QString& title, const QString& year, const QString& genre) {
    auto* infoLayout = createInfoLayout(title, year, genre);
    mainLayout->addLayout(infoLayout);
    mainLayout->addStretch();
}

QVBoxLayout* MovieWidget::createInfoLayout(const QString& title, const QString& year, const QString& genre) {
    auto* infoLayout = new QVBoxLayout();

    auto* titleLayout = createTitleLabel(title);
    infoLayout->addLayout(titleLayout);

    auto* yearGenreLayout = createYearGenreLayout(year, genre);
    infoLayout->addLayout(yearGenreLayout);

    return infoLayout;
}


QHBoxLayout* MovieWidget::createTitleLabel(const QString& title) {
    auto* titleLayout = new QHBoxLayout();
    ClickableLabel* titleLabel = new ClickableLabel(title,this);
    titleLabel->setFont(QFont("Arial", 15, QFont::Bold));
    titleLabel->setWordWrap(true);
    titleLabel->setContentsMargins(10, 0, 0, 0);
    titleLabel->setAlignment(Qt::AlignLeft);
    titleLabel->setScaledContents(false);
    titleLabel->setMaximumWidth(810);
    titleLabel->setStyleSheet("QLabel:hover{color:#ff3b3b;}");

    connect(titleLabel, &ClickableLabel::clicked, this, [this, title]() { openNewWindow(title); });

    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();

    return titleLayout;
}

void MovieWidget::openNewWindow(const QString& title) {
    Stack<FilmDetail> films = m_dbManager.getFilm(title);
    FilmDetail film=films.front();
    film_window = new DetailFIlmWidget(m_dbManager,film.title(), film.year(), film.genre(), film.rating(),
                                       film.poster(), film.description(), film.duraction(), film.director(), film.movie(),this);
    film_window->show();

}


QHBoxLayout* MovieWidget::createYearGenreLayout(const QString& year, const QString& genre) {
    auto* yearGenreLayout = new QHBoxLayout();
    yearGenreLayout->setContentsMargins(10, 0, 10, 0);

    QLabel* yearlabel = new QLabel(year);
    QLabel* elem = new QLabel(" /");
    QLabel* genrelabel = new QLabel(genre);

    yearlabel->setFont(QFont("Arial", 13));
    elem->setFont(QFont("Arial", 16));
    genrelabel->setFont(QFont("Arial", 13));
    yearlabel->setStyleSheet("QLabel:hover{color:#ff3b3b;}");
    genrelabel->setStyleSheet("QLabel{max-width:125px;}"
                              "QLabel:hover{color:#ff3b3b;}");

    yearGenreLayout->addWidget(yearlabel);
    yearGenreLayout->addWidget(elem);
    yearGenreLayout->addWidget(genrelabel);
    yearGenreLayout->addStretch();
    return yearGenreLayout;
}

void MovieWidget::toggleLikeButton(const QString& title, QPushButton* likeButton) {
    int user_id = m_dbManager.getUserId();
    int movie_id = m_dbManager.getMovieId(title);


    if (user_id == -1 || movie_id == -1) {
        qDebug() << "Ошибка получения данных из базы.";
        return;
    }

    if (m_isLiked) {
        m_dbManager.removeFavoriteMovie(user_id,movie_id);
        likeButton->setStyleSheet("QPushButton { color:white; border-radius:60px; font-size:36px; }"
                                      "QPushButton:hover { color:gray; }");
    }
    else {
        m_dbManager.addFavoriteMovie(user_id,movie_id);
        likeButton->setStyleSheet("QPushButton { color:red; border-radius:60px; font-size:36px; }");
    }

    m_isLiked = !m_isLiked;
}

QHBoxLayout* MovieWidget::createRatingLikeLayout(double rating, const QString& title) {
    auto* ratingLikeLayout = new QHBoxLayout();
    ratingLikeLayout->setContentsMargins(10, 0, 60, 0);

    auto* ratingLabel = createRatingLabel(rating);

    if (qobject_cast<FavoritesList*>(parent())) {
        auto* deleteButton = createDeleteButton(title);
        ratingLikeLayout->addWidget(deleteButton);
    }
    else{
        auto* likeButton = createLikeButton(title);
        connect(likeButton, &QPushButton::clicked, this, [this, title, likeButton]() {
            toggleLikeButton(title, likeButton);
        });
        ratingLikeLayout->addWidget(likeButton);
    }
    ratingLikeLayout->addWidget(ratingLabel);
    ratingLikeLayout->setSpacing(100);
    ratingLikeLayout->addStretch();
    return ratingLikeLayout;
}

QPushButton* MovieWidget::createDeleteButton(const QString& title) {
    QPushButton* deleteButton = new QPushButton("");
    deleteButton->setIcon(QIcon(":/icons/white_cart.png"));
    deleteButton->setIconSize(QSize(25,25));
    deleteButton->setStyleSheet("QPushButton { font-size: 18px; color: #ededed; border: none; background-color: transparent; }"
                                "QPushButton:hover { background-color: red; }");

    connect(deleteButton, &QPushButton::clicked, this, [this, title]() {
        deleteMovieFromFavorites(title);
    });

    return deleteButton;
}

void MovieWidget::deleteMovieFromFavorites(const QString& title) {
    int user_id = m_dbManager.getUserId();
    int movie_id = m_dbManager.getMovieId(title);

    if (user_id == -1 || movie_id == -1) {
        qDebug() << "Ошибка получения данных из базы.";
        return;
    }

    m_dbManager.removeFavoriteMovie(user_id, movie_id);

}

