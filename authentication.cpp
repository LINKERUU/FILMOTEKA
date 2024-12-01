#include "authentication.h"
#include "ui_authentication.h"


Authentication::Authentication(DatabaseManager& dbManager, QWidget *parent)
    : m_dbManager(dbManager), QDialog(parent), ui(new Ui::Authentication)
{
    ui->setupUi(this);

    setFixedSize(1111, 462);

    ui->error_label->setVisible(false);
    ui->cancel_button->setVisible(false);
    ui->show_password_button->setVisible(false);
    ui->password_linedit->setVisible(false);
}

Authentication::~Authentication()
{
    delete ui;
}

QString Authentication::hashPassword(const QString &password) {
    QByteArray passwordBytes = password.toUtf8();
    QByteArray hashed = QCryptographicHash::hash(passwordBytes, QCryptographicHash::Sha256);
    return QString(hashed.toHex());
}

void Authentication::on_admin_button_clicked()
{
    if (ui->admin_button->text() == "Войти") {
        QString enteredPasswordHash = hashPassword(ui->password_linedit->text());
        QString storedPasswordHash = "994c5e2c8b5fe4e7c123d39b74bcc870e7a91b7cd15412745ba085e6d1eb3f0f";

        if (enteredPasswordHash == storedPasswordHash) {
            Admin *adminWindow = new Admin(m_dbManager);
            this->close();
            adminWindow->exec();
        } else {
            ui->password_linedit->clear();
            ui->error_label->setText("Неверный пароль!");
            ui->error_label->setVisible(true);
        }
    } else {
        ui->cancel_button->setVisible(true);
        ui->password_linedit->setVisible(true);
        ui->user_button->setVisible(false);
        ui->show_password_button->setVisible(true);
        ui->show_password_button->setIcon(QIcon(":/icons/close_eye.png"));
        ui->text_label->setText("Администратор");
        ui->admin_button->setText("Войти");
    }

}

void Authentication::on_cancel_button_clicked()
{
    ui->cancel_button->setVisible(false);
    ui->show_password_button->setVisible(false);
    ui->password_linedit->setVisible(false);
    ui->user_button->setVisible(true);
    ui->admin_button->setText("Администратор");
    ui->text_label->setText("Авторизация");
    ui->error_label->setVisible(false);
}

void Authentication::on_show_password_button_clicked()
{
    if (ui->password_linedit->echoMode() == QLineEdit::Password) {
        ui->show_password_button->setIcon(QIcon(":/icons/open_eye.png"));
        ui->password_linedit->setEchoMode(QLineEdit::Normal);
    } else {
        ui->show_password_button->setIcon(QIcon(":/icons/close_eye.png"));
        ui->password_linedit->setEchoMode(QLineEdit::Password);
    }
}

void Authentication::on_password_linedit_textChanged(const QString &arg1)
{
    ui->error_label->setVisible(false);
}

void Authentication::on_user_button_clicked()
{
    ListMovie* catalogmovie = new ListMovie(m_dbManager);
    catalogmovie->show();
    this->close();
}
