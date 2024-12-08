#ifndef LISTMOVIES_H
#define LISTMOVIES_H

#include "movie.h"
#include "authentication.h"
#include "moviewidget.h"
#include <QResizeEvent>
#include <QScrollArea>
#include <QMainWindow>

class ListMovie : public QMainWindow {
public:
    ListMovie(DatabaseManager& dbManager, QWidget* parent = nullptr);

protected:
    void updateMovieDisplay(Stack<Movie>& movies,bool update);

    DatabaseManager& m_dbManager;

    void configureGridLayout( QGridLayout* gridLayout);

    void clearGridLayout(QGridLayout* gridLayout);

private:

    void setupUI();

    void loadMovies(bool flag);

    void resizeEvent(QResizeEvent *event);

    void on_exitbutton_click();

    void search_movie(const QString& text);

    void openFavoriteMovies();

};

#endif // LISTMOVIES_H


