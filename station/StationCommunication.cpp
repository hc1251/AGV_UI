#include "StationCommunication.h"
#include "agv/AgvInformation.h"
#include "agv/AgvCommunication.h"
#include <QSqlQuery>
#include "database/ConnectionPool.h"
#include <QUrl>
#include <QMediaPlayer>
#include <windows.h>


QMutex StationCommunication::mutex;
StationCommunication* StationCommunication::instance=NULL;

StationCommunication::StationCommunication()
{
    ConfigUtil &config = Singleton<ConfigUtil>::getInstance();//读取配置文件中的内容
    hostPort = config.getHostPort();//8080
    agvPort = config.getAgvPort();//8080
    stationPort=config.getStationPort();//8081
    //QHostAddress hostIp;
    hostIp.setAddress (QString(config.getHostIp ()));

    //player->setMedia(QUrl::fromLocalFile("/resource/song.mp3"));///Users/me/Music/coolsong.mp3
    //player->setMedia(QUrl::fromLocalFile("/Users/Administrator/song.mp3"));
    //player->setMedia(QUrl::fromLocalFile("/Users/207Robot/Desktop/song.mp3"));

    player = new QMediaPlayer;
    QUrl url("D:/resource/song.mp3");
    player->setMedia(url);
    player->setVolume(100);

    bigStaLists << 60 << 61 << 62 << 63 << 64 << 65 << 66 << 67 << 68 << 69 << 70 << 71 << 72 << 73 << 74 << 75 << 76 << 77;
  smallStaLists << 41 << 43 << 39 << 40 << 35 << 37 << 32 << 34 << 28 << 25 << 23 << 22 << 20 << 19 << 17 <<  8 <<  5 <<  7;

    QTimer::singleShot(4000, this, SLOT(delayConnect()));//为了防止界面接收不到信号
    //connect(udpSocket,&QUdpSocket::readyRead,this,&StationCommunication::slotReadyReadDatagrams);

    /*
    udpSocket2 = new QUdpSocket(this);
    if(!udpSocket2->bind(8082, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint))//建立socket连接
    {
        QMessageBox::warning (0,"警告",tr("8082端口绑定失败，请先关闭该端口"));
        return;
    }
    connect(udpSocket2,&QUdpSocket::readyRead,this,&StationCommunication::slotReadyReadDatagrams2);

    */



}

StationCommunication::~StationCommunication()
{
    if(udpSocket != NULL && udpSocket->isOpen()){
        udpSocket->close();
    }
}

StationCommunication *StationCommunication::getInstance()
{
    if(instance == NULL){
        mutex.lock();
        if(NULL == instance){
            instance = new StationCommunication();
        }
        mutex.unlock();
    }
    return instance;
}


void StationCommunication::upDateUdp()
{

    if(udpSocket->bytesAvailable() != 0)
    {
        udpSocket->close();
        if(!(udpSocket-> isOpen()))
        {
            udpSocket = new QUdpSocket(this);
            qDebug()<<"upDateUdp..close and open again";
            bool bind = udpSocket->bind(hostIp, stationPort);
            //if(!udpSocket->bind(stationPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint))//建立socket连接
            if(!bind)
            {
                QMessageBox::warning (0,QStringLiteral("警告"),QStringLiteral("8081端口绑定失败，请先关闭该端口"));
                return;
            }
            connect(udpSocket,&QUdpSocket::readyRead,this,&StationCommunication::slotReadyReadDatagrams);
        }
    }

}

//定时检查最近一次接收AGV发送命令的时间，判断其是否离线
void StationCommunication::slotBroadCastUdp()
{

    qDebug()<<"8082 broad cast udp to 192.168.31.255 on 8081";
    QByteArray sendCmd("f000");//("Hello");
    QHostAddress ipst = QHostAddress("192.168.31.255");//QHostAddress::Broadcast;// QHostAddress("192.168.31.255");
    udpSocket2->writeDatagram(sendCmd, ipst, 8081);

}

void StationCommunication::delayConnect()
{
    udpSocket = new QUdpSocket(this);

    qDebug()<<"delayConnect。。..";

    //bool bind = udpSocket->bind(QHostAddress("192.168.31.254"), 8081);

    bool bind = udpSocket->bind(hostIp, stationPort);
    //if(!udpSocket->bind(stationPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint))//建立socket连接
    if(!bind)
    {
        QMessageBox::warning (0,QStringLiteral("警告y"),QStringLiteral("8081端口绑定失败，请先关闭该端口"));
        return;
    }

    connect(udpSocket,&QUdpSocket::readyRead,this,&StationCommunication::slotReadyReadDatagrams);

    udpUpdataTimer = new QTimer(this);
    connect (udpUpdataTimer,SIGNAL(timeout()),this,SLOT(upDateUdp()));
    udpUpdataTimer->start (3000);
}


