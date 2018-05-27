#include "mappoint.h"

bool showMapPoint = false;

MapPoint::MapPoint(QWidget *parent) :
    QWidget(parent)
{
}

void MapPoint::paintEvent(QPaintEvent *)
{
    if(!showMapPoint)//显示卡片
   // if(showMapPoint)
    {
        QPainter painter(this);
        QPen pen;
        painter.fillRect(0,0,8,8,QColor(0,0,255));//
        pen.setColor(QColor(255,0,255));//
        pen.setStyle (Qt::DashDotLine);
        painter.setPen(pen);
        painter.drawText(0,this->height()-12,QString::number(cardNum));
    }
}

void MapPoint::slotPointShow()
{
    this->update();
}
