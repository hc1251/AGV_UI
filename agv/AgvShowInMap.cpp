#include "AgvShowInMap.h"
#include<QPoint>
#include <QSqlQuery>
#include<QDebug>
#include"database/ConnectionPool.h"
#include<qmath.h>
//#define int LINETHRED 10;

AgvShowInMap::AgvShowInMap(QObject *parent) : QObject(parent)
{
}
PosInfo *AgvShowInMap::agvPosition(int start){
    /*根据小车当前所处的卡号，找到小车的位置以及在该卡号小车的方向*/
   // posInfo=new PosInfo();
    //初始化
     posInfo=new PosInfo();
   // posInfo->agvPos->setX(0);
    //posInfo->agvPos->setY(0);
   // posInfo->direction=0;
    //posInfo=new PosInfo();
    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlQuery query(db);
    QPoint *startPosition =new QPoint(0,0);
    //  int radius = 80;//转弯半径,单位像素
    int startX, startY,direction;
    bool succ = query.exec (QString("select x,y,direction from mapPoint where cardPoint = '%1'").arg(start));
    if (succ&&query.first ()){
        int x = query.value("x").toInt();
        int y = query.value("y").toInt();
        direction=query.value ("direction").toInt ();//agv的方向，0代表水平，1代表竖直
        startX = x;
        startY = y;
        startPosition =new QPoint(x,y);
    }
    posInfo->agvPos=startPosition;
    posInfo->direction=direction;
    qDebug()<<QString("agvposition x:%1, y:%2").arg(startPosition->x()).arg(startPosition->y())<<direction;
    Singleton<ConnectionPool>::getInstance().releaseConnection(db);
    return posInfo;
}

int AgvShowInMap::agvStationId (int stationNum){
    int stationid;
    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlQuery query(db);
    bool succ=query.exec(QString("select * from StationTable where stationnum='%1'").arg(stationNum));
    if(succ&&query.first ())
    {
        stationid=query.value ("stationid").toInt ();
        qDebug()<<"stationid"<<stationid;
    }
    Singleton<ConnectionPool>::getInstance().releaseConnection(db);
    return stationid;
}
int AgvShowInMap::agvStationNum (int stationId)
{
    int stationNum;
    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlQuery query(db);
    bool succ=query.exec(QString("select * from StationTable where stationid='%1'").arg(stationId));
    if(succ&&query.first ())
    {
        stationNum=query.value ("stationnum").toInt ();
        qDebug()<<"stationnum"<<stationNum;
    }
    Singleton<ConnectionPool>::getInstance().releaseConnection(db);
    return stationNum;
}
