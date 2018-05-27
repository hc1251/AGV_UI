#ifndef AgvItem_H
#define AgvItem_H

#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QDebug>
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include "database/ConnectionPool.h"



class AgvItem : public QWidget
{
    Q_OBJECT
public:
    explicit AgvItem(QWidget *parent = 0, int num = 0);

private:
    void paintEvent(QPaintEvent *);
public:
    int stationX; //地图显示坐标
    int stationY;
    int id;     //小车编号
    int cardDirection;//卡的方向
    int agvState;//状态 0：离线   1：在线
    int agvErrorInfo;//agv运行是否出错
    int card;  //卡点
    int sWidth; //显示宽度
    int sHeight;  //显示高度

private:
    QTimer *timer;
    bool enter;
    double opacity;
    double lastOpacity;
    QGraphicsOpacityEffect* effect;

private slots:
    void timeout();
signals:
    void sigCarPressed(int carNum);
};

#endif // AgvItem_H
