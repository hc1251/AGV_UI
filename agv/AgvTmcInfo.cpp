#include "AgvTmcInfo.h"
#include "database/ConnectionPool.h"
#include <QMutex>

QMutex AgvTmcInfo::mutex;
AgvTmcInfo* AgvTmcInfo::instance = NULL;

AgvTmcInfo::AgvTmcInfo(QObject *parent) : QObject(parent)
{
    //single paton
    crossNum = 0;
}

AgvTmcInfo *AgvTmcInfo::getInstance()
{
    if(instance == NULL){
        mutex.lock();
        if(NULL == instance){
            instance = new AgvTmcInfo();
        }
        mutex.unlock();
    }
    return instance;
}



//处理交管信息
//agvnum：AGV的编号   cardnum:AGV所在的卡号   directionnum:AGV行使的方向 ，isFiFo 是否先进先出
void AgvTmcInfo::slotTmcTable (int agvnum,int cardnum,int directionnum){

    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlQuery query(db);

    int agvNum = agvnum;
    int agvCurrentCard = cardnum;
    //int agvDirection = 1;
    //bool isFiFo = YES;
    //int crossNum = 0;

    int tt = 0;

    /*
    if(cardnum == 51){
        qDebug()<<"51号卡号过滤交管功能";
        return;
    }
    */

    bool succ=query.exec(QString("select * from mapPoint where cardPoint=%1").arg (agvCurrentCard));
    if(!(succ&&query.first()))
    {
        qDebug()<< agvCurrentCard <<QStringLiteral("卡号不在界面中, 过滤交管功能");
        return;
    }






    //优点：1、一张卡即当一个路口的加锁卡又当另一个路口的解锁卡,多个路口多个加减锁卡，前提是路口不一样 || 2、工位卡可以当加解锁卡
    //加锁卡
    bool succ1 = query.exec (QString("select * from TmcTable where lockcard = '%1'").arg(agvCurrentCard));
    if(succ1 && query.first())
    {
        //read locked card

        crossNum = query.value("crossid").toInt();//交管路口
        tt = crossNum;
        //qDebug()<<QObject::tr ("读到加锁卡")<<agvCurrentCard<<"crossNum:"<<crossNum;

        bool succ9 = query.exec (QString("select * from TmcAgvNum where agvNum = '%1' and crossid = '%2'").arg(agvNum).arg(crossNum));
        if(succ9 && query.first()){

            //表明该车已经在交管表中了，对后续事件不考虑
            qDebug()<<"alerady in TmcAgvNum, do nothing;";

        }else{

            //qDebug()<<"not exsit in TmcAgvNum, add to TmcAgvNum";

            //如果该交管口有不是该车timestap，则表示已经有其他车在该交管口了
            bool succ3 = query.exec (QString("select * from TmcAgvNum where agvNum != '%1' AND crossid = '%2'").arg(agvNum).arg(crossNum));

            if(succ3 && query.first()) {//交管路口有车，锁定

                qDebug()<<"there have a another car in the cross ,need to stop now!" << crossNum;
                emit sigControlAgv(agvNum,HITAGV::Central_Lock);

            } else {//无车，添加进数据库,继续前进
                //qDebug()<<" try to insert car to TmcAgvNum";
                //qDebug()<<agvNum<<"号车"<<QObject::tr ("读到加锁卡")<<agvCurrentCard<<"crossNum:" << crossNum <<"并添加进数据库";

                query.prepare("insert into TmcAgvNum(crossid,agvNum,agvTime)values(:crossid,:agvNum,:agvTime)");
                query.bindValue (":crossid",crossNum);
                query.bindValue (":agvNum",agvNum);
                query.bindValue (":agvTime",QTime::currentTime ().toString ("hhmmss"));
                query.exec();
            }
        }
    }

    //读到其他卡都要解锁该车对应的路口
    if(tt == 0)
    {
        //表示读到的非加锁卡,只要一辆车读取到非加锁卡，都要解锁掉另外的车辆，而不管是在哪个路口，，，只有两辆车这样做可以，多了车估计需要拟定新协议，读到卡和心跳包分开
        qDebug()<<"read unlock card..."<<agvNum << crossNum;

        //bool deleSucc =  query.exec (QString("delete from TmcAgvNum where agvNum = '%1' AND cross = '%2'").arg(agvNum).arg(crossNum));
        bool deleSucc =  query.exec (QString("delete from TmcAgvNum where agvNum = '%1'").arg(agvNum));

        if(deleSucc)
        {
            qDebug()<<"delete car in TmcAgvNum success!";
        }
        else
        {
            qDebug()<<"delete car in TmcAgvNum failed!";
        }

        //放行交管表中的小车
        int savedAgvNum = 0;
        bool succ9=query.exec (QString("SELECT agvNum FROM TmcAgvNum where agvNum != '%1'").arg (agvNum));
        if(succ9&&query.last())
        {
            int tmp = query.value("agvNum").toInt();//获取小车的编号
            if(savedAgvNum < tmp){
                savedAgvNum = tmp;//!fifo
            }
        }
        if(savedAgvNum != 0)
        {
            qDebug()<<"must..release agvNum = "<<savedAgvNum;
            emit  sigControlAgv(savedAgvNum,HITAGV::Central_Unlock);
        }

        /*
        //放行交管表中的小车
        int savedAgvNum = 0;
        bool succ10=query.exec (QString("SELECT agvNum FROM TmcAgvNum where crossid='%1'").arg (crossNum));
        if(succ10&&query.last())
        {
            int tmp = query.value("agvNum").toInt();//获取小车的编号
            if(savedAgvNum < tmp){
                savedAgvNum = tmp;//!fifo
            }
        }

        if(savedAgvNum != 0){
            qDebug()<<"must..release agvNum = "<<savedAgvNum;
            emit  sigControlAgv(savedAgvNum,HITAGV::Central_Unlock);
        }else{
            //qDebug()<<"savedAgvNum == 0, no car in the Tabel";
        }
        */
    }

    Singleton<ConnectionPool>::getInstance().releaseConnection(db);


    /*
    //解锁卡
    bool succ4 = query.exec (QString("select * from TmcTable where unlockCard = '%1'").arg(agvCurrentCard));
    if(succ4 && query.first()) {

        int crossNum2 = query.value("cross").toInt();//交管路口
        qDebug()<<agvNum << "号车"<<QObject::tr ("读到解锁卡")<<agvCurrentCard<<"cross:"<<crossNum2<<"并删除数据库信息";

        //去删除该车所在的交管信息
        bool deleSucc =  query.exec (QString("delete from TmcAgvNum where agvNum = '%1' AND cross = '%2'").arg(agvNum).arg(crossNum2));

        if(deleSucc){
            //qDebug()<<"delete car in TmcAgvNum success!";
        }else{
            //qDebug()<<"delete car in TmcAgvNum failed!";
        }

        //放行交管表中的小车
        int savedAgvNum = 0;
        bool succ10=query.exec (QString("SELECT agvNum FROM TmcAgvNum where cross='%1'").arg (crossNum2));
        if(succ10&&query.last())
        {
            int tmp = query.value("agvNum").toInt();//获取小车的编号
            if(savedAgvNum < tmp){
                savedAgvNum = tmp;//!fifo
            }
        }

        if(savedAgvNum != 0){
            qDebug()<<"must..release agvNum = "<<savedAgvNum;
            emit  sigControlAgv(savedAgvNum,HITAGV::Central_Unlock);
        }else{
            //qDebug()<<"savedAgvNum == 0, no car in the Tabel";
        }

    }
    Singleton<ConnectionPool>::getInstance().releaseConnection(db);
    */

}



