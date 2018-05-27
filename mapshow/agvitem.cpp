#include "agvitem.h"

AgvItem::AgvItem(QWidget *parent,int num) :
    QWidget(parent),stationX(0),stationY(0),cardDirection(0),agvState(0),agvErrorInfo(0)
{

    id = num;
    effect = new QGraphicsOpacityEffect(this);
    opacity = 1.0;
    lastOpacity = 1.0;
    effect->setOpacity(opacity);
    this->setGraphicsEffect(effect);
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&AgvItem::timeout);//闪烁
    timer->start(200);
}

void AgvItem::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QRectF target;
    QRectF source;
    QImage image;
    //尺寸小
  /* if(agvState==0)//agv离线，显示灰颜色
    {
        if(cardDirection==0)
        {
        image.load (":/images/0AGV.png");
        target.setRect(0, 8, sWidth, sHeight/2+4);
        }
        else
        {
            image.load (":/images/1AGV.png");
            target.setRect(8, 0, sWidth/2+4, sHeight);

        }
    }
    else{
        if(agvErrorInfo==0)//agv没有错误，显示正常颜色
        {
            if(cardDirection==0)
            {
                image.load (QString(":/images/AGV_%1.png").arg (id));
                target.setRect(0, 8, sWidth, sHeight/2+4);
            }
            else{
                image.load (QString(":/images/AGV0_%1.png").arg (id));
                target.setRect(8, 0, sWidth/2+4, sHeight);
            }
        }
        else//agv运行出错，显示红颜色
        {
            if(cardDirection==0)
            {
                image.load (":/images/AGV0.png");
                target.setRect(0, 8, sWidth, sHeight/2+4);
            }
            else{
                image.load (":/images/AGV1.png");
                //target.setRect(8, 0, sWidth/2+4, sHeight);
                target.setRect(15, 0, sWidth, sHeight*2);
            }
        }
    }*/
    if(agvState==0)//agv离线，显示灰颜色
      {
          if(cardDirection==0)
          {
          image.load (":/images/0AGV.png");
         target.setRect(0, 15, sWidth*2, sHeight);
          }
          else
          {
              image.load (":/images/1AGV.png");
               target.setRect(15, 0, sWidth, sHeight*2);

          }
      }
      else{
          if(agvErrorInfo==0)//agv没有错误，显示正常颜色
          {
              if(cardDirection==0)
              {
                  image.load (QString(":/images/AGV_%1.png").arg (id));
                  target.setRect(0, 15, sWidth*2, sHeight);
              }
              else{
                  image.load (QString(":/images/AGV0_%1.png").arg (id));
                  target.setRect(15, 0, sWidth, sHeight*2);
              }
          }
          else//agv运行出错，显示红颜色
          {
              if(cardDirection==0)
              {
                  image.load (":/images/AGV0.png");
              //    target.setRect(0, 8, sWidth, sHeight/2+4);
                  target.setRect(0, 15, sWidth*2, sHeight);
              }
              else{
                  image.load (":/images/AGV1.png");
                  target.setRect(15, 0, sWidth, sHeight*2);
              }
          }
      }
    source.setRect(0, 0, image.width(),image.height());

   //   source.setRect(0, 0, image.width()*4,image.height()*4);
    //target:将源图像文件中source位置的图像画到目标绘图设备中target位置上，大小自适应
    painter.drawImage(target, image, source);
}

void AgvItem::timeout()
{
  /*  if((uint)(opacity*10) == 10)
        lastOpacity = 1.0;
    else if((uint)(opacity*10) == 5)
        lastOpacity = 0.5;
    if((uint)(lastOpacity*10) == 10)
        opacity -= 0.1;
    else if((uint)(lastOpacity*10) == 5)
        opacity += 0.1;
    effect->setOpacity(opacity);
    this->setGraphicsEffect(effect);
    this->update();*/
}


