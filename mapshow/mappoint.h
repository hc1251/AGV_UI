#ifndef MAPPOINT_H
#define MAPPOINT_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <QDebug>
#include <QSqlQuery>
#include "database/ConnectionPool.h"
extern bool showMapPoint ;

class MapPoint : public QWidget
{
    Q_OBJECT
public:
    explicit MapPoint(QWidget *parent = 0);
    int cardNum;
    int x,y;

private:
    void paintEvent(QPaintEvent *);

public slots:
    void slotPointShow();
};

#endif // MAPPOINT_H
