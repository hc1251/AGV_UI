#ifndef TMCMODIFYWINDOW_H
#define TMCMODIFYWINDOW_H

#include <QWidget>

namespace Ui {
class TmcModifyWindow;
}

class TmcModifyWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TmcModifyWindow(QWidget *parent = 0);
    ~TmcModifyWindow();
    int rowNumber;
private slots:
    void slotTableWidgetClicked(int,int);
    void on_deteleBtn_clicked();

    void on_insertBtn_clicked();

    void on_quitBtn_clicked();

private:
    Ui::TmcModifyWindow *ui;
};

#endif // TMCMODIFYWINDOW_H
