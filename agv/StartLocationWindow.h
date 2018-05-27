#ifndef STARTLOCATIONWINDOW_H
#define STARTLOCATIONWINDOW_H

#include <QWidget>

namespace Ui {
class StartLocationWindow;
}

class StartLocationWindow : public QWidget
{
    Q_OBJECT

public:
    explicit StartLocationWindow(QWidget *parent = 0);
    ~StartLocationWindow();

private:
    Ui::StartLocationWindow *ui;


public:
     int rowNumber;
private slots:
     void slotTableWidgetClicked(int row, int columm);
     void on_modifyBtn_clicked();
     void on_cancelBtn_clicked();
};

#endif // STARTLOCATIONWINDOW_H