void StationCommunication::slotReadyReadDatagrams2()
{
    while(udpSocket->hasPendingDatagrams()){
        QByteArray datagram;
        QHostAddress senderHostAddress;
        qint64 lenth = udpSocket->pendingDatagramSize();
        if(lenth < 0){
            return;
        }
        datagram.resize(lenth);
        udpSocket->readDatagram(datagram.data(),datagram.size(),&senderHostAddress);


        emit sigReceiveContextToShow(datagram.toHex());
        //qDebug()<<"8082端口接收命令："<<senderHostAddress.toString()<<QString(datagram.toHex());
    }
}

void StationCommunication::slotReadyReadDatagrams()
{
    while(udpSocket->hasPendingDatagrams())//如果有数据
    {
        QByteArray datagram;
        QHostAddress senderHostAddress;


        datagram.resize(udpSocket->pendingDatagramSize());
        qint64 lenth = datagram.size();//udpSocket->pendingDatagramSize();
        if(lenth < 0)
        {
            QMessageBox::warning(NULL,QStringLiteral("警告2"),QStringLiteral("程序可能会出现重大问题，需要改善，请重启程序！"));
            return;
        }
        datagram.resize(lenth);
        qint64 readResult = udpSocket->readDatagram(datagram.data(),datagram.size(),&senderHostAddress);

        if(readResult < 0)
        {
            QMessageBox::warning(NULL,QStringLiteral("警告1"),QStringLiteral("程序可能会出现重大问题，需要改善，请重启程序！"));
            return;
        }

        //datagram.resize(lenth);
        //udpSocket->readDatagram(datagram.data(),datagram.size(),&senderHostAddress);

        emit sigReceiveContextToShow(datagram);
        qDebug()<<stationPort<<QStringLiteral("端口接收命令：")<<senderHostAddress.toString()<<QString(datagram.toHex());

        if(datagram.size()!=0x010)
        {
            qDebug()<<stationPort<<QStringLiteral("端口接收数据长度有误");
            return;
        }
        if(((quint8)datagram.at (0)!=0xFD) && ((quint8)datagram.at(1)!= 0x10))
        {
           // qDebug()<<stationPort<<"端口接收数据标志位有误";
            return;
        }

        //立即回复，待测
        quint8 cmd = (quint8)datagram.at (4) == 0x72 ? 0x52 : 0x51;
        QByteArray sendCmd=datagram ;
        sendCmd[4]=cmd ;
        QHostAddress ipst = QHostAddress(senderHostAddress.toString ());
        udpSocket->writeDatagram(sendCmd, ipst, stationPort) ;
        //udpSocket->writeDatagram(sendCmd, ipst, 3072) ;
        //udpSocket->writeDatagram(sendCmd, ipst, 8082) ;
        qDebug() << stationPort <<QStringLiteral( "端口发送返回命令：" )<< ipst.toString()<<QString(sendCmd.toHex());
        emit sigSendContextToShow(sendCmd) ;//接收到的信息显示在程序接收界面中

        if(((quint8)datagram.at (4)==0x71))//放行
        {
            quint8 stNum = (quint8)datagram.at (14);//工位编号

            //某些大号的工位编号和小号的呼叫盒编号处同一位置，在界面中未显示。
            if (bigStaLists.contains(stNum))
            {
                int index = bigStaLists.indexOf(stNum);
                stNum = smallStaLists.at(index);
            }

            QList <AgvInformation *> agvList = AgvCommunication::getInstance()->getAgvInfoList();
            bool left = true;
            for (int i = 0; i < agvList.size(); i++)
            {
                AgvInformation *agv = agvList.at(i);
                if (agv->getAgvCurrentStation() == stNum)
                {
                    left = false;
                    quint8 agvNum = agv->getAgvNum();
                    emit sigControlAgv (agvNum,HITAGV::Central_Start);
                }
            }
            if(left)
            {
                qDebug()<<"agv allready left";
            }
            return;
        }

        if(((quint8)datagram.at (4)==0x72))//呼叫
        {

            QString stationIp = senderHostAddress.toString ();
            StationInformation* stationInfo = new StationInformation(this);

            for (int i = 5; i <= 10 ; i++)
            {
                quint8 callBoxNum = (quint8)datagram.at (i);//呼叫盒编号
                //if (callBoxNum != 0){
                    //need a iterator and then.
                    //QList <quint8 > *li = &(stationInfo->callBoxList);
                    //li->append(callBoxNum);//全部加上，容易比较些

                    stationInfo->callBoxList.append(callBoxNum);

                //}
            }

            //有呼叫，播放音频
            player->play();

            /*
            quint8 callBox1Num=(quint8)datagram.at (5);//呼叫盒编号
            quint8 callBox2Num=(quint8)datagram.at (6);//呼叫盒编号
            quint8 callBox3Num=(quint8)datagram.at (7);//呼叫盒编号
            quint8 callBox4Num=(quint8)datagram.at (8);//呼叫盒编号
            quint8 callBox5Num=(quint8)datagram.at (9);//呼叫盒编号
            quint8 callBox6Num=(quint8)datagram.at (10);//呼叫盒编号
            */
            quint8 stationNum=(quint8)datagram.at (14);//工位编号

            stationInfo->setStationIp (stationIp);
            stationInfo->setStationNum (stationNum);
            /*
            stationInfo->setCallBox1Num (callBox1Num);
            stationInfo->setCallBox2Num (callBox2Num);
            stationInfo->setCallBox3Num (callBox3Num);
            stationInfo->setCallBox4Num (callBox4Num);
            stationInfo->setCallBox5Num (callBox5Num);
            stationInfo->setCallBox6Num (callBox6Num);
            */


            /****************/
            //QString stationNumSt = (QString)datagram.at (14);//工位编号
            QString stationNumSt = QString::number ( (quint8) datagram.at (14) );

            if( !mapStaCaller.contains(stationNumSt) )
            {
                qDebug()<<"receive new station info and not contain";
                mapStaCaller.insert(stationNumSt, stationInfo->callBoxList);
                emit sigStationReceiveNew (stationInfo);
            }
            else
            {

                QList <quint8> callEx = mapStaCaller. value(stationNumSt) ;
                bool contai = false;
                StationInformation * stationNew = new StationInformation(this);

                //init stationNew
                for(int i = 0; i < callEx.count(); i++){
                    QList <quint8> *tmpList = &(stationNew -> callBoxList);
                    tmpList->append(0);
                }
                stationNew->setStationNum(stationNum);

                for(int i = 0; i < callEx.count(); i++)
                {
                    quint8 callNumNew = stationInfo->callBoxList.at(i);
                    quint8 callNumEx  = callEx.at(i);

                    if(callNumNew != 0 && callNumEx == 0 )
                    {
                        contai = true;
                        QList <quint8> *tmpList = &(stationNew -> callBoxList);
                        //qDebug()<<"the new Num:"<<i;
                        tmpList->replace(i, 1);
                        callEx.replace(i, 1);
                    }
                    /*** by chen  未用到
                     * 工位呼叫原理，每次只有一个呼叫盒发送信息，若之前的呼叫盒信息未收到，新的呼叫盒信息会连带之前的呼叫盒信息
                     * 若已收到，其他位均为0，只有呼叫盒的信息为1
                    else if(callNumNew == 0 && callNumEx != 0 )
                    {
                        contai = true;
                        QList <quint8> *tmpList = &(stationNew -> callBoxList);
                        //qDebug()<<"the new Num:"<<i;
                        tmpList->replace(i, 0);
                        callEx.replace(i, 0);
                    }***/
                }

                mapStaCaller.insert(stationNumSt, callEx);

                if( contai ) //变化了
                {
                    qDebug()<<"station info is changed";
                    emit sigStationReceiveNew (stationNew);
                }
                else
                {
                    qDebug()<<"the same as before";
                }
            }
        }

        Sleep(200);

    }
}

            /****************/

            /*
            if(stationInfoList.length () == 0)//工位信息表中无内容，则直接添加
            {
                stationInfoList.append (stationInfo);
                emit sigStationReceiveInfo (stationInfoList.length ());
                return;
            }


            bool stationIpContains = false ;
            for(int i=0;i<stationInfoList.length();i++)//工位信息表中有内容，首先判断是否有重复的信息，有则替换，否则添加
            {
                StationInformation* staInfoEx = stationInfoList.at(i);//allready have
                if(staInfoEx->getStationIp() .contains (stationIp)) //ip is same with station call
                {
                    stationIpContains = true;
                    QList <quint8> callListNew = stationInfo ->callBoxList;
                    QList <quint8> callListEx = staInfoEx->callBoxList;

                    if(callListEx.size() != callListNew.size()){

                        qDebug()<<"size not equal, need fresh 2";
                        int index = i;
                        stationInfoList.replace(index, stationInfo);
                        emit sigStationReceiveInfo (stationInfoList.length ());

                    }else{

                        bool notExsit = false;
                        int size = callListNew.size();
                        int t = 0;
                        for (t = 0; t < size; t++){
                            quint8 callNumNew = callListNew.at(t);
                            quint8 callNumEx = callListEx.at(t);

                            if(callNumEx != callNumNew ){//对应位置不相等，不存在
                                notExsit = true;
                                switch (t) {
                                case 0:
                                    staInfoEx->setCallBox1Num (1);
                                    break;
                                case 1:
                                    staInfoEx->setCallBox2Num (1);
                                    break;
                                case 2:
                                    staInfoEx->setCallBox3Num (1);
                                    break;
                                case 3:
                                    staInfoEx->setCallBox4Num (1);
                                    break;
                                case 4:
                                    staInfoEx->setCallBox5Num (1);
                                    break;
                                case 5:
                                    staInfoEx->setCallBox6Num (1);
                                    break;
                                default:
                                    break;
                                }
                                //break;
                            }
                        }

                        if(!notExsit){
                            qDebug()<<"exsit the new callers....";
                            //xxxxxxxxxxxxxxxxxxxxx
                            //emit sigStationReceiveInfo (stationInfoList.length ());
                            //xxxxxxxxxxxxxxxxxxxxx
                        }else{

                            qDebug()<<"not exsit the new callers2.. ,need refresh";
                            int index = i;
                            //stationInfoList.replace(index, stationInfo);
                            staInfoEx->setStationNum (stationNumSt);
                            stationInfoList.replace(index, staInfoEx);
                            emit sigStationReceiveInfo (stationInfoList.length ());
                        }
                    }


                    */

                    /*
                    bool change = false;//判断staInfo是否有改变变量
                    for (int i = 0; i < callListNew.size(); i++)
                    {

                        quint8 callNumNew = callListNew.at(i);
                        bool contains = false; //判断callNumNew是否已经被包含在列表中
                        for(int j = 0; j < callListEx.size(); j++)
                        {
                            quint8 callNumEx = callListEx.at(j);
                            if(callNumEx == callNumNew)
                            {
                                qDebug()<<"same with the in com";
                                contains = true;
                            }
                        }

                        if(contains){
                            //已经存在的端口号列表包含了新的端口号 do nothing
                            qDebug()<<"in == ex, do nothing";
                         } else {
                            //已经存在的端口号列表未包含新的端口号，append
                            qDebug()<<"in != ex, need refresh";
                            staInfoEx->callBoxList.append(callNumNew);
                            change = true;
                        }
                    }

                    if(change){
                        //staInfo有改变才会触发
                        emit sigStationReceiveInfo (stationInfoList.length ());
                    }
                    */
