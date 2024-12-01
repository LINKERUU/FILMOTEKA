#include "exceptionhandler.h"

ExceptionHandler::ExceptionHandler(QObject* parent) : QStyledItemDelegate(parent) {}


void ExceptionHandler::setModelData(QWidget* editor, QAbstractItemModel* model,
                  const QModelIndex& index) {
    // Получение значения из редактора
    QString value;

    // Проверка, является ли редактор QLineEdit
    if (auto lineEdit = dynamic_cast<QLineEdit*>(editor)) {
        value = lineEdit->text().trimmed();
    }
    // Проверка, является ли редактор QDoubleSpinBox
    else if (auto doubleSpinBox = dynamic_cast<QDoubleSpinBox*>(editor)) {
        value = QString::number(doubleSpinBox->value());
    } else {
        throw QString("Неизвестный тип редактора");
    }

    // Проверка на корректность данных в зависимости от столбца
    try {
        switch (index.column()) {
        case 1: // Название фильма
            if (value.isEmpty()) {
                throw QString("Название фильма не может быть пустым.");
            }
            break;
        case 2: // Год выпуска
            if (value.toInt() < 1900 || value.toInt() > 2024) {
                throw QString("Введите корректный год");
            }
            break;
        case 4:
            if (value.toDouble() > 10 || value.toDouble() < 0) {
                throw QString("Рейтинг должен быть в диапазоне от 0 до 10.");
            }
            break;
        case 7: // Длительность
            if (value.toInt() <= 0) {
                throw QString("Длительность должна быть больше 0.");
            }
            break;
        default:
            break;
        }

        QStyledItemDelegate::setModelData(editor, model, index);

    } catch (const QString& err) {
        ExceptionHandler::handleException(err);
    } catch (...) {
        ExceptionHandler::handleUnknownException();
    }
}
void ExceptionHandler::handleException(const QString& errMessage, QWidget* parent ) {
    QMessageBox::critical(parent, "Ошибка", errMessage);
}

void ExceptionHandler::handleUnknownException(QWidget* parent ) {
    QMessageBox::critical(parent, "Ошибка", "Произошло необработанное исключение");
}
