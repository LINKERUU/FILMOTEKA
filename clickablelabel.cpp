#include "clickablelabel.h"


ClickableLabel::ClickableLabel(const QString& text, QWidget* parent, Qt::WindowFlags f)
    : QLabel(text, parent) {

}

ClickableLabel::~ClickableLabel() {}

void ClickableLabel::mousePressEvent(QMouseEvent* event) {
    emit clicked();
}
