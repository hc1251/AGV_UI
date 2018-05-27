#include "AgvCommunication.h"
#include <QTimer>
#include "AgvSymbol.h"
QMutex AgvCommunication::mutex;
AgvCommunication* AgvCommunication::instance = NULL;
AgvCommunication::AgvCommunication()
{
    ConfigUtil &config = Singleton<ConfigUtil>::getInstance();//读取配置文件中的内容
    hostPort = config.getHostPort();//8080
    agvPort = config.getAgvPort();//8080
    stationPort = config.getStationPort();//8081
//    qDebug()<<QStringLiteral("主机监听端口：")<<hostPort<<QStringLiteral("agv监听端口:")<<agvPort<<QStringLiteral("工位监听端口：")<<stationPort;
    hostIp.setAddress (QString(config.getHostIp ()));

    /*udpSocket->bind(hostIp,hostPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);//建立socket连接*/

    QTimer::singleShot(2000, this, SLOT(delayConnect2()));//2000ms后执行函数delayConnect2()函数，防止界面未完全启动时，socket已启动

   //connect(udpSocket,&QUdpSocket::readyRead,this,&AgvCommunication::slotReadyReadDatagrams);
}

AgvCommunication::~AgvCommunication()
{
    if(udpSocket != NULL && udpSocket->isOpen())
    {
        udpSocket->close();        
    }
}

AgvCommunication *AgvCommunication::getInstance()
{
    if(instance == NULL)
    {
        mutex.lock();
        if(NULL == instance)
        {
            instance = new AgvCommunication();
        }
        mutex.unlock();
    }
    return instance;
}

bool AgvCommunication::initAllAgv ()
{
    bool ok=false;
#ifndef INIT_ALL_AGV
#define INIT_ALL_AGV
    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlQuery query(db);
    ok = query.exec("select * from AgvMessage");
    while(ok && query.next())
    {
        AgvInformation* agvInfo=new AgvInformation(this);
        agvInfo->setAgvNum (query.value ("agvNum").toInt ());
        agvInfo->setAgvIp (query.value ("agvIpAddress").toString ());
        agvInfo->setAgvCurrentStation (query.value ("agvCurrentCard").toInt ());
        agvInfo->setAgvTargetStation (query.value ("agvTargetStationNum").toInt ());
        agvInfo->setAgvVoltage (query.value ("agvVoltage").toInt ());
        agvInfo->setAgvDistance (query.value ("agvDistance").toInt ());
        agvInfo->setAgvDirection(query.value("agvDirection").toInt());
        agvInfo->setAgvReciveTime (query.value ("agvReceiveTime").toString ());
        agvInfoList.append (agvInfo);
        connect(agvInfo,&AgvInformation::sigExistBuffer,this,&AgvCommunication::slotSendBuffer);//确保数据一直发送
    }
    Singleton<ConnectionPool>::getInstance().releaseConnection(db);
#endif
    return ok;
}

void AgvCommunication::delayConnect2()
{
    qDebug()<<"connect agv udpsocket";
    udpSocket = new QUdpSocket(this);
    bool bind = udpSocket->bind(hostIp, agvPort);

    //bool bind = udpSocket->bind(QHostAddress("192.168.31.254"), 8080);
    //if(udpSocket->bind(hostPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint))//建立socket连接

    if( bind )
    {
        if(!initAllAgv())
        {
            QMessageBox::warning(0,QStringLiteral("异常报告"),QStringLiteral("初始化AGV状态失败"));
            return;
        }
        else
            qDebug()<<"initialize all agv success";
    }else {
        QMessageBox::warning(0,QStringLiteral("警告-端口绑定失败"),QStringLiteral("8080..端口绑定失败，请先关闭该端口"));
        return;
    }
    connect(udpSocket,&QUdpSocket::readyRead,this,&AgvCommunication::slotReadyReadDatagrams);

    /*@by chen*/
    /*开启定时器，判断socket是否处于在线状态*/
    QTimer *timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpDate()));
    timer->start(1000);
}

