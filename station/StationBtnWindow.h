#ifndef STATIONBTNWINDOW_H
#define STATIONBTNWINDOW_H

#include <QWidget>
#include <QButtonGroup>

namespace Ui {
class StationBtnWindow;
}

class StationBtnWindow : public QWidget
{
    Q_OBJECT

public:
    explicit StationBtnWindow(QWidget *parent = 0);
    ~StationBtnWindow();

private:
    Ui::StationBtnWindow *ui;
public:
    QButtonGroup *stationBtnGroup;
 signals:
   void buttonClicked(int);
private slots:
    void on_pushButton_clicked(int);
    void on_pushButton_2_clicked();

signals:
    void sendSelectStationInfo(QString);//传递选择的工位信息
};

#endif // STATIONBTNWINDOW_H
