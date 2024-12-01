#ifndef ADMIN_H
#define ADMIN_H

#include <QDialog>
#include <QStyledItemDelegate>
#include <QFileDialog>
#include <QSqlTableModel>
#include "authentication.h"
#include "exceptionhandler.h"
#include "noneditabledelegate.h"


namespace Ui {
class Admin;
}

class Admin : public QDialog
{

public:
    Admin(DatabaseManager& dbManager, QWidget *parent = nullptr);
    QByteArray readVideoFile(const QString& filePath);
    void updateTable();
    ~Admin();

private slots:
    void on_remove_button_clicked();

    void on_pick_poster_clicked();

    void on_pick_movie_clicked();

    void on_addmovie_button_clicked();

    void on_quit_button_clicked();

private:
    Ui::Admin *ui;
    DatabaseManager& m_dbManager;
};

#endif // ADMIN_H
