#include "mapshow.h"

MapShow::MapShow(QWidget *parent) :
    QWidget(parent)
{
    if(!image.load("D:/resource/map.png"))
    {
        QMessageBox::information(this,QStringLiteral("提示"),"load map failed!!");
        exit(0);
    }
    imageWidth = image.width();
    imageHeight = image.height();
    touchX = 0;
    touchY = 0;
    press = false;
    multiple = 1;

   InputWin = new PointInput(this);
    connect(InputWin,SIGNAL(deletePoint(QString)),this,SLOT(deletePointSlot(QString)));
    connect(InputWin,SIGNAL(addPoint()),this,SLOT(addPointSlot()));

   InputWin->hide();

    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlQuery query(db);
    query.exec("select * from AgvMessage");
    int num = 1;
    while(query.next())
    {
        AgvItem* tempcar = new AgvItem(this,query.value("agvNum").toInt());
        tempcar->resize(60,60);//显示小车图像区域
        tempcar->stationX = this->width() - 100;
        tempcar->stationY = 32*num + 20;
        num++;
        tempcar->sWidth = 30;
        tempcar->sHeight = 30;
        tempcar->card = query.value("agvCurrentCard").toInt();
        tempcar->id = query.value("agvNum").toInt();
        connect(tempcar,&AgvItem::sigCarPressed,this,&MapShow::slotCarPressed);
        tempcar->show();
        tempcar->raise();
        carList.append(tempcar);
    }
    initPoint();
    currentChooseCar = 0;
    Singleton<ConnectionPool>::getInstance().releaseConnection(db);
}

MapShow::~MapShow()
{

}

void MapShow::paintEvent(QPaintEvent *)
{
    if( touchX < 0)
    {
        touchX = 0;
    }
    else if( touchX > (image.width() - imageWidth))
    {
        touchX = image.width() - imageWidth;
    }

    if( touchY < 0)
    {
        touchY = 0;
    }
    else if( touchY > (image.height() - imageHeight))
    {
        touchY = image.height() - imageHeight;
    }

    QPainter painter(this);
    QRectF target(0.0, 0.0,this->width(),this->height());
    //QRectF target(0.0, 0.0,imageWidth,imageHeight);
    QRectF source(touchX, touchY, imageWidth, imageHeight);
    QPen pen;
    pen.setColor(QColor(255,255,255));
    painter.setPen(pen);

   painter.drawImage(target, image, source);

   // QString x,y;
   // x.setNum(mousePoint.x());
   // y.setNum(mousePoint.y());
  //  painter.drawText(this->width()-170,20,"当前坐标" + x + ","+y);

    for(int i =0 ; i < carList.length() ;i++)
    {
        if(carList.at(i)->stationX > touchX && carList.at(i)->stationX < (imageWidth+touchX))
        {
            if(carList.at(i)->stationY > touchY && carList.at(i)->stationY < (imageHeight+touchY))
            {
                carList.at(i)->show();
                float pixelX = (float)this->width()/(float)imageWidth;
                float showX = carList.at(i)->stationX - touchX;
                showX *= pixelX;
                float pixelY = (float)this->height()/(float)imageHeight;
                float showY = carList.at(i)->stationY - touchY;
                showY *= pixelY;
                carList.at(i)->move(showX - carList.at(i)->width()/2,showY - carList.at(i)->height()/2);
             //   qDebug()<<carList.at(i)->id;
            }
            else
            {
                carList.at(i)->hide();//注释掉小车不会随着窗口的改变而变换位置
            }
        }
        else
        {
            carList.at(i)->hide();//小车闪烁程序
        }
    }
    updatePoint();
}

void MapShow::wheelEvent(QWheelEvent *e)
{
    if(e->delta() > 0)
    {
        if(multiple < 20)
        {
            multiple += 0.1;
        }
    }
    else
    {
        if(multiple > 1.1)
        {
            multiple -= 0.1;
        }
        else
        {
            touchX = 0;
            touchY = 0;
        }
    }
    imageWidth = (float)image.width()/multiple;
    imageHeight = (float)image.height()/multiple;
    this->update();
}

void MapShow::mousePressEvent(QMouseEvent *e)
{
    movePoint = e->pos();
    pressPoint.setX(touchX);
    pressPoint.setY(touchY);
    float pixelX = (float)imageWidth/(float)this->width();
    float showX = (float)e->x()*pixelX;
    showX += touchX;
    float pixelY = (float)imageHeight/(float)this->height();
    float showY = (float)e->y()*pixelY;
    showY += touchY;
    mousePoint.setX(showX);
    mousePoint.setY(showY);
    if(e->button() == Qt::RightButton)
    {
        press = true;
        this->setCursor(Qt::OpenHandCursor);
    }
    else if(e->button() == Qt::LeftButton)
    {
        this->setFocus();
        return;
    }
    this->update();
}

