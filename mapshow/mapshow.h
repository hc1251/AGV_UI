#ifndef MapShow_H
#define MapShow_H

#include <QWidget>
#include <QPaintEvent>
#include <QImage>
#include <QWheelEvent>
#include <QDebug>
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#include <QList>
#include <QTimer>
#include <QHostAddress>
#include "agvitem.h"
#include "pointinput.h"
#include "mappoint.h"
#include "database/ConnectionPool.h"
#include <QTime>
#define QT_AUTO_SCREEN_SCALE_FACTOR
class AgvItem;
class MapShow : public QWidget
{
    Q_OBJECT

public:
    explicit MapShow(QWidget *parent = 0);
    ~MapShow();
    void runUpdate(bool open); // 打开刷新agv功能

public slots:
    //void slotUpdateMapShowAgvLocation(const quint8& agvNum,const quint16& cardNum,const quint16& x,const quint16& y);
    void slotUpdateMapShowAgvLocation(const quint8& agvNum,const quint16& x,const quint16& y,const quint16& cardDirection,const quint16& state,const quint16& agvErrorInfo);
private:
    void paintEvent(QPaintEvent *);
    void wheelEvent(QWheelEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    //发布版本注销掉双击按钮***
    void mouseDoubleClickEvent(QMouseEvent*);
    //******//
    void keyPressEvent(QKeyEvent*);
    void initPoint();
    void updatePoint();

private:
     AgvItem *agvitem;
    QImage image;
    float imageWidth; //当前窗口显示图像的像素宽度
    float imageHeight;//当前窗口显示图像的像素高度
    float touchX;
    float touchY;
    QPoint pressPoint;
    QPoint movePoint;
    QPoint mousePoint; //当前鼠标的位置
    bool press; //是否按下
    float multiple; //放大倍数
    QList<AgvItem*> carList;
    QList<MapPoint*> point; //卡点集合
    PointInput* InputWin;//输入坐标窗口
    int currentChooseCar;

private slots:
    void deletePointSlot(QString card);//删除点
    void addPointSlot();
    void slotCarPressed(int carNum);

signals:
    void closePointShow();
    void sendagvnum(int agvnum);
public:
    int agvnumm;

};

#endif // MapShow_H
