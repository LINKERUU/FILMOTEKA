#include "moviewidget.h"


MovieWidget::MovieWidget(const QString& title, const QString& year, const QString& genre, double rating,
                         const QByteArray& posterData, DatabaseManager& dbManager, QWidget* parent)
    : QWidget(parent), m_dbManager(dbManager) { // Инициализация m_dbManager
    setupUI(title, year, genre, rating, posterData);
}

void MovieWidget::setupUI(const QString& title, const QString& year, const QString& genre, double rating, const QByteArray& posterData) {
    auto* mainLayout = createMainLayout();
    setupPoster(mainLayout, posterData,title);
    setupRatingAndLikeButtons(mainLayout, rating);
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

QLabel* MovieWidget::createImageLabel(const QByteArray& posterData,QString title) {
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


void MovieWidget::setupRatingAndLikeButtons(QVBoxLayout* mainLayout, double rating) {

    auto* ratingLikeLayout = createRatingLikeLayout(rating);
    mainLayout->addLayout(ratingLikeLayout);
}

QHBoxLayout* MovieWidget::createRatingLikeLayout(double rating) {
    auto* ratingLikeLayout = new QHBoxLayout();
    ratingLikeLayout->setContentsMargins(10, 0, 60, 0);

    auto* ratingLabel = createRatingLabel(rating);
    auto* likeButton = createLikeButton();
    connect(likeButton, &QPushButton::clicked, this, &MovieWidget::toggleLikeButton);

    ratingLikeLayout->addWidget(likeButton);
    ratingLikeLayout->addWidget(ratingLabel);
    ratingLikeLayout->setSpacing(100);
    ratingLikeLayout->addStretch();
    return ratingLikeLayout;
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

QPushButton* MovieWidget::createLikeButton() {
    QPushButton* likeButton = new QPushButton("♥");
    QFont font = likeButton->font();
    likeButton->setFixedSize(30, 30);
    likeButton->setFont(font);
    likeButton->setStyleSheet("QPushButton { color:white;border-radius:60px;font-size:36px;}"
                              "QPushButton:hover {color:gray;} ");
    return likeButton;
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
                                       film.poster(), film.description(), film.duraction(), film.director(), film.movie());
    // film_window = new VideoPlayer();
    film_window->show(); // Показать новое окно

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

void MovieWidget::toggleLikeButton() {

    auto* likeButton = findChild<QPushButton*>();
    if (m_isLiked) {
        likeButton->setStyleSheet("QPushButton { color:red;border-radius:60px;font-size:36px;}");
    } else {
        likeButton->setStyleSheet("QPushButton { color:white;border-radius:60px;font-size:36px;}"
                                  "QPushButton:hover { color:gray;} ");
    }
    m_isLiked = !m_isLiked;
}
