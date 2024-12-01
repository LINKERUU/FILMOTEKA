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
    DatabaseManager& m_dbManager;
private:
    void setupUI();

    void configureGridLayout( QGridLayout* gridLayout);

    void loadMovies(bool flag);

    void resizeEvent(QResizeEvent *event);

    void on_exitbutton_click();

    void search_movie(const QString& text);

    void updateMovieDisplay(Stack<Movie>& movies,bool update);

    void clearGridLayout(QGridLayout* gridLayout);

};

#endif // LISTMOVIES_H


