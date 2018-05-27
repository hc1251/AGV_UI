#ifndef AGVUSEDWINDOW_H
#define AGVUSEDWINDOW_H

#include <QWidget>

namespace Ui {
class AgvUsedWindow;
}

class AgvUsedWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AgvUsedWindow(QWidget *parent = 0);
    ~AgvUsedWindow();

private:
    Ui::AgvUsedWindow *ui;
};

#endif // AGVUSEDWINDOW_H