//                }
//            }

//            if(!stationIpContains){
//                stationInfoList.append (stationInfo);

//                emit sigStationReceiveInfo (stationInfoList.length ());
//            }

            /*
            int num=0;
            for(int i=0;i<stationInfoList.length ();i++)//工位信息表中有内容，首先判断是否有重复的信息，有则替换，否则添加
            {
                StationInformation* staInfo = stationInfoList.at(i);
                if(staInfo->getStationIp ().contains (stationIp))
                {
                    num++;
                    stationInfoList.removeAt (i);
                    stationInfoList.append (stationInfo);
                }
            }

            if(num == 0)
            {
                stationInfoList.append (stationInfo);
            }

            *
            */

            //stdout
//            for(int i=0;i<stationInfoList.length ();i++)
//            {
//                qDebug()<<"工位IP："<<stationInfoList.at (i)->getStationIp ()<<"呼叫盒"<<stationInfoList.at (i)->getCallBox1Num ()
//                       <<"工位编号"<<stationInfoList.at (i)->getStationNum ();
//            }
//        }

//        qDebug()<<"stationInfoList Size2:"<<stationInfoList.count();

        //这样每次只要接收到消息都会触发显示函数,数据量不多这样也是可以
        //emit sigStationReceiveInfo (stationInfoList.length ());
//    }
//}

StationInfo* StationCommunication::StationNameInfo(int stationNum, int callBoxNum)
{
    stationInfo=new StationInfo;
    stationInfo->boardName="";
    stationInfo->cargoName="";

    QString boardName,cargoName;
    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlQuery query(db);
    bool succ=query.exec(QString("select * from CallBoxInfo where stationNum=%1 and callBoxNum=%2").arg(stationNum).arg(callBoxNum));
    if(succ&&query.first ())
    {
        boardName=query.value ("boardName").toString();
        cargoName=query.value ("cargoName").toString ();
    }
    else
    {
        boardName="";
        cargoName="";
        qDebug()<<"工位名称查询失败";
    }
    stationInfo->boardName=boardName;
    stationInfo->cargoName=cargoName;
    Singleton<ConnectionPool>::getInstance().releaseConnection(db);
    return stationInfo;
}


