#ifndef COMMONUI_H
#define COMMONUI_H

#include <QWidget>

namespace Ui {
class CommonUI;
}

class CommonUI : public QWidget
{
    Q_OBJECT

public:
    explicit CommonUI(QWidget *parent = nullptr);
    ~CommonUI();

    Ui::CommonUI *ui;
};

#endif // COMMONUI_H