void MapShow::mouseMoveEvent(QMouseEvent *e)
{
    if(press)
    {
        touchX = (pressPoint.x() + (movePoint.x() - e->pos().x()));
        touchY = (pressPoint.y() + (movePoint.y() - e->pos().y()));
        if( touchX < 0)
        {
            touchX = 0;
        }
        else if( touchX > (image.width() - imageWidth))
        {
            touchX = image.width() - imageWidth;
        }

        if( touchY < 0)
        {
            touchY = 0;
        }
        else if( touchY > (image.height() - imageHeight))
        {
            touchY = image.height() - imageHeight;
        }
        this->setCursor(Qt::ClosedHandCursor);
        this->update();
    }

    float pixelX = (float)imageWidth/(float)this->width();
    float showX = (float)e->x()*pixelX;
    showX += touchX;

    float pixelY = (float)imageHeight/(float)this->height();
    float showY = (float)e->y()*pixelY;
    showY += touchY;

    mousePoint.setX(showX);
    mousePoint.setY(showY);

    this->update();
}

void MapShow::mouseReleaseEvent(QMouseEvent *)
{
    press = false;
    this->setCursor(Qt::ArrowCursor);
    updatePoint();
}

void MapShow::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        InputWin->move(e->x(),e->y());
        InputWin->setPoint(mousePoint);
        InputWin->show();
    }
}

void MapShow::keyPressEvent(QKeyEvent *ke)
{
    if(ke->key() == Qt::Key_F1)
    {
        showMapPoint = !showMapPoint;
        emit closePointShow();
    }
    if(ke->key() == Qt::Key_0)
    {
        showMapPoint = !showMapPoint;
        emit closePointShow();
    }
    update();
}

void MapShow::slotUpdateMapShowAgvLocation(const quint8 &agvNum, const quint16 &x, const quint16 &y,const quint16& cardDirection,const quint16& state,const quint16& agvErrorInfo)
{
   for(int i = 0;i < carList.length(); ++i){
       if(carList.at(i)->id == agvNum){
           agvnumm=agvNum;
          // carList.at(i)->card = cardNum;//屏蔽这个
           carList.at(i)->stationX = x;
           carList.at(i)->stationY = y;
           carList.at(i)->cardDirection=cardDirection;
           carList.at (i)->agvState=state;
           carList.at (i)->agvErrorInfo=agvErrorInfo;
           break;
       }
   }
   this->update();
}


void MapShow::initPoint()
{
    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlQuery query(db);
    query.exec("select * from mapPoint");
    while(query.next())
    {
        MapPoint* tempPoint = new MapPoint(this);
        tempPoint->cardNum = query.value("cardPoint").toInt();
        tempPoint->x = query.value("x").toUInt();
        tempPoint->y = query.value("y").toUInt();
        connect(this,&MapShow::closePointShow,tempPoint,&MapPoint::slotPointShow);
        point.append(tempPoint);
    }
   Singleton<ConnectionPool>::getInstance().releaseConnection(db);
}

void MapShow::updatePoint()
{
    for(int i =0 ; i < point.length() ;i++)
    {
        if(point.at(i)->x > touchX && point.at(i)->x < (imageWidth+touchX))
        {
            if(point.at(i)->y > touchY && point.at(i)->y < (imageHeight+touchY))
            {
                point.at(i)->show();
                float pixelX = (float)this->width()/(float)imageWidth;
                float showX = point.at(i)->x - touchX;
                showX *= pixelX;

                float pixelY = (float)this->height()/(float)imageHeight;
                float showY = point.at(i)->y - touchY;
                showY *= pixelY;
                point.at(i)->move(showX,showY);
            }
            else
            {
                point.at(i)->hide();
            }
        }
        else
        {
            point.at(i)->hide();
        }
    }
}

void MapShow::deletePointSlot(QString card)
{
    for(int i = 0; i < point.length();i++)
    {
        if(point.at(i)->cardNum == card.toInt())
        {

            delete point.at(i);
            point.removeAt(i);
        }
    }
    updatePoint();
}

void MapShow::addPointSlot()
{
    MapPoint* tempPoint = new MapPoint(this);
    tempPoint->cardNum = InputWin->card.toInt();
    tempPoint->x = InputWin->x;
    tempPoint->y = InputWin->y;
    tempPoint->show();
    point.append(tempPoint);
    updatePoint();
}


void MapShow::slotCarPressed(int carNum)
{
    currentChooseCar = carNum;
}