/*
//处理交管信息
//agvnum：AGV的编号   cardnum:AGV所在的卡号   directionnum:AGV行使的方向 ，isFiFo 是否先进先出
void AgvTmcInfo::slotTmcTable (int agvnum,int cardnum,int directionnum){
    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlQuery query(db);

    int agvNum = agvnum;
    int agvCurrentCard = cardnum;
    //int agvDirection = 1;
    //bool isFiFo = YES;

    //优点：1、一张卡即当一个路口的加锁卡又当另一个路口的解锁卡,多个路口多个加减锁卡，前提是路口不一样 || 2、工位卡可以当加解锁卡
    //加锁卡
    bool succ1 = query.exec (QString("select * from TmcTable where lockcard = '%1'").arg(agvCurrentCard));
    if(succ1 && query.first()){
        //read locked card

        int crossNum = query.value("cross").toInt();//交管路口
        //qDebug()<<QObject::tr ("读到加锁卡")<<agvCurrentCard<<"crossNum:"<<crossNum;

        bool succ9 = query.exec (QString("select * from TmcAgvNum where agvNum = '%1' and cross = '%2'").arg(agvNum).arg(crossNum));
        if(succ9 && query.first()){

            //表明该车已经在交管表中了，对后续事件不考虑
            //qDebug()<<"alerady in TmcAgvNum, do nothing;";

        }else{

            //qDebug()<<"not exsit in TmcAgvNum, add to TmcAgvNum";

            //如果该交管口有不是该车timestap，则表示已经有其他车在该交管口了
            bool succ3 = query.exec (QString("select * from TmcAgvNum where agvNum != '%1' AND cross = '%2'").arg(agvNum).arg(crossNum));

            if(succ3 && query.first()) {//交管路口有车，锁定
                qDebug()<<"there have a another car in the cross ,need to stop now!";
                emit sigControlAgv(agvNum,HITAGV::Central_Lock);
            } else {//无车，添加进数据库,继续前进
                //qDebug()<<" try to insert car to TmcAgvNum";
                qDebug()<<agvNum<<"号车"<<QObject::tr ("读到加锁卡")<<agvCurrentCard<<"crossNum:" << crossNum <<"并添加进数据库";

                query.prepare("insert into TmcAgvNum(cross,agvNum,agvTime)values(:cross,:agvNum,:agvTime)");
                query.bindValue (":cross",crossNum);
                query.bindValue (":agvNum",agvNum);
                query.bindValue (":agvTime",QTime::currentTime ().toString ("hhmmss"));
                query.exec();
            }
        }
    }

    //解锁卡
    bool succ4 = query.exec (QString("select * from TmcTable where unlockCard = '%1'").arg(agvCurrentCard));
    if(succ4 && query.first()) {

        int crossNum = query.value("cross").toInt();//交管路口
        qDebug()<<agvNum << "号车"<<QObject::tr ("读到解锁卡")<<agvCurrentCard<<"cross:"<<crossNum<<"并删除数据库信息";

        //去删除该车所在的交管信息
        bool deleSucc =  query.exec (QString("delete from TmcAgvNum where agvNum = '%1' AND cross = '%2'").arg(agvNum).arg(crossNum));

        if(deleSucc){
            //qDebug()<<"delete car in TmcAgvNum success!";
        }else{
            //qDebug()<<"delete car in TmcAgvNum failed!";
        }

        //放行交管表中的小车
        int savedAgvNum = 0;
        bool succ10=query.exec (QString("SELECT agvNum FROM TmcAgvNum where cross='%1'").arg (crossNum));
        if(succ10&&query.last())
        {
            int tmp = query.value("agvNum").toInt();//获取小车的编号
            if(savedAgvNum < tmp){
                savedAgvNum = tmp;//!fifo
            }
        }


//        while (query.next ())
//        {
//            int tmp = query.value("agvNum").toInt();//获取小车的编号
//            qDebug()<<"while cicle and agvNum = "<<tmp;
//            if(savedAgvNum < tmp){
//                savedAgvNum = tmp;//!fifo
//            }
//        }


        if(savedAgvNum != 0){
            qDebug()<<"must..release agvNum = "<<savedAgvNum;
            emit  sigControlAgv(savedAgvNum,HITAGV::Central_Unlock);
        }else{
            //qDebug()<<"savedAgvNum == 0, no car in the Tabel";
        }

    }
    Singleton<ConnectionPool>::getInstance().releaseConnection(db);
}
*/
