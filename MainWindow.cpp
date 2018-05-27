#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "DebugWindow.h"
#include "RecordWindow.h"
#include "agv/AgvCommunication.h"
#include "agv/TimeManager.h"
#include "database/ConnectionPool.h"
#include "util/ConfigUtil.h"
#include "mapshow/mapshow.h"
#include "agv/AgvShowInMap.h"
#include "agv/AgvTmcInfo.h"
#include "ModifyPwdWindow.h"
#include "station/StationCommunication.h"
#include"css/IconDraw.h"
#include"css/WidgetDraw.h"
#include "station/StationCargoInfoWindow.h"
#include "station/StationCallWindow.h"
#include "agv/TmcModifyWindow.h"
#include "agv/CrossInfoWindow.h"
#include "agv/ReadCardWindow.h"
#include "agv/StartLocationWindow.h"
#include "station/StationBtnWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   // ConfigUtil &config = Singleton<ConfigUtil>::getInstance();//读取配置文件中的内容
   // agvInitPosition=config.getAGVInitPostion();//AGV初始位置
    initMainWindow();//初始化主界面
    initStationShowItem ();//初始化工位显示信息
    initStatusBar();//初始化状态栏
    setupWidgetBarChart(ui->widget);
}
MainWindow::~MainWindow()
{
    threadAgv.quit();
    threadStation.quit ();
    threadAgv.wait();
    threadStation.wait ();

    delete ui;
}
//初始化主界面
void MainWindow::initMainWindow()
{
    agvCommunication = AgvCommunication::getInstance();
    agvTmcInfo = AgvTmcInfo::getInstance();
    stationCommunication=StationCommunication::getInstance ();

    connect(&threadAgv,&QThread::finished,agvCommunication,&AgvCommunication::deleteLater);
    agvCommunication->moveToThread(&threadAgv);
    threadAgv.start();//开启线程

    connect(&threadStation,&QThread::finished,stationCommunication,&StationCommunication::deleteLater);
    stationCommunication->moveToThread (&threadStation);
    threadStation.start ();//开启线程


    QEventLoop();

    // show map
    mapShow = new MapShow(ui->mapShowWidget);
    mapShow->resize(ui->mapShowWidget->size());
    mapShow->show();
    //开启定时器每5s检查一次AGV离线
    QTimer *timer=new QTimer(this);
    connect (timer,SIGNAL(timeout()),this,SLOT(checkonoffTimer()));
    timer->start (5000);

    QImage image,result;
    image.load(":images/agvImg3.png");
    result = image.scaled(800, 600).scaled(220, 170, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//放缩图片，以固定大小显示
    ui->agv_img_label->setPixmap(QPixmap::fromImage(result));//在Label控件上显示图片

    ui->tableWidget->setColumnCount (4);//4辆AGV小车
    ui->tableWidget->setRowCount (6);
    ui->tableWidget->setVerticalHeaderLabels(QStringList()<<QStringLiteral("AGV编号")<<QStringLiteral("当前站点")<<QStringLiteral("目标站点")<<QStringLiteral("运行状态")<<QStringLiteral("电量")<<QStringLiteral("错误信息"));
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->tableWidget->verticalHeader()->setFixedWidth (100);
    ui->tableWidget->verticalHeader()->setDefaultAlignment (Qt::AlignCenter);
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);//去掉虚框
    ui->tableWidget->verticalHeader()->setHighlightSections(false);

    for(int i=0;i<ui->tableWidget->columnCount ();i++)
    {
        QString str1 = QStringLiteral("%1号车").arg(i+1);
        QPixmap Pix;
        Pix.load(QString(":/images/AGV_%1.png").arg (i+1));
        Pix= Pix.scaled(100,100,Qt::KeepAspectRatio);//设置label的图片大小
        ui->tableWidget->setItem(0, i, new QTableWidgetItem(Pix,str1));
        ui->tableWidget->item(0,i)->setTextAlignment(Qt::AlignCenter);//文字居中显示
        ui->tableWidget->setItem(1, i, new QTableWidgetItem(tr("0")));
        ui->tableWidget->item(1,i)->setTextAlignment(Qt::AlignCenter);//文字居中显示
        ui->tableWidget->setItem(2, i, new QTableWidgetItem(tr("0")));
        ui->tableWidget->item(2,i)->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem *item = new QTableWidgetItem(QStringLiteral("获取失败"));
        item->setTextColor(QColor(255,0,0));
        ui->tableWidget->setItem(3, i,item);
        ui->tableWidget->item(3,i)->setTextAlignment(Qt::AlignCenter);//文字居中显示
        ui->tableWidget->setItem(4, i, new QTableWidgetItem(tr("0")));
        ui->tableWidget->item(4,i)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(5, i, new QTableWidgetItem(tr("0")));
        ui->tableWidget->item(5,i)->setTextAlignment(Qt::AlignCenter);
       // ui->tableWidget->setColumnWidth(i,ui->tableWidget->width()*1.2);//设置tablewidget的列宽
          ui->tableWidget->setColumnWidth(i,ui->tableWidget->width()*1.9);//设置tablewidget的列宽
    }

    for(int i=0;i<ui->tableWidget->rowCount ();i++)
    {
      //  ui->tableWidget->setRowHeight (i,ui->tableWidget->height ()*0.97);//设置tablewidget的行宽
          ui->tableWidget->setRowHeight (i,ui->tableWidget->height ()*1.3);//设置tablewidget的行宽
    }
  /*  int length=4;//agvCommunication->getAgvInfoList ().length ();//4辆小车
    for(int i=1;i<length+1;i++)
    {
        ui->agvControlComboBox->insertItem (i,QString::number (i)+"号车");
    }*/
    agvBtnGroup=new QButtonGroup(this);
    agvBtnGroup->addButton(ui->agvBtn1,0);
    agvBtnGroup->addButton(ui->agvBtn2,1);
    agvBtnGroup->addButton(ui->agvBtn3,2);
    agvBtnGroup->addButton(ui->agvBtn4,3);
    connect(agvBtnGroup,SIGNAL(buttonClicked(int)),SLOT(slotAgvBtnClicked(int)));
    agvClickId=0;

    connect(agvCommunication,&AgvCommunication::agvReceiveMessage,this,&MainWindow::slotAgvReceiveMessage);
    connect(this,&MainWindow::sigUpdateMapShowAgvLocation,mapShow,&MapShow::slotUpdateMapShowAgvLocation);
    connect (this,&MainWindow::sigControlAgv,agvCommunication,&AgvCommunication::slotControlAgv);
    connect (agvTmcInfo,&AgvTmcInfo::sigControlAgv,agvCommunication,&AgvCommunication::slotControlAgv);
    connect(this,&MainWindow::sigScheduleAgv,agvCommunication,&AgvCommunication::slotScheduleAgv );
    connect(this,&MainWindow::sigTmcTableReceive,agvTmcInfo,&AgvTmcInfo::slotTmcTable);



//    udpSocket = new QUdpSocket(this);
//    if(!udpSocket->bind(8081, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint))//建立socket连接
//    {
//        QMessageBox::warning (0,"警告2",tr("NO.8081端口绑定失败，请先关闭该端口"));
//        return;
//    }

    callRowNum = 0;

}

