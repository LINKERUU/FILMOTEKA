#ifndef EXCEPTIONHANDLER_H
#define EXCEPTIONHANDLER_H

#include <QString>
#include <QMessageBox>
#include <QWidget>
#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QIntValidator>
#include <QDoubleSpinBox>



class ExceptionHandler : public QStyledItemDelegate {
    Q_OBJECT

public:

    ExceptionHandler(QObject* parent = nullptr);


    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) ;

    static void handleException(const QString& errMessage, QWidget* parent = nullptr);

    static void handleUnknownException(QWidget* parent = nullptr);
};


#endif // EXCEPTIONHANDLER_H
