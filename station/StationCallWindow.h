#ifndef STATIONCALLWINDOW_H
#define STATIONCALLWINDOW_H

#include <QDialog>

namespace Ui {
class StationCallWindow;
}

class StationCallWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StationCallWindow(QWidget *parent = 0);
    ~StationCallWindow();

private:
    Ui::StationCallWindow *ui;
};

#endif // STATIONCALLWINDOW_H
