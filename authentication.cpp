#include "authentication.h"
#include "ui_authentication.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

Authentication::Authentication(DatabaseManager& dbManager, QWidget *parent)
    : m_dbManager(dbManager), QDialog(parent), ui(new Ui::Authentication)
{
    ui->setupUi(this);
    m_dbManager.open();
    setFixedSize(1111, 462);
    resetUI();
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

void Authentication::resetUI() {
    ui->error_label->setVisible(false);
    ui->cancel_button->setVisible(false);
    ui->show_password_button->setVisible(false);
    ui->password_linedit->setVisible(false);
    ui->enter_button->setVisible(false);
    ui->login_linedit->setVisible(false);
    ui->entrance_button->setVisible(true);
    ui->register_button->setVisible(true);
    ui->register_button->setText("Регистрация");
    ui->text_label->setText("Авторизация");
    ui->password_linedit->clear();
    ui->login_linedit->clear();
}

void Authentication::on_register_button_clicked() {
    if (ui->register_button->text() == "Войти") {
        handleLogin();
    }
    else {
        ui->cancel_button->setVisible(true);
        ui->password_linedit->setVisible(true);
        ui->login_linedit->setVisible(true);
        ui->enter_button->setVisible(true);
        ui->show_password_button->setVisible(true);
        ui->entrance_button->setVisible(false);
        ui->register_button->setVisible(false);

        setFixedSize(1111, 560);
        ui->widget->setFixedHeight(460);
        ui->show_password_button->setIcon(QIcon(":/icons/close_eye.png"));
        ui->text_label->setText("Регистрация");
        ui->enter_button->setText("Зарегистрироваться");
    }
}

void Authentication::on_cancel_button_clicked() {
    setFixedSize(1111, 462);
    ui->widget->setFixedHeight(400);
    resetUI();
}

void Authentication::on_show_password_button_clicked() {
    if (ui->password_linedit->echoMode() == QLineEdit::Password) {
        ui->password_linedit->setEchoMode(QLineEdit::Normal);
        ui->show_password_button->setIcon(QIcon(":/icons/open_eye.png"));
    }
    else {
        ui->password_linedit->setEchoMode(QLineEdit::Password);
        ui->show_password_button->setIcon(QIcon(":/icons/close_eye.png"));
    }
}

void Authentication::on_password_linedit_textChanged(const QString &) {
    ui->error_label->setVisible(false);
}

void Authentication::on_entrance_button_clicked() {
        ui->cancel_button->setVisible(true);
        ui->password_linedit->setVisible(true);
        ui->login_linedit->setVisible(true);
        ui->enter_button->setVisible(true);
        ui->show_password_button->setVisible(true);
        ui->entrance_button->setVisible(false);
        ui->register_button->setVisible(false);

        setFixedSize(1111, 560);
        ui->widget->setFixedHeight(460);
        ui->show_password_button->setIcon(QIcon(":/icons/close_eye.png"));
        ui->text_label->setText("Вход");
        ui->enter_button->setText("Войти");
}

void Authentication::on_enter_button_clicked() {
    if (ui->enter_button->text() == "Зарегистрироваться") {

        QString username = ui->login_linedit->text();
        QString password = ui->password_linedit->text();

        if (username.isEmpty() || password.isEmpty()) {
            ui->error_label->setText("Все поля должны быть заполнены!");
            ui->error_label->setVisible(true);
            return;
        }

        QString hashedPassword = hashPassword(password);

        if (!m_dbManager.insertUser(username, hashedPassword)) {
            ui->error_label->setText("Ошибка при регистрации пользователя!");
            ui->error_label->setVisible(true);
        }
        else {
            QMessageBox::information(this, "Успех", "Пользователь успешно зарегистрирован!");
            resetUI();
        }
    }
    if(ui->enter_button->text() == "Войти"){
        handleLogin();
    }
}

void Authentication::handleLogin() {
    QString enteredPasswordHash = hashPassword(ui->password_linedit->text());
    QString storedPasswordHash = m_dbManager.login(ui->login_linedit->text());
    QString adminPasswordHash = m_dbManager.login("admin");


    if (enteredPasswordHash == storedPasswordHash && ui->login_linedit->text()!="admin") {
        ListMovie* catalogmovie = new ListMovie(m_dbManager);
        catalogmovie->show();
        this->close();
    }
    else if (enteredPasswordHash == adminPasswordHash){
        this->close();
        Admin* admin_panel = new Admin(m_dbManager);
        admin_panel->exec();

    }
    else {
        ui->password_linedit->clear();
        ui->error_label->setText("Неверное имя пользователя или пароль!");
        ui->error_label->setVisible(true);
    }
}
