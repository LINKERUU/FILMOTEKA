#include "NonEditableDelegate.h"

NonEditableDelegate::NonEditableDelegate(QObject* parent)
    : QStyledItemDelegate(parent) {}

QWidget* NonEditableDelegate::createEditor(QWidget*, const QStyleOptionViewItem&, const QModelIndex&) const {
    return nullptr;
}
