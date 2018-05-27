#include "AgvInformation.h"
#include <QThread>

AgvInformation::AgvInformation(QObject *parent) : QObject(parent)
  ,agvNum(0),agvIp(""),agvPort(0),agvCurrentStation(0),agvTargetStation(0),agvVoltage(0)
  ,agvCurrentSpeed(0),agvDistance(0),agvCurrentStatus(0),agvErrorNum(0),agvReciveTime("")
{
    sendBufferTimer=new QTimer(this);
    connect(sendBufferTimer,&QTimer::timeout,this,&AgvInformation::slotSendBuffer);
    sendBufferTimer->start (1000);//开启定时器
}

AgvInformation::~AgvInformation ()
{

}

QList<QByteArray>& AgvInformation::getAgvBufferList()
{
    return bufferList;
}
bool AgvInformation::agvAckSuccess (const QByteArray &agvData)
{
    //如果命令标志位相同则确认小车接收成功
    if(!bufferList.isEmpty() && agvData.size() == 0x10
            && bufferList.at(0).size() == 0x10)
    {
        for(int i=0;i<bufferList.length ();i++)
        {
            if((bufferList.at (i).at (4)==agvData.at (4)))
            {
                bufferList.removeAt (i);
                i--;
            }
        }
        qDebug()<<QStringLiteral("接收成功");
        sendBufferCount=0;

        return true;
    }
    return false;
}

void AgvInformation::appendAgvBuffer(const QByteArray &cmdData)
{
    if(cmdData.size() != 0x10){
        qDebug()<<QStringLiteral("命令字节缺失");
        return;
    }
    if(!bufferList.contains(cmdData)){
        bufferList.append(cmdData);
    }
}

void AgvInformation::slotSendBuffer (){

    if(!bufferList.isEmpty ())
    {
        for(int i=0;i<bufferList.length ();i++)
        {
            emit sigExistBuffer(this->agvIp,bufferList.at (i));
        }
        sendBufferCount++;
        if(sendBufferCount > 15){
            qDebug()<<QStringLiteral("发送次数超过10次,未收到小车的回复");
            bufferList.removeAt(0);
            sendBufferCount=0;
        }
    }
}
