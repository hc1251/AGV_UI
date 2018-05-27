#ifndef STATIONCARGOINFOWINDOW_H
#define STATIONCARGOINFOWINDOW_H

#include <QDialog>


namespace Ui {
class StationCargoInfoWindow;
}

class StationCargoInfoWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StationCargoInfoWindow(QWidget *parent = 0);
    ~StationCargoInfoWindow();

private:
    Ui::StationCargoInfoWindow *ui;
private slots:
    void slotTableWidgetClicked(int row, int columm);
    void on_modifyBtn_clicked();
    void on_cancelBtn_clicked();
public:
    int rowNumber;
};

#endif // STATIONCARGOINFOWINDOW_H
