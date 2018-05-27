#ifndef CROSSINFOWINDOW_H
#define CROSSINFOWINDOW_H

#include <QWidget>

namespace Ui {
class CrossInfoWindow;
}

class CrossInfoWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CrossInfoWindow(QWidget *parent = 0);
    ~CrossInfoWindow();

private slots:
    void on_clearBtn_clicked();

    void on_quitBtn_clicked();

    void on_crossInfoTableWidget_activated(const QModelIndex &index);

private:
    Ui::CrossInfoWindow *ui;
};

#endif // CROSSINFOWINDOW_H