void AgvCommunication::timerUpDate()
{
    if(udpSocket->bytesAvailable()!=0)
    {
        qDebug()<<QStringLiteral("接收不到数据，需重启Socket");
        udpSocket->close();

      emit sigReceiveContextToShow("接收不到数据，重启socket");

        if(!(udpSocket-> isOpen()))
        {
            udpSocket = new QUdpSocket(this);
            qDebug()<<"upDateUdp..close and open again";
            bool bind = udpSocket->bind(hostIp, agvPort);
            //if(!udpSocket->bind(stationPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint))//建立socket连接
            if(!bind)
            {
                QMessageBox::warning (0,QStringLiteral("警告——端口绑定失败"),QStringLiteral("8080端口绑定失败，请先关闭该端口"));
                return;
            }
            connect(udpSocket,&QUdpSocket::readyRead,this,&AgvCommunication::slotReadyReadDatagrams);
        }
     }
}

void AgvCommunication::slotReadyReadDatagrams()
{
    //缓冲有数据
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        QHostAddress senderHostAddress;
        datagram.resize(udpSocket->pendingDatagramSize());//读取缓冲数据长度
        udpSocket->readDatagram(datagram.data(),datagram.size(),&senderHostAddress);//读取数据，长度，ip地址
        
        qDebug()<<hostPort<<QStringLiteral("端口接收命令：")<<senderHostAddress.toString()<<QString(datagram.toHex());
        if(datagram.size()!=0x010)
        {
            qDebug()<<hostPort<<QStringLiteral("---------------------------------端口接收数据长度有误");
            return;
        }
        
        if(((quint8)datagram.at (0)!=0xFD)&& ((quint8)datagram.at(1)!= 0x10))
        {
            qDebug()<<hostPort<<QStringLiteral("端口接收数据标志位有误");
            return;
        }

        if(((quint8)datagram.at (4)==0x31))//接收到的是小车发送来的消息
        {
             //接收到小车发的程序后立即回复
            quint8 cmd=(quint8)datagram.at (4)==0x31?0x41:0x42;
            QByteArray sendCmd=datagram;
            sendCmd[4]=cmd;

             //显示到界面上，，需要先处理后显示
             //emit sigReceiveContextToShow(datagram);
             //udpSocket->writeDatagram (sendCmd,QHostAddress(senderHostAddress.toString ()),agvPort);
             //emit sigSendContextToShow(datagram);

            int agvNum=(quint8)datagram.at(14);
            qDebug()<<hostPort << QStringLiteral("端口 ") << agvNum <<QStringLiteral("号车 接收命令：")<<senderHostAddress.toString()<<QString(datagram.toHex());
            if((agvNum>4||(agvNum<1)))
            {
                qDebug()<<QStringLiteral("小车编号有误");
                return;
            }
            AgvInformation *agvInfo=agvInfoList.at ((quint8)datagram.at (14)-1);//(quint8)datagram.at (14)为小车的编号
            int earlyCard=agvInfo->getAgvCurrentStation();//上一次读取的卡号
            int newCard= (quint8)datagram.at(5);//最新读取的卡号
            agvInfo->setAgvIp (senderHostAddress.toString ());
            agvInfo->setAgvCurrentStation ((quint8)datagram.at (5));//当前卡号
            agvInfo->setAgvCurrentSpeed ((quint8)datagram.at (6));//当前速度
            agvInfo->setAgvVoltage ((quint8)datagram.at (7));//当前电压
            agvInfo->setAgvDistance ((quint8)datagram.at (8));//移动距离
            agvInfo->setAgvDirection((quint8)datagram.at(9));//移动方向
            agvInfo->setAgvCurrentStatus ((quint8)datagram.at (10));//当前状态
            agvInfo->setAgvErrorInfo ((quint8)datagram.at(11));//错误信息
            agvInfo->setAgvTargetStation ((quint8)datagram.at (12));//AGV目标工位号
            // agvInfo->setAgvReciveTime (QTime::currentTime ().toString ("hhmmss"));
            agvInfo->setAgvReciveTime(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

           //  QDateTime current_date_time = QDateTime::currentDateTime();
           //  QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");

            AgvIpAddress=senderHostAddress.toString();
            AgvCurrentCard=(quint8)datagram.at (5);
            AgvSpeed=(quint8)datagram.at (6);
            AgvVoltage=(quint8)datagram.at (7);
            AgvDistance=(quint8)datagram.at (8);
            AgvDirection=(quint8)datagram.at(9);
            AgvState=(quint8)datagram.at (10);
            AgvError=(quint8)datagram.at(11);
            AgvTargetStationNum=(quint8)datagram.at (12);
            AgvReceiveTime=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            qDebug()<<QStringLiteral("结果")<<AgvIpAddress<<AgvCurrentCard<<QStringLiteral("电压：")<<AgvVoltage<<AgvDistance<<AgvDirection<<AgvState<<AgvError<<AgvReceiveTime;
          //      sigSendNetworkPostionInfo((quint8)datagram.at(14),1234,AgvCurrentCard,AgvTargetStationNum,AgvReceiveTime);
                QString errorInfo=agvSymbol->errorInfo (AgvError);
                QString state = agvSymbol->getAgvStatus (AgvState);
             //判断界面中是否存在小车所处的卡的位置，不存在则返回，否则在监控界面中显示小车的位置
             QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
             QSqlQuery query(db);

             bool succ11=query.exec (QString("update AgvMessage set agvIpAddress='%1',agvCurrentCard='%2'"
                                                         ",agvSpeed='%3',agvVoltage='%4',agvDistance='%5',"
                                                         "agvDirection='%6',agvState='%7',agvError='%8',"
                                              "agvTargetStationNum='%9' ,agvReceiveTime='%10' where agvNum='%11'")
                                               .arg (AgvIpAddress).arg (AgvCurrentCard)
                                                 .arg (AgvSpeed).arg(AgvVoltage).arg(AgvDistance).arg(AgvDirection)
                                                 .arg(AgvState).arg(AgvError).arg(AgvTargetStationNum).arg(AgvReceiveTime).arg(agvNum));


            if(succ11)
            {
                qDebug()<<QStringLiteral("接收到的信息更新到数据库------成功");
            }
            else
            {
                qDebug()<<QStringLiteral("接收到的信息更新到数据库------失败");
            }
            
            int agvCard = (quint8)datagram.at (5);

            bool succ = query.exec(QString("select * from mapPoint where cardPoint=%1").arg (agvCard));

             if( succ && query.first() )
             {
                  emit agvReceiveMessage((quint8)datagram.at (14));
             }
             else
             {
                 qDebug()<< agvCard << QStringLiteral("卡号不在界面中");
             }

             //将读取的卡号存入数据库中
             if ( earlyCard!=newCard )
             {
                 query.prepare("insert into ReadCardTable(agvNum,cardNum,time,stramp)values(:agvNum,:cardNum,:time,:stramp)");
                 query.bindValue(":agvNum",(quint8)datagram.at (14)-1);
                 query.bindValue(":cardNum",newCard);
                 query.bindValue(":time",QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
                 query.bindValue(":stramp",QDateTime::currentDateTime().toTime_t());
                 bool suc11=query.exec();
                 if (suc11)
                 {
                     qDebug()<<QStringLiteral("小车读取卡号插入成功")<<(quint8)datagram.at(14)-1<<QStringLiteral("读到新卡号")<<newCard;
                 }
                 else
                 {
                     qDebug()<<QStringLiteral("小车读取新卡插入失败");
                 }
             }

             Singleton<ConnectionPool>::getInstance().releaseConnection(db);
         }
         else
         {
             int agvNum = (quint8)datagram.at(14);
             if((agvNum < 1)||(agvNum>4))
             {
                 qDebug()<<QStringLiteral("小车编号有误");
                 return;
             }
             if(!agvInfoList.at (agvNum-1)->agvAckSuccess (datagram))
             {
                 AgvInformation *agvInfo=agvInfoList.at (agvNum-1);//(quint8)datagram.at (2)为小车的编号
                 agvInfo->setAgvIp (senderHostAddress.toString ());
             }
         }

         //显示到界面上，，需要先处理后显示
         emit sigReceiveContextToShow(datagram);
     }

}


void AgvCommunication::slotSendBuffer (const QString &ip, const QByteArray &buffer)
{

    udpSocket->writeDatagram (buffer,QHostAddress(ip),agvPort);
    emit sigSendContextToShow(buffer);
}

//主控中心到AGV
void AgvCommunication::slotControlAgv(const quint8 &agvNum, HITAGV::ActionSymbol action)
{
    QString ip=agvInfoList.at (agvNum-1)->getAgvIp ();//获取IP地址
    QByteArray cmdData;
    cmdData.append(0xFD);//byte0
    cmdData.append(0x10);//byte1
    cmdData.append(agvNum);//byte2
    QByteArray data;
    data.append(0x0F);//byte3
    data.append(char(action));//byte4
    data.append((char)0x00);//byte5启动前进 单纯启动
    QByteArray zero;
    zero.fill(0x00,8);
    data.append(zero);//byte5~13数据
    cmdData.append(data);
    cmdData.append(0x20);//byte14
    cmdData.append (0xFF);//byte15
    udpSocket->writeDatagram(cmdData,QHostAddress(ip),agvPort);
    int listSize=infoList.size ();
    infoList.insert (listSize,cmdData);
    emit sigSendContextToShow(cmdData);
    qDebug()<<QStringLiteral("主控中心发送控制AGV命令：")<<QString(cmdData.toHex());
    if(agvInfoList.at (agvNum-1)->getAgvBufferList ().contains (cmdData))//将发送命令添加至缓存列表中
        return;
    else
        agvInfoList.at (agvNum-1)->appendAgvBuffer(cmdData);
}
//控制双向AGV
void AgvCommunication::slotDoubleControlAgv(const quint8 &agvNum, HITAGV::ActionSymbol action1, HITAGV::ActionSymbol action2)
{
    QString ip=agvInfoList.at (agvNum-1)->getAgvIp ();//获取IP地址
    QByteArray cmdData;
    cmdData.append(0xFD);
    cmdData.append(0x10);
    cmdData.append(agvNum);
    QByteArray data;
    data.append(0x0F);
    data.append(char(action1));//启动
    data.append((char)action2);//启动前进 单纯启动
    QByteArray zero;
    zero.fill(0x00,8);
    data.append(zero);
    cmdData.append(data);
    cmdData.append(0x20);
    cmdData.append (0xFF);
    udpSocket->writeDatagram(cmdData,QHostAddress(ip),agvPort);
    int listSize=infoList.size ();
    infoList.insert (listSize,cmdData);
    emit sigSendContextToShow(cmdData);
    qDebug()<<QStringLiteral("主控中心发送控制AGV命令：")<<QString(cmdData.toHex());
    if(agvInfoList.at (agvNum-1)->getAgvBufferList ().contains (cmdData))//将发送命令添加至缓存列表中
        return;
    else
        agvInfoList.at (agvNum-1)->appendAgvBuffer(cmdData);
}

//调度AGV至某一工位
void AgvCommunication::slotScheduleAgv (const quint8 agvNum, const QByteArray station)
{
    QString ip=agvInfoList.at (agvNum-1)->getAgvIp ();//获取IP地址
    QByteArray cmdData;
    cmdData.append(0xFD);
    cmdData.append(0x10);
    cmdData.append(agvNum);
    QByteArray data;
    data.append(0x0F);
    data.append(char(HITAGV::Central_Station));
    data.append(station);
    QByteArray zero;
    zero.fill(0x00,8);
    data.append(zero);
    cmdData.append(data);
    cmdData.append(0x20);
    cmdData.append(0xFF);
    udpSocket->writeDatagram(cmdData,QHostAddress(ip),agvPort);
    int listSize = infoList.size ();
    infoList.insert (listSize,cmdData);

    qDebug()<<QStringLiteral("主控中心发送调度AGV至某一工号命令：")<<QString(cmdData.toHex());
    if(agvInfoList.at(agvNum -1)->getAgvBufferList().contains(cmdData))
    {
        return;
    }
    else
    {
        agvInfoList.at(agvNum-1)->appendAgvBuffer(cmdData);
    }
    emit sigSendContextToShow(cmdData);
}

/* CRC 高位字节值表 */
const unsigned char  auchCRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};
/* CRC低位字节值表*/
const unsigned char   auchCRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
} ;

//计算CRC码
quint16 AgvCommunication::crc16(unsigned char *puchMsg, unsigned short usDataLen)
{
    unsigned  char uchCRCHi = 0xFF ;			/* 高CRC字节初始化  */
    unsigned char uchCRCLo = 0xFF ;			/* 低CRC 字节初始化 */
    quint16 uIndex ;							/* CRC循环中的索引  */
    quint16 r_crc;
    while (usDataLen--)
    {
        uIndex   = uchCRCHi ^ *puchMsg++ ;	/* 计算CRC */
        uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
        uchCRCLo = auchCRCLo[uIndex] ;
    }
    r_crc=(uchCRCHi<<8 |uchCRCLo);
    return (r_crc);
}
