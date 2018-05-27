#ifndef READCARDWINDOW_H
#define READCARDWINDOW_H

#include <QDialog>
#include <QSqlTableModel>

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QStandardItemModel>

#include <QDateTime>

namespace Ui {
class ReadCardWindow;
}

class ReadCardWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ReadCardWindow(QWidget *parent = 0);
    ~ReadCardWindow();

private:
    Ui::ReadCardWindow *ui;

};

#endif // READCARDWINDOW_H
