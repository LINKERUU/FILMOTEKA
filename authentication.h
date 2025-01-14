#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <QDialog>
#include <QCryptographicHash>
#include <QApplication>
#include <QFile>
#include "SqlDatabase.h"
#include "admin.h"
#include "ListMovies.h"

namespace Ui {
class Authentication;
}

class Authentication : public QDialog
{
    Q_OBJECT

public:
    explicit Authentication(DatabaseManager& dbManager, QWidget *parent = nullptr);
    ~Authentication();

private slots:
    void on_register_button_clicked();
    void on_cancel_button_clicked();
    void on_show_password_button_clicked();
    void on_password_linedit_textChanged(const QString &arg1);
    void on_entrance_button_clicked();
    QString hashPassword(const QString &password);
    void resetUI();
    void handleLogin();
    void on_enter_button_clicked();

private:
    Ui::Authentication *ui;

    DatabaseManager& m_dbManager;
};

#endif // AUTHENTICATION_H
