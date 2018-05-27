#ifndef STARTLOCATION_H
#define STARTLOCATION_H

#include <QWidget>

namespace Ui {
class StartLocation;
}

class StartLocation : public QWidget
{
    Q_OBJECT

public:
    explicit StartLocation(QWidget *parent = 0);
    ~StartLocation();

private:
    Ui::StartLocation *ui;
};

#endif // STARTLOCATION_H
