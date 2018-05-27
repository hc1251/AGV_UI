#ifndef AGVINFORMATION_H
#define AGVINFORMATION_H

#include <QObject>
#include "AgvSymbol.h"
#include <QTimer>

class AgvInformation : public QObject
{
    Q_OBJECT
public:
    explicit AgvInformation(QObject *parent = 0);
    ~AgvInformation();

signals:
    void sigExistBuffer(const QString& ip,const QByteArray &buffer); //发送缓存中的数据
private slots:
    void slotSendBuffer();

private:
    quint8 agvNum;//小车编号
    QString agvIp;//小车IP
    quint8 agvPort;//小车端口
    quint8 agvCurrentStation;//小车当前站点
    quint8 agvTargetStation;//小车目标站点
    quint8 agvVoltage;//小车电压
    quint8 agvCurrentSpeed;//小车速度
    quint8 agvDistance;//小车移动距离
    quint8 agvDirection;//小车移动的方向
    quint8 agvCurrentStatus;//小车当前状态
    quint8 agvErrorNum;//小车错误编号
    QString agvReciveTime;//小车接收时间
    QList<QByteArray> bufferList;//小车发送缓存数据
    quint16 sendBufferCount;//发送缓存计数器
    QTimer *sendBufferTimer;

    bool powerLow;

public:
    quint8 getAgvNum()const{return agvNum;}
    QString getAgvIp()const{return agvIp;}
    quint8 getAgvPort()const{return agvPort;}
    quint8 getAgvCurrentStation()const{return agvCurrentStation;}
    quint8 getAgvTargetStation()const{return agvTargetStation;}
    quint8 getAgvVoltage()const{return agvVoltage;}
    quint8 getAgvCurrentSpeed()const{return agvCurrentSpeed;}
    quint8 getAgvDistance()const{return agvDistance;}
    quint8 getAgvDirection()const{return agvDirection;}
    quint8 getAgvCurrentStatus()const{return agvCurrentStatus;}
    quint8 getAgvErrorInfo()const{return agvErrorNum;}
    QString getAgvReciveTime()const{return agvReciveTime;}

    QList<QByteArray>& getAgvBufferList();
    bool agvAckSuccess(const QByteArray& agvData);//判断小车是否接受成功

    void setAgvNum(const quint8& num){this->agvNum=num;}
    void setAgvIp(const QString& ip){this->agvIp=ip;}
    void setAgvPort(const quint8& port){this->agvPort=port;}
    void setAgvCurrentStation(const quint8& station){this->agvCurrentStation=station;}
    void setAgvTargetStation(const quint8& station){this->agvTargetStation=station;}
    void setAgvVoltage(const quint8& voltage){this->agvVoltage=voltage;}
    void setAgvCurrentSpeed(const quint8& speed){this->agvCurrentSpeed=speed;}
    void setAgvDistance(const quint8& distance){this->agvDistance=distance;}
    void setAgvDirection(const quint8& direction){this->agvDirection=direction;}
    void setAgvCurrentStatus(const quint8& status){this->agvCurrentStatus=status;}
    void setAgvErrorInfo(const quint8 &errorNum ){this->agvErrorNum = errorNum;}
    void setAgvReciveTime(const QString& receiveTime){this->agvReciveTime=receiveTime;}
    void appendAgvBuffer(const QByteArray& cmdData);
};

#endif // AGVINFORMATION_H
