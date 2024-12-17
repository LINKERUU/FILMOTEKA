#include "exceptionhandler.h"
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QMessageBox>
#include <QDate>
#include <QIntValidator>
#include <QDoubleValidator>

ExceptionHandler::ExceptionHandler(QObject* parent) : QStyledItemDelegate(parent) {}

QWidget* ExceptionHandler::createEditor(QWidget* parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    // В зависимости от столбца создаем соответствующий редактор
    if (index.column() == 1) { // Например, для "Название" используем QLineEdit
        QLineEdit *editor = new QLineEdit(parent);
        return editor;
    } else if (index.column() == 2) { // Для "Год" используем QLineEdit
        QLineEdit *editor = new QLineEdit(parent);
        editor->setValidator(new QIntValidator(1900, 9999, parent));
        return editor;
    } else if (index.column() == 4) { // Для "Рейтинг" используем QDoubleSpinBox
        QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
        editor->setRange(0.0, 99.0);
        return editor;
    } else if (index.column() == 7) { // Для "Длительность" используем QLineEdit
        QLineEdit *editor = new QLineEdit(parent);
        editor->setValidator(new QIntValidator(1, 9999, parent));  // Положительные значения
        return editor;
    }

    // Для других столбцов используем стандартный редактор
    return QStyledItemDelegate::createEditor(parent, option, index);
}

void ExceptionHandler::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
    QString value;

    // Определяем тип редактора и получаем из него значение
    if (auto lineEdit = dynamic_cast<QLineEdit*>(editor)) {
        value = lineEdit->text().trimmed();
    } else if (auto doubleSpinBox = dynamic_cast<QDoubleSpinBox*>(editor)) {
        value = QString::number(doubleSpinBox->value());
    }

    try {
        // Проверка на основании столбца
        switch (index.column()) {
        case 1: // Название
            if (value.isEmpty()) {
                throw QString("Название не может быть пустым.");
            }
            break;
        case 2: // Год
        {
            int year = value.toInt();
            if (year < 1900 || year > QDate::currentDate().year()) {
                throw QString("Год должен быть между 1900 и текущим годом.");
            }
        }
        break;
        case 4: // Рейтинг
        {
            double rating = value.toDouble();
            if (rating < 0.0 || rating > 10.0) {
                throw QString("Рейтинг должен быть в диапазоне от 0 до 10.");
            }
        }
        break;
        case 7: // Длительность
        {
            int duration = value.toInt();
            if (duration <= 0 || duration >=3000) {
                throw QString("Длительность должна быть больше 0 и меньше 3000.");
            }
        }
        break;
        default:
            break; // Для других столбцов проверка не требуется
        }

        // Если проверка прошла успешно, передаем данные в модель
        if (!model->setData(index, value)) {
            throw QString("Не удалось сохранить данные.");
        }
    } catch (const QString& err) {
        // В случае ошибки выводим сообщение
        QMessageBox::critical(nullptr, "Ошибка ввода", err);

        // Возвращаем редактор в исходное состояние, если произошла ошибка
        if (auto lineEdit = dynamic_cast<QLineEdit*>(editor)) {
            lineEdit->setText(model->data(index).toString());
        } else if (auto doubleSpinBox = dynamic_cast<QDoubleSpinBox*>(editor)) {
            doubleSpinBox->setValue(model->data(index).toDouble());
        }
    } catch (...) {
        QMessageBox::critical(nullptr, "Ошибка", "Произошла неизвестная ошибка.");
    }
}

void ExceptionHandler::handleException(const QString& errMessage, QWidget* parent) {
    QMessageBox::critical(parent, "Ошибка", errMessage);
}

void ExceptionHandler::handleUnknownException(QWidget* parent) {
    QMessageBox::critical(parent, "Ошибка", "Произошла необработанное исключение.");
}
