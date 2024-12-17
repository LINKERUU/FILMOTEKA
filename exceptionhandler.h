#ifndef EXCEPTIONHANDLER_H
#define EXCEPTIONHANDLER_H

#include <QString>
#include <QMessageBox>
#include <QWidget>
#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QDate>
#include <QIntValidator>
#include <QDoubleSpinBox>



class ExceptionHandler : public QStyledItemDelegate {
    Q_OBJECT

public:

    ExceptionHandler(QObject* parent = nullptr);

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem &option, const QModelIndex &index)const;

    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

    static void handleException(const QString& errMessage, QWidget* parent = nullptr);

    static void handleUnknownException(QWidget* parent = nullptr);
};


#endif // EXCEPTIONHANDLER_H