//运行时间统计表
void MainWindow::setupWidgetBarChart(QCustomPlot *customPlot)
{
    // set dark background gradient:
    QLinearGradient gradient(0, 0, 0, 400);
    gradient.setColorAt(0, QColor(90, 90, 90));
    gradient.setColorAt(0.38, QColor(105, 105, 105));
    gradient.setColorAt(1, QColor(70, 70, 70));
    customPlot->setBackground(QBrush(gradient));

    // create empty bar chart objects:
  // QCPBars *regen = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    QCPBars *nuclear = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    QCPBars *fossil = new QCPBars(customPlot->xAxis, customPlot->yAxis);
   // regen->setAntialiased(false); // gives more crisp, pixel aligned bar borders
    nuclear->setWidth(0.4);
    fossil->setWidth(0.4);
   nuclear->setAntialiased(false);
    fossil->setAntialiased(false);
    //regen->setStackingGap(1);
    nuclear->setStackingGap(1);
    fossil->setStackingGap(1);
    // set names and colors:
    fossil->setName(QStringLiteral("运行时间"));
    fossil->setPen(QPen(QColor(0, 255, 0).lighter(170)));
    fossil->setBrush(QColor(0, 255, 0));
   nuclear->setName(QStringLiteral("空闲时间"));
   nuclear->setPen(QPen(QColor(255, 0, 0).lighter(150)));
   nuclear->setBrush(QColor(255, 0, 0));
    // stack bars on top of each other:
    nuclear->moveAbove(fossil);
    // prepare x axis with country labels:
    QVector<double> ticks;
    QVector<QString> labels;
    ticks << 1 << 2 << 3 << 4 ;
    labels << QStringLiteral("1号车") << QStringLiteral("2号车") << QStringLiteral("3号车") << QStringLiteral("4号车") ;
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    customPlot->xAxis->setTicker(textTicker);
    customPlot->xAxis->setTickLabelRotation(60);
    customPlot->xAxis->setSubTicks(false);
    customPlot->xAxis->setTickLength(0, 4);
    customPlot->xAxis->setRange(0, 5);
    customPlot->xAxis->setBasePen(QPen(Qt::white));
    customPlot->xAxis->setTickPen(QPen(Qt::white));
    customPlot->xAxis->grid()->setVisible(true);
    customPlot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    customPlot->xAxis->setTickLabelColor(Qt::white);
    customPlot->xAxis->setLabelColor(Qt::white);

    // prepare y axis:
    customPlot->yAxis->setRange(0, 15);
    customPlot->yAxis->setPadding(5); // a bit more space to the left border
    customPlot->yAxis->setLabel(QStringLiteral("运行时间（h）"));
    customPlot->yAxis->setBasePen(QPen(Qt::white));
    customPlot->yAxis->setTickPen(QPen(Qt::white));
    customPlot->yAxis->setSubTickPen(QPen(Qt::white));
    customPlot->yAxis->grid()->setSubGridVisible(true);
    customPlot->yAxis->setTickLabelColor(Qt::white);
    customPlot->yAxis->setLabelColor(Qt::white);
   customPlot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
    customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

    // Add data:
    QVector<double> fossilData, nuclearData;
    fossilData  << 10<< 8 << 3 << 4 ;
    nuclearData << 2 << 3 << 2.5 << 1 ;
    fossil->setData(ticks, fossilData);
    nuclear->setData(ticks, nuclearData);
    customPlot->legend->setVisible(true);
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
    customPlot->legend->setBrush(QColor(255, 255, 255, 10));
    customPlot->legend->setBorderPen(Qt::NoPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    customPlot->legend->setFont(legendFont);
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void MainWindow::slotAgvBtnClicked(int buttonID)
{

    ui->agvBtn1->setStyleSheet("QPushButton{background-color:rgb(0, 255, 0)}");
    ui->agvBtn2->setStyleSheet("QPushButton{background-color:rgb(255, 255, 0)}");
    ui->agvBtn3->setStyleSheet("QPushButton{background-color:rgb(255, 85, 255)}");
    ui->agvBtn4->setStyleSheet("QPushButton{background-color:rgb(255, 101, 24)}");

    switch(buttonID)
    {
    case 0:
        agvClickId=1;
        ui->agvBtn1->setStyleSheet("QPushButton{color:red;font:13pt bold;background-color:rgb(0, 255, 0)}");

       // btn->setStyleSheet("QPushButton{color: white; font: 13pt 微软雅黑 bold; background-color:black;}"):
        break;
    case 1:
        ui->agvBtn2->setStyleSheet("QPushButton{color:red;font:13pt bold;background-color:rgb(255, 255, 0)}");
        agvClickId=2;
        break;
    case 2:
        ui->agvBtn3->setStyleSheet("QPushButton{color:red;font:13pt bold;background-color:rgb(255, 85, 255)}");
        agvClickId=3;
        break;
    case 3:
        ui->agvBtn4->setStyleSheet("QPushButton{color:red;font:13pt bold;background-color:rgb(255, 101, 24)}");
       agvClickId=4;
        break;
    default:
        agvClickId=0;
        break;
    }
}

//初始化工位显示
void MainWindow::initStationShowItem ()
{
    ui->stationInfoTableWidget->setColumnCount (3);
    ui->stationInfoTableWidget->setHorizontalHeaderLabels (QStringList()<<QStringLiteral("工位—呼叫盒")<<QStringLiteral("机台号")<<QStringLiteral("物料名称"));
    ui->stationInfoTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->stationInfoTableWidget->horizontalHeader()->setDefaultAlignment (Qt::AlignCenter);
    ui->stationInfoTableWidget->setFocusPolicy(Qt::NoFocus);//去掉虚框
    ui->stationInfoTableWidget->horizontalHeader ()->setHighlightSections(false);
    for(int i=0;i<ui->stationInfoTableWidget->columnCount();i++)
    {
//        ui->stationInfoTableWidget->setColumnWidth(i,ui->stationInfoTableWidget->width()*0.9);//设置stationInfoTableWidget的列宽
         ui->stationInfoTableWidget->setColumnWidth(i,ui->stationInfoTableWidget->width()*1.3);//设置stationInfoTableWidget的列宽
    }
  /*  for(int i=1;i<78;i++)//75个工位
    {
        ui->stationControlComboBox->insertItem (i,"工位"+QString::number (i));
    }*/
    //connect(stationCommunication,&StationCommunication::sigStationReceiveInfo,this,&MainWindow::slotStationReceiveInfo);
    connect (stationCommunication,&StationCommunication::sigControlAgv,agvCommunication,&AgvCommunication::slotControlAgv);
    connect(stationCommunication,&StationCommunication::sigStationReceiveNew,this,&MainWindow::slotStationReceiveNew);
}

//初始化状态栏
void MainWindow::initStatusBar(){
    timer=new QTimer;
    timer->setInterval (1000);
    timer->start ();
      QFont font ("", 12, 50);
    QStatusBar *statusBar = this->statusBar();
    time_statusLabel = new QLabel();
    time_statusLabel->setFont(font);
    time_statusLabel->setMinimumSize (130,20);
    info_statusLabel = new QLabel();
    info_statusLabel->setMinimumSize (400,20);

 //   QFont font ("Microsoft YaHei", 15, 30);
 //   QFont font ("Microsoft YaHei", 15, 70);
    info_statusLabel->setFont(font);
    info_statusLabel->setText (QStringLiteral("@copyright by 柳州柳工叉车有限公司"));//青岛世达智能自动化有限公司"));
    statusBar->addPermanentWidget(info_statusLabel);
    statusBar->addPermanentWidget(time_statusLabel);
    connect(timer,SIGNAL(timeout()),this,SLOT(statusbarTimer()));
}

//状态栏显示当前的时间
void MainWindow::statusbarTimer ()
{
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
    time_statusLabel->setText(current_date);
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    mapShow->resize(ui->mapShowWidget->size());//若不加此句，map大小不随窗口调整而改变
}



//定时检查最近一次接收AGV发送命令的时间，判断其是否离线
void MainWindow::checkonoffTimer ()
{
    for(int i=0;i<agvCommunication->getAgvInfoList ().length ();i++)
    {
        QString currentTime=QTime::currentTime ().toString ("hhmmss");
        QString saveReceiveTime=agvCommunication->getAgvInfoList ().at (i)->getAgvReciveTime ();
        //if((currentTime.toInt ()-saveReceiveTime.toInt ()>100)&&(currentTime.toInt ()-saveReceiveTime.toInt ()<300))
        if((currentTime.toInt ()-saveReceiveTime.toInt ()>10))
        {
            QTableWidgetItem *item = new QTableWidgetItem(QStringLiteral("获取失败"));
            item->setTextColor(QColor(255,0,0));
            ui->tableWidget->setItem(3,i, item);
            ui->tableWidget->item(3,i)->setTextAlignment(Qt::AlignCenter);

            QList<AgvInformation*> list = AgvCommunication::getInstance()->getAgvInfoList();
            int saveAgvCurrentCard=list.at (i)->getAgvCurrentStation ();
            int saveAgvTargetStationNum=list.at (i)->getAgvTargetStation ();
            int saveAgvDistance=list.at (i)->getAgvDistance ();
            int errorInfo = list.at (i)->getAgvErrorInfo ();

            //离线改变小车颜色
            PosInfo *posInfo=agvShowInMap->agvPosition (saveAgvCurrentCard);
            QPoint *agvPos =posInfo->agvPos;
            qDebug()<<QStringLiteral("离线")<<i+1<<agvPos->x ()<<agvPos->y ();
            int state=0;
            emit sigUpdateMapShowAgvLocation(i+1,agvPos->x (),agvPos->y (),posInfo->direction,state,errorInfo);
        }
    }
}

//接收的信息是Agv发送的
void MainWindow::slotAgvReceiveMessage (quint8 num)
{
    agvShowInMap=new AgvShowInMap(this);
   int agvNum,agvCurrentCard,agvSpeed,agvVoltage,agvDistance,agvDirection,agvState,agvErrorInfo,agvTargetStationNum;
   agvNum = num;//接收到第几个小车的信息
   QList<AgvInformation*> list = AgvCommunication::getInstance()->getAgvInfoList();
   agvCurrentCard=list.at (num-1)->getAgvCurrentStation ();//当前卡号
   agvSpeed=list.at (num-1)->getAgvCurrentSpeed ();//当前速度
   agvVoltage=list.at (num-1)->getAgvVoltage ();//当前电压
   agvDistance=list.at (num-1)->getAgvDistance ();//移动距离
   agvDirection=list.at(num-1)->getAgvDirection();//移动方向
   agvState=list.at (num-1)->getAgvCurrentStatus ();//当前状态
   agvErrorInfo=list.at (num-1)->getAgvErrorInfo ();//错误信息
   agvTargetStationNum=list.at (num-1)->getAgvTargetStation ();//AGV目标工位号
     //打开数据库
   QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
   QSqlQuery query(db);
   quint16 startCardNum;
   query.exec (QString("select startCardNum from StartLocationCard where agvNum='%1'").arg(agvNum));
   while (query.next ()) {
       startCardNum= query.value("startCardNum").toInt();//获取起始点的卡号
   }
   Singleton<ConnectionPool>::getInstance().releaseConnection(db);
   if(agvTargetStationNum==0 || agvTargetStationNum == 99)
   {

       agvTargetStationNum=startCardNum;
      // agvTargetStationNum=agvInitPosition;//若AGV没有工位，则AGV行使到起始位置  起始位置卡号为100
   }

   QString errorInfo=agvSymbol->errorInfo (agvErrorInfo);
   QString state = agvSymbol->getAgvStatus (agvState);
  // if(agvCurrentCard == agvInitPosition){
   if(agvCurrentCard ==startCardNum ){
       //qDebug()<<"agv NUM:" << agvNum << "arrive at init position";
      // QString st = "起始站点";
       ui->tableWidget->setItem (1,agvNum-1,new QTableWidgetItem(QStringLiteral("起始站点")));//QStringLiteral("起始站点")));//当前站点
   }else{
       ui->tableWidget->setItem (1,agvNum-1,new QTableWidgetItem(QString::number(agvCurrentCard)));//当前站点
   }

   QTableWidgetItem *item = new QTableWidgetItem(state);
   QString agvElectric=QString::number(agvVoltage*2/10);
   agvElectric.append("V");
   item->setTextColor(QColor(0,255,0));
   ui->tableWidget->setItem(3,agvNum-1, item);
   ui->tableWidget->setItem(2,agvNum-1,new QTableWidgetItem(QString::number(agvTargetStationNum)));
   ui->tableWidget->setItem(4,agvNum-1,new QTableWidgetItem(agvElectric));
   ui->tableWidget->setItem (5,agvNum-1,new QTableWidgetItem(errorInfo));
   for(int i=0;i<ui->tableWidget->rowCount ();i++)
   {
       ui->tableWidget->item(i,agvNum-1)->setTextAlignment(Qt::AlignCenter);
   }


   PosInfo *posInfo;
   QPoint *agvPos=new QPoint(0,0);
   //  posInfo=agvShowInMap->agvPosition (agvCurrentCard,agvTargetStationNum,agvDistance);




   posInfo=agvShowInMap->agvPosition (agvCurrentCard);
   agvPos= posInfo->agvPos;
   int intState=1;//小车在线intState=1;离线intState=0;主要是为了在不同情况下小车的颜色不同
   emit sigUpdateMapShowAgvLocation(agvNum,agvPos->x (),agvPos->y (),posInfo->direction,intState,agvErrorInfo);
   //查看agv当前的卡号是否处于交管表中
   emit sigTmcTableReceive(agvNum,agvCurrentCard,agvDirection);

   //查看agv当前的卡号是否为双向交管表中
  //  emit sigDoubleTmcTableReceive(agvNum,agvCurrentCard,agvDirection);

  /* bool isFiFo;
   if(ui -> crossTmcFIFOAction)
   {
       isFiFo = true;
   }
   else if(ui->crossTmcMaxNumAction)
   {
       isFiFo = false;
   }
   emit sigTmcTableReceive(agvNum,agvCurrentCard,agvDirection,isFiFo);*/
}

void MainWindow::slotStationReceiveNew(StationInformation *staNew)
{
    QString staNumSt = QString::number ( staNew->getStationNum() );
    QList <quint8> *callList = &(staNew ->callBoxList);

    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlQuery query(db);

    for(int i=0; i < callList->size(); i++)
    {
        quint8 call = callList->at(i);
        if(call != 0)
        {
            //接收新的呼叫盒，存入数据库
            query.prepare("insert into StationCallTable(station,callBox,time,stramp)values(:station,:callBox,:time,:stramp)");
            query.bindValue(":station",staNumSt.toInt());
            query.bindValue(":callBox",i+1);
            query.bindValue(":time",QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            query.bindValue(":stramp",QDateTime::currentDateTime().toTime_t());
            bool suc11=query.exec();
            if (suc11)
            {
                qDebug()<<QStringLiteral("保存工位呼叫信息成功,工位号：")<<staNumSt<<QStringLiteral("呼叫盒编号：")<<i+1;
            }
            else
            {
                qDebug()<<QStringLiteral("保存工位呼叫信息失败");
            }

            callRowNum ++;
            ui->stationInfoTableWidget->setRowCount(callRowNum);

            //保存的形式  工位编号-呼叫盒编号
            QString staCall = staNumSt+"-"+QString::number( i + 1);
            ui->stationInfoTableWidget->setItem (callRowNum-1,0,new QTableWidgetItem(staCall));
            StationInfo *stationInfo = stationCommunication->StationNameInfo(staNumSt.toInt(),i+1);
            QString boardName = stationInfo->boardName;
            QString cargoName = stationInfo->cargoName;

            ui->stationInfoTableWidget->setItem (callRowNum-1,1,new QTableWidgetItem(boardName));
            ui->stationInfoTableWidget->setItem (callRowNum-1,2,new QTableWidgetItem(cargoName));
        }
    }
    Singleton<ConnectionPool>::getInstance().releaseConnection(db);
}

//接收的信息是Station发送的 %%%%未用到%%%%
//num ：infolist 的num, 元素为 StationInformation
void MainWindow::slotStationReceiveInfo (quint8 num)
{
    //ui->stationInfoTableWidget->setRowCount (list.size());
    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlQuery query(db);
    QList<StationInformation*> list=StationCommunication::getInstance ()->getStationInfoList ();
    if(num != 0){
        ui->stationInfoTableWidget->setRowCount (list.size());
    }

    int rowNum=0;
    for(int i=0;i<num;i++)
    {
        QString stationNum=QString::number (list.at (i)->getStationNum ());
        QList <int> *numsList = new QList<int>();

        int callBox1Num = list.at (i)->getCallBox1Num ();
        numsList ->append (callBox1Num);
        int callBox2Num = list.at (i)->getCallBox2Num ();
        numsList ->append (callBox2Num);
        int callBox3Num = list.at (i)->getCallBox3Num ();
        numsList ->append (callBox3Num);
        int callBox4Num = list.at (i)->getCallBox4Num ();
        numsList ->append (callBox4Num);
        int callBox5Num = list.at (i)->getCallBox5Num ();
        numsList ->append (callBox5Num);
        int callBox6Num = list.at (i)->getCallBox6Num ();
        numsList ->append (callBox6Num);

        int times = 0;
        int t = 0;
        for (t = 0; t < numsList->size(); ++t)
        {
            int call = numsList->at (t);
            if (call != 0)
            {//接收到新的地标卡时，存入数据库

                /*
                    query.prepare("insert into startmp(station,callBox,time)values(:station,:callBox,:time)");
                    query.bindValue(":station",stationNum);
                    query.bindValue(":callBox",t+1);
                    query.bindValue(":time",QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
                    query.bindValue(":stramp",QDateTime::currentDateTime().toTime_t());
                    bool suc11=query.exec();
                    if (suc11)
                    {
                        qDebug()<<"保存工位呼叫信息成功,工位号："<<stationNum<<"呼叫盒编号："<<t;
                    }
                    else
                    {
                        qDebug()<<"保存工位呼叫信息失败";
                    }
                */

                times ++;
                ui->stationInfoTableWidget->setRowCount(rowNum+1);

                //保存的形式  工位编号-呼叫盒编号
                ui->stationInfoTableWidget->setItem (rowNum,0,new QTableWidgetItem(stationNum+"-"+QString::number(t+1)));

                StationInfo *stationInfo=stationCommunication->StationNameInfo(stationNum.toInt(),t+1);
                QString boardName=stationInfo->boardName;
                QString cargoName=stationInfo->cargoName;

                ui->stationInfoTableWidget->setItem (rowNum,1,new QTableWidgetItem(boardName));
                ui->stationInfoTableWidget->setItem (rowNum,2,new QTableWidgetItem(cargoName));
                rowNum++;
            }
        }

        qDebug()<<"staion index"<<stationNum<<"caller list not == 0 size :"<<times;

    }
       Singleton<ConnectionPool>::getInstance().releaseConnection(db);
}
//AGV送货按钮
void MainWindow::on_stationReleaseBtn_clicked ()
{
    int mm = ui->stationInfoTableWidget->rowCount ();
    if(mm==0)
    {
        qDebug()<<"StationInfoTableWidget为空";
        return;
    }

    QList<QTableWidgetItem*> selectedItemList = ui->stationInfoTableWidget->selectedItems ();//获取选中的列表
    int selectedCounts = selectedItemList.count();
    if(selectedCounts == 0)
    {
        qDebug()<<QStringLiteral("未选中任意一行， 程序返回");
        QMessageBox::warning(NULL,QStringLiteral("警告"),QStringLiteral("您未选择需要到达的工位"));
        return;
    }

    qDebug()<<QStringLiteral("选中行数")<<selectedCounts/3;

    QList <int> *selectedStationList = new QList<int>();
    QList <int> *selectedCallerList = new QList<int>();
    QList <int> *selectedRowList = new QList<int>();

    QList <QString> *selectedTextList = new QList<QString>();

    //多行选中
    for(int i = 0;i < selectedCounts / 3; i++)
    {
        QTableWidgetItem *selectedItem = selectedItemList.at (i * 3);
        int selectedRow = ui->stationInfoTableWidget->row (selectedItem);//获取选中所在的行号
        selectedRowList->append(selectedRow);

        QString selectedText = selectedItem->text ();//行号所在行数的内容、、ex：23-1
        selectedTextList->append(selectedText);

        QString stationName;
        int s = selectedText.indexOf("-");//至"-"的下标
        stationName = selectedText.mid(0,s);//获取工位名称
        int stationNum = stationName.toInt();
        selectedStationList->append(stationNum);//工位名称添加至表格中


        int callerNum = selectedText.mid(s+1).toInt();//获取工位盒编号
        selectedCallerList->append(callerNum);//呼叫和名称添加至表格中
    }

    bool equal = true;
    //int index = 0;判断选中的工位编号是否是同一个工位
    quint8 stationName = (quint8)selectedStationList->at(0);
    for(int i = 0; i < selectedStationList->count(); i++)
    {
        int tmp = selectedStationList->at(i);
        if(stationName != tmp){
            equal = false;
            break;
        }
    }



    if(equal)
    {

        QByteArray station;
        station.append(char(stationName));
        QList <AgvInformation *> agvList = AgvCommunication::getInstance()->getAgvInfoList();

        int num=0;
        quint8 agvNum;
        for (int i = 0; i < agvList.size(); i++)
        {
            AgvInformation *agv = agvList.at(i);
            //查看处在起始位置且没有出错的小车

            if (agv->getAgvCurrentStation() == agvInitPosition && agv->getAgvErrorInfo() == 0 )
            {
                num++;
                agvNum = agv->getAgvNum() ;
                break ;
            }
        }

        if(num == 0)
        {
            QMessageBox::warning(NULL,QStringLiteral("警告"),QStringLiteral("起始位置处没有小车,或者小车有错误，请检查后再启动小车"));
            return;
        }
        else
        {
            qDebug()<<QStringLiteral("1调度AGV")<<agvNum<<QStringLiteral("至工位：")<<stationName ;
            emit sigScheduleAgv(agvNum,station) ;
        }

        //QList<StationInformation*> *stationList = &(StationCommunication::getInstance () -> getStationInfoList ());

        QString stationNumSt = QString::number ((quint8) stationName );
        QMap<QString, QList<quint8> > * mapStaCaller = &(StationCommunication::getInstance ()->mapStaCaller);
        QList <quint8> callEx = mapStaCaller->value(stationNumSt);

        /*
        for(int i = 0; i < stationList->size(); i++){
            StationInformation *info = stationList->at(i);
            if(info->getStationNum() == stationName){
                stationList->removeOne(info);
            }
        }
        */

        for(int i = 0; i < selectedRowList->count(); i++){

            QTableWidgetItem *selectedItem = selectedItemList.at (i * 3);
            int selectedRow = ui->stationInfoTableWidget->row (selectedItem);//获取选中所在的行号

            QString selectedText = selectedItem->text ();//行号所在行数的内容、、ex：23-1
            qDebug()<<QStringLiteral("删除行号")<<selectedRow<<QStringLiteral("名称")<<selectedText;

            QString stationNumS;
            int s = selectedText.indexOf("-");
            stationNumS = selectedText.mid(0,s);
            quint8 callerNum = (quint8)selectedText.mid(s+1).toInt();


            callEx.replace(callerNum-1, 0);

            for(int t = 0; t< callEx.size(); t++)
            {
                if(callEx.at(t)!= 0)
                 qDebug()<<":"<<t;
            }

            ui->stationInfoTableWidget -> removeRow(selectedRow);
            callRowNum --;

            /*
             * 很奇怪这样不行？。。
            int tmpRow = selectedRowList->at(i);
            qDebug()<<"删除行号"<<tmpRow;//<<"名称"<<selectedText;

            ui->stationInfoTableWidget->removeRow(tmpRow);

            stationList->removeAt(tmpRow);

            qDebug()<<"stationInfoList Size3:"<<stationList->count();
            */

        }

        if( mapStaCaller->contains(stationNumSt) )
        {
            mapStaCaller->insert(stationNumSt, callEx);
            qDebug()<<"yew contains";
        }
        else
        {
            qDebug()<<"increadable wrong";
        }


        /*
        for(int i = 0; i < selectedTextList->size(); i++)
        {

            QString selectedText = selectedTextList->at(i);
            int s = selectedText.indexOf("-");
            int stationNum = selectedText.mid(0,s).toInt();
            int callerNum = selectedText.mid(s+1).toInt();


            //int indexStation = stationList->indexOf()

            //设置stationNum 的callerNum == 0；
            for (int j = 0 ; j < stationList->size(); j++)
            {
                StationInformation *info = stationList->at(j);
                if(info -> getStationNum() == stationNum)
                {
                    QList< quint8 > *list = &(info->callBoxList);

                    for(int t = 0; t < list->size(); t++)
                    {
                        quint8 callNum = list->at(t);
                        if(callNum == callerNum)
                        {
                            list->replace(callNum-1,0);
                        }
                    }

                    switch (callerNum) {
                    case 1:
                        info->setCallBox1Num(0);
                        break;
                    case 2:
                        info->setCallBox2Num(0);
                        break;
                    case 3:
                        info->setCallBox3Num(0);
                        break;
                    case 4:
                        info->setCallBox4Num(0);
                        break;
                    case 5:
                        info->setCallBox5Num(0);
                        break;
                    case 6:
                        info->setCallBox6Num(0);
                        break;
                    default:
                        break;
                    }

                }

                QList< quint8 > *list = &(info->callBoxList);
                bool allZero = true;
                for(int t = 0; t < list->size(); t++){
                    quint8 callNum = list->at(t);
                    if(callNum != 0){
                        allZero = false;
                    }
                }

                if(allZero){//说明全都为0，删除该条记录
                    stationList->removeOne(info);
                }
            }

        }
        */


    }else{
        QMessageBox::warning(NULL,QStringLiteral("警告"),QStringLiteral("您选择的工位号不一致，AGV无法发货！"));
        return;
    }

}
//删除选中按钮
void MainWindow::on_pushButton_clicked()
{
    int stationTotalCount = ui -> stationInfoTableWidget -> rowCount ();
    if(stationTotalCount == 0)
    {
        qDebug()<<"StationInfoTableWidget 为空";
         QMessageBox::warning(NULL,QStringLiteral("警告"),QStringLiteral("工位列表为空"));
        return;
    }

    QList<QTableWidgetItem*>selectedItemList = ui->stationInfoTableWidget->selectedItems ();

    //多选选中删除
    int selectedCount = selectedItemList.count ();
    if(selectedCount == 0){
        qDebug()<<QStringLiteral("未选中任意一行， 程序返回");
        QMessageBox::warning(NULL,QStringLiteral("警告"),QStringLiteral("您未选择需要到达的工位"));
        return;
    }

    qDebug() << QStringLiteral("清除选中行数：") << selectedCount / 3;
    //QList<StationInformation*> *list = &(StationCommunication::getInstance ()->getStationInfoList ());
    QMap<QString, QList<quint8> >*mapStaCaller = &(StationCommunication::getInstance ()->mapStaCaller);

    for(int i = 0;i < selectedCount / 3; i++)
    {
        QTableWidgetItem *selectedItem = selectedItemList.at (i*3);
        int selectedRow = ui->stationInfoTableWidget->row (selectedItem);//获取选中所在的行号

        QString selectedText = selectedItem->text ();//行号所在行数的内容
        qDebug()<<QStringLiteral("删除行号2")<<selectedRow<<QStringLiteral("名称2")<<selectedText;

        int s = selectedText.indexOf("-");
        QString stationNumSt = selectedText.mid(0,s);//工位编号
        int callerNum = selectedText.mid(s+1).toInt();//呼叫盒编号

        QList <quint8> callEx = mapStaCaller->value(stationNumSt);
        callEx.replace(callerNum-1, 0);
        mapStaCaller->insert(stationNumSt, callEx);

        ui->stationInfoTableWidget -> removeRow(selectedRow);
        callRowNum --;

        //list->removeAt(selectedRow);
    }

}
//实时监测在起始位置上的小车
void MainWindow::stationReleaseTimer (){
    QByteArray station;
    station.append(char(stationReleaseList[0]));//若list表中存在多个station放行的信息，应先响应首个放行的station
    QList <AgvInformation *> agvList = AgvCommunication::getInstance()->getAgvInfoList();
    int num=0;
    //打开数据库
  QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
  QSqlQuery query(db);
  quint16 startCardNum;
    for (int i = 0; i < agvList.size(); i++)
    {
        AgvInformation *agv = agvList.at(i);

      query.exec (QString("select startCardNum from StartLocationCard where agvNum='%1'").arg(i+1));
      while (query.next ()) {
          startCardNum= query.value("startCardNum").toInt();//获取起始点的卡号
      }

       // if (agv->getAgvCurrentStation() ==agvInitPosition ){
      if (agv->getAgvCurrentStation() ==startCardNum ){
            num++;
            quint8 agvNum = agv->getAgvNum();
            qDebug()<<QStringLiteral("2调度AGV")<<agvNum<<QStringLiteral("至工位：")<<stationReleaseList[0];
            emit sigScheduleAgv(agvNum,station);
            stationReleaseList.removeAt (0);
            if(stationReleaseList.length ()==0)//list表中无内容，则关闭定时器，否则继续开启查询下一个停在起始位置上的小车
            {
                   stationTimer->stop ();
            }
        }
    }
     Singleton<ConnectionPool>::getInstance().releaseConnection(db);
}

//启动工位按钮
//前提：agv在起始位置且该车未出错
void MainWindow::on_stationStartBtn_clicked()
{

    int agvNum=agvClickId;
    if(agvNum==0)
    {
        QMessageBox::warning(NULL,QStringLiteral("警告"),QStringLiteral("请先选择需要操作的小车"));
        return;

    }
         //   = ui->agvControlComboBox->currentText ().remove (QRegularExpression("[^0-9]")).toInt ();
   // int stationNum = ui->stationControlComboBox->currentText().remove(QRegularExpression("[^0-9]")).toInt();
    int stationNum=0;
     stationNum= ui->selectStationBtn->text().remove(QRegularExpression("[^0-9]")).toInt();
    if(stationNum==0)
    {
         QMessageBox::warning(NULL,QStringLiteral("警告"),QStringLiteral("未选择工位编号，无法调度小车"));
         return;
    }
    QByteArray station;
    station.append(char(stationNum));

    QList <AgvInformation *> agvList = AgvCommunication::getInstance()->getAgvInfoList();
    AgvInformation *agv = agvList.at(agvNum-1);

    //打开数据库
    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlQuery query(db);
    quint16 startCardNum;
    query.exec (QString("select startCardNum from StartLocationCard where agvNum='%1'").arg(agvNum));
    while (query.next ()) {
        startCardNum= query.value("startCardNum").toInt();//获取起始点的卡号
    }
    Singleton<ConnectionPool>::getInstance().releaseConnection(db);


   // if ((agv->getAgvCurrentStation() == agvInitPosition )&&(agv->getAgvErrorInfo()==0)){
    if ((agv->getAgvCurrentStation() == startCardNum )&&(agv->getAgvErrorInfo()==0)){
        qDebug()<<QStringLiteral("3调度AGV")<<agvNum<<QStringLiteral("至工位：")<<stationNum;
        emit sigScheduleAgv(agvNum,station);
    }else{
        QMessageBox::warning(NULL,QStringLiteral("警告"),QStringLiteral("所选AGV不在起始位置或所选AGV小车有错误，无法调度"));
    }
}

//agv启动按钮
void MainWindow::on_startButton_clicked()
{
    int agvNum=agvClickId;
    if(agvNum==0)
    {
        QMessageBox::warning(NULL,QStringLiteral("警告"),QStringLiteral("请先选择需要操作的小车"));
        return;

    }
          //  =ui->agvControlComboBox->currentText ().remove (QRegularExpression("[^0-9]")).toInt ();
    QString str=ui->tableWidget->item (3,agvNum-1)->text ();
    if(str==("获取失败")){//asdfQString::fromLocal8Bit
        WidgetDraw::ShowMessageBoxWarning (QStringLiteral("该车已离线"));
    }
    else{
       // emit sigControlAgv (agvNum,HITAGV::Central_Start);
         emit  sigDoubleControlAgv(agvNum,HITAGV::Central_Start,HITAGV::Central_Front);
    }
}
//agv反向启动
void MainWindow::on_startButton2_clicked()
{
    int agvNum=agvClickId;
    if(agvNum==0)
    {
        QMessageBox::warning(NULL,QStringLiteral("警告"),QStringLiteral("请先选择需要操作的小车"));
        return;

    }
    //ui->agvControlComboBox->currentText ().remove (QRegularExpression("[^0-9]")).toInt ();
    QString str=ui->tableWidget->item (3,agvNum-1)->text ();
    if(str==("获取失败")){//asdfQString::fromLocal8Bit
        WidgetDraw::ShowMessageBoxWarning (QStringLiteral("该车已离线"));
    }
    else{
        emit  sigDoubleControlAgv(agvNum,HITAGV::Central_Start,HITAGV::Central_Back);

      //  emit sigControlAgv (agvNum,HITAGV::Central_Stop);
    }
}


//agv停止按钮
void MainWindow::on_stopBtn_clicked()
{
    int agvNum=agvClickId;
    if(agvNum==0)
    {
        QMessageBox::warning(NULL,QStringLiteral("警告"),QStringLiteral("请先选择需要操作的小车"));
        return;

    }
    //ui->agvControlComboBox->currentText ().remove (QRegularExpression("[^0-9]")).toInt ();
    QString str=ui->tableWidget->item (3,agvNum-1)->text ();
    if(str==("获取失败")){//asdfQString::fromLocal8Bit
        WidgetDraw::ShowMessageBoxWarning (QStringLiteral("该车已离线"));
    }
    else{
        emit sigControlAgv (agvNum,HITAGV::Central_Stop);
    }
}

//agv复位按钮
void MainWindow::on_resetBtn_clicked()
{
    int agvNum=agvClickId;
    if(agvNum==0)
    {
        QMessageBox::warning(NULL,QStringLiteral("警告"),QStringLiteral("请先选择需要操作的小车"));
        return;

    }
    //ui->agvControlComboBox->currentText ().remove (QRegularExpression("[^0-9]")).toInt ();
    QString str=ui->tableWidget->item (3,agvNum-1)->text ();
    if(str==("获取失败")){//asdfQString::fromLocal8Bit
        WidgetDraw::ShowMessageBoxWarning (QStringLiteral("该车已离线"));
    }
    else{
        emit sigControlAgv (agvNum,HITAGV::Central_Reset);
    }
}

//agv解锁按钮
void MainWindow::on_unlockedBtn_clicked()
{
    int agvNum=agvClickId;
    if(agvNum==0)
    {
        QMessageBox::warning(NULL,QStringLiteral("警告"),QStringLiteral("请先选择需要操作的小车"));
        return;

    }
    //ui->agvControlComboBox->currentText ().remove (QRegularExpression("[^0-9]")).toInt ();
    QString str=ui->tableWidget->item (3,agvNum-1)->text ();
    if(str==("获取失败"))//asdfQString::fromLocal8Bit
    {
        WidgetDraw::ShowMessageBoxWarning (QStringLiteral("该车已离线"));
    }
    else
    {

        //删除交管信息
        QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
        QSqlQuery query(db);
        //清空信息表
        bool deleSucc =  query.exec (QString("delete from TmcAgvNum"));
        if(deleSucc){
            qDebug()<<"Delete * In TmcAgvNum Tabel Success!";
            emit sigControlAgv (agvNum,HITAGV::Central_Unlock);
        }else{
            qDebug()<<"Delete * In TmcAgvNum Tabel Failed!";
            QMessageBox::warning(NULL,QStringLiteral("警告"),QStringLiteral("解锁失败，请尝试重新解锁"));
        }
        /*
        bool succ4 = query.exec (QString("select * from TmcAgvNum where agvNum = '%1'").arg(agvNum));
        while(succ4 && query.next()){
        //if(succ4 && query.first()) {
            int crossNum = query.value("crossid").toInt();//交管路口
            qDebug()<<"agvNum:"<<agvNum<<QObject::tr ("删除路口信息")<<"cross:"<<crossNum;

            //去删除该车所在的交管信息
            bool deleSucc =  query.exec (QString("delete from TmcAgvNum where cross = '%1'").arg(crossNum));
            if(deleSucc){
                qDebug()<<"delete car in TmcAgvNum success!";
            }else{
                qDebug()<<"delete car in TmcAgvNum failed!";
            }
        }
        */
        Singleton<ConnectionPool>::getInstance().releaseConnection(db);

    }
}

//窗口右上角关闭按钮
void MainWindow::closeEvent (QCloseEvent *event)
{
    int returnValue =WidgetDraw::ShowMessageBoxQuesion (QStringLiteral("确定关闭所有窗口并退出吗？"));
    if(returnValue == 1)
    {
        QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
        QSqlQuery query(db);
        for (int i=1; i<ui->tableWidget->columnCount ()+1; i++)
        {
            query.exec (QString("update AgvMessage set agvIpAddress=%1,agvCurrentCard=%2,agvReceiveTime=%3 where agvNum=%4").arg (0).arg (0).arg (290000).arg(i));
        }
        query.exec (QString("delete from TmcAgvNum"));//删除交管表中的数据
        query.exec(QString("delete from DoubleTmcAgvNum"));//删除双向交管卡的数据
        uint stramp=QDateTime::currentDateTime().toTime_t();
        uint temp=stramp-(5*24*60*60);//2天记录

        //query.exec(QString("delete from startmp "));
        query.exec(QString("delete from startmp where stramp <= %1").arg(temp));
        query.exec(QString("delete from ReadCardTable where stramp <= %1").arg(temp));//删除七天以上的数据

        Singleton<ConnectionPool>::getInstance().releaseConnection(db);

        QCoreApplication::quit();

        event->accept ();

    }else{

        event->ignore ();
    }
}

/*
void MainWindow::on_crossTmcFIFOAction_triggered()
{
    qDebug()<<"选择先进先出模式";
    ui->crossTmcMaxNumAction->setChecked(false);
    ui->crossTmcFIFOAction->setChecked(true);
}
//关闭交管
void MainWindow::on_crossTmcMaxNumAction_triggered()
{
    qDebug()<<"选择大号先行模式";
    ui->crossTmcMaxNumAction->setChecked(true);
    ui->crossTmcFIFOAction->setChecked(false);
}

void MainWindow::on_crossTmcCloseAction_triggered()
{
    if(ui->crossTmcCloseAction->isChecked())
    {
        qDebug()<<"关闭交管";
        ui->crossTmcMaxNumAction->setChecked(false);
        ui->crossTmcFIFOAction->setChecked(false);
        ui->crossTmcMaxNumAction->setDisabled(true);
        ui->crossTmcFIFOAction->setDisabled(true);
        //向交管表里面的AGV小车发送复位命令并删除交管表中的信息
        QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
        QSqlQuery query(db);
        query.exec (QString("SELECT agvNum FROM TmcAgvNum"));
        int agvNum;
        while (query.next ())
        {
            agvNum=query.value (0).toInt ();//获取小车IP地址
            emit sigControlAgv (agvNum,HITAGV::Central_Reset);
        }
        query.exec (QString("delete from TmcAgvNum"));//删除交管表中的数据
        Singleton<ConnectionPool>::getInstance().releaseConnection(db);
    }
    else{
        ui->crossTmcMaxNumAction->setDisabled(false);
        ui->crossTmcFIFOAction->setDisabled(false);
        ui->crossTmcMaxNumAction->setChecked(false);
        ui->crossTmcFIFOAction->setChecked(true);//默认交管为先进先出模式
        qDebug()<<"开启交管，默认选择先进先出模式";
    }
}
//AGV任务记录窗口
void MainWindow::on_agvTaskRecordAction_triggered()
{
    if(!AgvSymbol::haveInitDepartureWindow){
        departureRecordWindow = new RecordWindow;
        departureRecordWindow->whichWindow = DepartureWindow;
        departureRecordWindow->initDepartureAgvWindowShow();
        departureRecordWindow->move(400,300);
        departureRecordWindow->show();
        AgvSymbol::haveInitDepartureWindow = true;
    }
    else{
        if(departureRecordWindow->isHidden())
            departureRecordWindow->show();
        else
            departureRecordWindow->hide();
    }
}


//电池更换记录窗口
void MainWindow::on_batteryRecordAction_triggered()
{
    if(!AgvSymbol::haveInitBatteryWindow){
        batteryRecordWindow = new RecordWindow;
        batteryRecordWindow->whichWindow = BatteryWindow;
        batteryRecordWindow->initBatteryWindowShow();
        batteryRecordWindow->move(400,300);
        batteryRecordWindow->show();
        AgvSymbol::haveInitBatteryWindow = true;
    }
    else{
        if(batteryRecordWindow->isHidden())
            batteryRecordWindow->show();
        else
            batteryRecordWindow->hide();
    }
}
//障碍生效检测
void MainWindow::on_barrierAction_triggered()
{
    if(ui->barrierAction->isChecked ()){

    }
}*/
//交管信息修改窗口
void MainWindow::on_tmcInfoAction_triggered()
{
    tmcModifyWindow=new TmcModifyWindow;
    tmcModifyWindow->show();
}
//路口信息窗口
void MainWindow::on_crossInfoAction_triggered()
{
    crossInfoWindow=new CrossInfoWindow;
    crossInfoWindow->show();

}

//工位-物料名修改窗口
void MainWindow::on_stationCargoAction_triggered()
{
stationCargoInfoWindow=new StationCargoInfoWindow;
stationCargoInfoWindow->show ();
}
//工位呼叫窗口
void MainWindow::on_actionCall_triggered()
{
stationCallWindow=new StationCallWindow;
stationCallWindow->show();

}
//地标卡信息窗口
void MainWindow::on_readCardAction_triggered()
{
    readCardWindow=new ReadCardWindow;
    readCardWindow->show();
}
//UDP接收发送信息显示窗口
void MainWindow::on_debugAction_triggered()
{
    debugWindow=new DebugWindow;
    debugWindow->show ();
    connect(agvCommunication,&AgvCommunication::sigSendContextToShow,debugWindow,&DebugWindow::slotSendContext);
    connect(agvCommunication,&AgvCommunication::sigReceiveContextToShow,debugWindow,&DebugWindow::slotReceiveContext);
    connect(stationCommunication,&StationCommunication::sigSendContextToShow,debugWindow,&DebugWindow::slotSendContext);
    connect(stationCommunication,&StationCommunication::sigReceiveContextToShow,debugWindow,&DebugWindow::slotReceiveContext);
}


//窗口最大化
/*void MainWindow::on_maxSizeAction_triggered()
{
    showMaximized();
}*/
//窗口最小化
void MainWindow::on_miniSizeAction_triggered()
{
    showMinimized();
}

//修改密码窗口
void MainWindow::on_modifyPwdAction_triggered()
{
    modifyPwdWindow=new ModifyPwdWindow;
    modifyPwdWindow->show ();
}

//关于
void MainWindow::on_aboutAction_triggered()
{
    QMessageBox message(QMessageBox::NoIcon,QStringLiteral("关于"),QStringLiteral("\n""\n""版权所有青岛世达智能自动化有限公司\n"
                                                "公司网址:http://www.st-auto.cn/\n"
                       ));
    QImage image;
    QImage result;
    image.load(":images/logo.png");
    result = image.scaled(800, 600).scaled(100, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//放缩图片，以固定大小显示
    message.setIconPixmap(QPixmap::fromImage (result));
    message.exec();
}
//公司网址
void MainWindow::on_urlAction_triggered()
{
    QDesktopServices::openUrl(QUrl(QLatin1String("http://www.st-auto.cn/")));
}







void MainWindow::on_startLocationAction_triggered()
{
    startLocationWindow=new StartLocationWindow();
    startLocationWindow->show();

}


void MainWindow::on_selectStationBtn_clicked()
{
    stationBtnWindow=new StationBtnWindow();
    connect(stationBtnWindow,&StationBtnWindow::sendSelectStationInfo,this,&MainWindow::receiveSelectStationInfo);
    stationBtnWindow->show();
}
void MainWindow::receiveSelectStationInfo(QString data)
{
    ui->selectStationBtn->setText(data);
}
