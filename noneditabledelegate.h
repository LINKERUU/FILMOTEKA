#ifndef NONEDITABLEDELEGATE_H
#define NONEDITABLEDELEGATE_H

#include <QStyledItemDelegate>

class NonEditableDelegate : public QStyledItemDelegate {
public:
    explicit NonEditableDelegate(QObject* parent = nullptr);

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

#endif // NONEDITABLEDELEGATE_H
