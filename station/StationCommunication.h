#ifndef STATIONCOMMUNICATION_H
#define STATIONCOMMUNICATION_H

#include <QObject>
#include"database/ConnectionPool.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtNetwork>
#include <QUdpSocket>
#include <QThread>
#include <time.h>
#include "util/ConfigUtil.h"
#include <QString>
#include "QHostAddress"
#include <QMetaType>
#include "QList"
#include "StationInformation.h"
#include "agv/AgvSymbol.h"
#include <QMediaPlayer>

class StationInformation;
class StationInfo{
public:
    QString boardName;
    QString cargoName;
};

class StationCommunication : public QObject
{
    Q_OBJECT
private:
    StationCommunication();
    ~StationCommunication();
    StationCommunication operator = (const StationCommunication &others);
    QList<StationInformation*> stationInfoList;
    static QMutex mutex;
    static StationCommunication* instance;
    quint16 agvPort;
    quint16 hostPort;
    quint16 stationPort;
    QHostAddress hostIp;

    QList <quint8> bigStaLists;
    QList <quint8> smallStaLists;

    QMediaPlayer *player;
    QTimer *udpUpdataTimer;

public:
    QUdpSocket *udpSocket;
    QUdpSocket *udpSocket2;
    static StationCommunication *getInstance();
    QList<StationInformation*> &getStationInfoList(){
        return stationInfoList;
    }
    StationInfo *StationNameInfo(int callBoxNum,int stationNum);
    StationInfo *stationInfo;
    QMap <QString, QList<quint8> > mapStaCaller;


private slots:
    void slotReadyReadDatagrams();
    void slotReadyReadDatagrams2();
    void slotBroadCastUdp();
    void delayConnect();
    void upDateUdp();

signals:
    void sigSendContextToShow(QByteArray context);
    void sigReceiveContextToShow(QByteArray context);
    void sigStationReceiveInfo(quint8 num);
    void sigStationReceiveNew(StationInformation *sta);
    void sigControlAgv(const quint8& agvNum,HITAGV::ActionSymbol action);//发送AGV启动、停止、解锁、复位信号
};

#endif // STATIONCOMMUNICATION_H
