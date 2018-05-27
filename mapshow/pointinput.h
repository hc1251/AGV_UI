#ifndef POINTINPUT_H
#define POINTINPUT_H

#include <QDialog>
#include <QWidget>
#include <QPoint>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include "database/ConnectionPool.h"

namespace Ui {
class PointInput;
}

class PointInput : public QDialog
{
    Q_OBJECT

public:
    explicit PointInput(QWidget *parent = 0);
    ~PointInput();

    void setPoint(QPoint tempPoint);
    int x;
    int y;
    QString card;

private:
    QPoint nowPoint;

private slots:
    void on_insertButton_clicked();

    void on_deleteButton_clicked();

signals:
    void deletePoint(QString);
    void addPoint();

private:
    Ui::PointInput *ui;
};

#endif // POINTINPUT_H
