#ifndef AGVCOMMUNICATION_H
#define AGVCOMMUNICATION_H

#include <QObject>
#include "database/ConnectionPool.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QUdpSocket>
#include <QThread>
#include <time.h>
#include "util/ConfigUtil.h"
#include <QString>
#include "QHostAddress"
#include <QMetaType>
#include "AgvSymbol.h"
#include "QList"
#include "AgvInformation.h"


class AgvCommunication;
class AgvSymbol;
class AgvCommunication:public QObject
{
    Q_OBJECT
private:
    AgvCommunication();
    ~AgvCommunication();
    AgvCommunication operator=(const AgvCommunication& other);
    AgvCommunication(const AgvCommunication &other);
    bool initAllAgv();  //启动程序后初始化小车信息
    QList<AgvInformation*>agvInfoList;
    static QMutex mutex;
    static AgvCommunication* instance;
    quint16 agvPort;
    quint16 hostPort;
    quint16 stationPort;
    QHostAddress hostIp;
    quint16 agvInitPosition;
    QList<QByteArray>bufferList;//小车发送的缓存
    QTimer *sendBufferTimer;  //发送缓存定时器


    //Agv的信息
    QString AgvIpAddress;
    quint8 AgvNum;
    quint8 AgvCurrentCard;
    quint8 AgvSpeed;
    quint8 AgvVoltage;
    quint8 AgvDistance;
    quint8 AgvDirection;
    quint8 AgvState;
    quint8 AgvError;
    quint8 AgvTargetStationNum;
    QString AgvCargoName;
    QString AgvReceiveTime;

    AgvSymbol *agvSymbol;

public:
    static AgvCommunication *getInstance();
    QList<AgvInformation*>& getAgvInfoList()
    {
        return agvInfoList;
    }
    QList<QByteArray>infoList;
    QUdpSocket *udpSocket;
private slots:
    void delayConnect2();
    void slotReadyReadDatagrams();      //UDP端口有数据可读
    quint16 crc16(unsigned char *puchMsg, unsigned short usDataLen); //CRC校验
    void slotSendBuffer(const QString &ip,const QByteArray& buffer);//发送缓存数据
    void timerUpDate();//检查socket是否处于连接状态

public slots:
    void slotControlAgv(const quint8& agvNum, HITAGV::ActionSymbol action); //发送小车控制命令
    void slotDoubleControlAgv(const quint8& agvNum,HITAGV::ActionSymbol action1,HITAGV::ActionSymbol action2);
    void slotScheduleAgv(const quint8 agvNum, const QByteArray station);  //发送路线调度小车
signals:
    void sigSendContextToShow(QByteArray context);
    void sigReceiveContextToShow(QByteArray context);
    void agvReceiveMessage(quint8 num);//UDP接收的信息
    void sigSendNetworkPostionInfo(quint8 agvnum, quint16 taskid, quint8 postidfrom,quint8 postidto, QString arrivetime);
    void sigSendNetworkStatusInfo(quint8 agvnum,QString agvstatus,quint16 agvwsctaskid,quint8 agvlocation,QString agvalarm);
};

#endif // AGVCOMMUNICATION_H
