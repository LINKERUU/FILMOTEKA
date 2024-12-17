#ifndef FAVORITESLIST_H
#define FAVORITESLIST_H

#include "ListMovies.h"

class FavoritesList : public ListMovie {
    Q_OBJECT
public:
    FavoritesList(DatabaseManager& dbManager, QWidget* parent = nullptr);
    void loadMovies(bool flag);

private:

    void setupFavorites();

    void on_exitbutton_click();

    void setupUI();

    void resizeEvent(QResizeEvent* event);
};

#endif // FAVORITESLIST_H
