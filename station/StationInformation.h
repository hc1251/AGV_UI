#ifndef STATIONINFORMATION_H
#define STATIONINFORMATION_H

#include <QObject>
#include <QDebug>

class StationInformation : public QObject
{
    Q_OBJECT
public:
    explicit StationInformation(QObject *parent = 0);
    ~StationInformation();
    QList <quint8> callBoxList;

signals:

public slots:
private:
    quint8 stationNum;//工位编号
    quint8 callBox1Num;//呼叫盒1编号
    quint8 callBox2Num;//呼叫盒2编号
    quint8 callBox3Num;//呼叫盒3编号
    quint8 callBox4Num;//呼叫盒4编号
    quint8 callBox5Num;//呼叫盒5编号
    quint8 callBox6Num;//呼叫盒6编号
    QString stationIp;//工位Ip
    QList<QByteArray> receiveBufferList;//接收工位缓存数据
public:
    quint8 getStationNum()const{return stationNum;}
    quint8 getCallBox1Num()const{return callBox1Num;}
    quint8 getCallBox2Num()const{return callBox2Num;}
    quint8 getCallBox3Num()const{return callBox3Num;}
    quint8 getCallBox4Num()const{return callBox4Num;}
    quint8 getCallBox5Num()const{return callBox5Num;}
    quint8 getCallBox6Num()const{return callBox6Num;}

    QString getStationIp()const{return stationIp;}
    QList<QByteArray>& getStationReceiveBuffer();

    void setStationNum(const quint8& num){this->stationNum=num;}
    void setStationIp(const QString& ip){this->stationIp=ip;}
    void setCallBox1Num(const quint8& num){this->callBox1Num=num;}
    void setCallBox2Num(const quint8& num){this->callBox2Num=num;}
    void setCallBox3Num(const quint8& num){this->callBox3Num=num;}
    void setCallBox4Num(const quint8& num){this->callBox4Num=num;}
    void setCallBox5Num(const quint8& num){this->callBox5Num=num;}
    void setCallBox6Num(const quint8& num){this->callBox6Num=num;}

    void appendStationReceiveBuffer(const QByteArray& cmdData);


};

#endif // STATIONINFORMATION_H
