#include "MainPage.h"
#include "ui_MainPage.h"

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


MainPage::MainPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainPage)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("Agv调度控制系统"));

    monitorPage=new MonitorPage(this);
    //statisticsPage=new StatisticsPage(this);
    aboutPage=new AboutPage(this);
    debugWindow=new DebugWindow;
    modifyPwdWindow=new ModifyPwdWindow();
    startLocationWindow=new StartLocationWindow();
    tmcModifyWindow=new TmcModifyWindow();
    crossInfoWindow=new CrossInfoWindow();
    stationCallWindow=new StationCallWindow();
    stationCargoInfoWindow=new StationCargoInfoWindow();
    readCardWindow=new ReadCardWindow();
    dataStatisticsPage=new DataStatisticsPage();
    statisticShowPage=new StatisticShowPage();

    ui->stackedWidget->addWidget(monitorPage);
    ui->stackedWidget->addWidget(statisticShowPage);
    ui->stackedWidget->addWidget(aboutPage);
    ui->stackedWidget->addWidget(debugWindow);
    ui->stackedWidget->addWidget(modifyPwdWindow);
    ui->stackedWidget->addWidget(startLocationWindow);
    ui->stackedWidget->addWidget(tmcModifyWindow);
    ui->stackedWidget->addWidget(crossInfoWindow);
    ui->stackedWidget->addWidget(stationCallWindow);
    ui->stackedWidget->addWidget(stationCargoInfoWindow);
    ui->stackedWidget->addWidget(dataStatisticsPage);
    ui->stackedWidget->addWidget(readCardWindow);
    ui->stackedWidget->setCurrentWidget(monitorPage);

    ui->monitor_pagebtn->setStyleSheet("QPushButton{background-color:rgb(0, 255, 0)}");

    initMainWindow();
    initStatusBar();
}

MainPage::~MainPage()
{
    threadAgv.quit();
    threadStation.quit();
    threadAgv.wait();
    threadStation.wait();
    delete ui;
}

//初始化状态栏
void MainPage::initStatusBar()
{
    timer=new QTimer;
    timer->setInterval (1000);
    timer->start ();
    QFont font ("宋体", 12, 50);
    QStatusBar *statusBar = this->statusBar();
    time_statusLabel = new QLabel();
    time_statusLabel->setFont(font);
    time_statusLabel->setMinimumSize (130,20);
    info_statusLabel = new QLabel();
    info_statusLabel->setMinimumSize (400,20);
    info_statusLabel->setFont(font);
    info_statusLabel->setText ("@copyright by" + QStringLiteral(" 芜湖哈特机器人产业技术研究院有限公司"));
    statusBar->addPermanentWidget(info_statusLabel);
    statusBar->addPermanentWidget(time_statusLabel);
    connect(timer,SIGNAL(timeout()),this,SLOT(statusbarTimer()));
}
//初始化
void MainPage::initMainWindow()
{
    agvCommunication = AgvCommunication::getInstance();
    agvTmcInfo = AgvTmcInfo::getInstance();
    stationCommunication=StationCommunication::getInstance ();


 //AGV连接线程
    connect(&threadAgv,&QThread::finished,agvCommunication,&AgvCommunication::deleteLater);
    agvCommunication->moveToThread(&threadAgv);
    threadAgv.start();//开启线程

    connect(&threadStation,&QThread::finished,stationCommunication,&StationCommunication::deleteLater);
    stationCommunication->moveToThread (&threadStation);
    threadStation.start ();//开启线程


    QEventLoop();
    //网络连接模块

    connect(agvCommunication,&AgvCommunication::agvReceiveMessage,monitorPage,&MonitorPage::slotAgvReceiveMessage);

    connect (monitorPage,&MonitorPage::sigControlAgv,agvCommunication,&AgvCommunication::slotControlAgv);
    connect(monitorPage,&MonitorPage::sigDoubleControlAgv,agvCommunication,&AgvCommunication::slotDoubleControlAgv);

    connect (agvTmcInfo,&AgvTmcInfo::sigControlAgv,agvCommunication,&AgvCommunication::slotControlAgv);
    connect(monitorPage,&MonitorPage::sigScheduleAgv,agvCommunication,&AgvCommunication::slotScheduleAgv );
    connect(monitorPage,&MonitorPage::sigTmcTableReceive,agvTmcInfo,&AgvTmcInfo::slotTmcTable);

    connect (stationCommunication,&StationCommunication::sigControlAgv,agvCommunication,&AgvCommunication::slotControlAgv);
    connect(stationCommunication,&StationCommunication::sigStationReceiveNew,monitorPage,&MonitorPage::slotStationReceiveNew);

}

//状态栏显示当前的时间
void MainPage::statusbarTimer ()
{
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
    time_statusLabel->setText(current_date);
}

void MainPage::buttonColorChange()
{
    ui->datastatistics_pagebtn->setStyleSheet("QPushButton{background-color:rgb(27,137,202)}");
    ui->monitor_pagebtn->setStyleSheet("QPushButton{background-color:rgb(27,137,202)}");
    ui->statistics_pagebtn->setStyleSheet("QPushButton{background-color:rgb(27,137,202)}");
    ui->about_pagebtn->setStyleSheet("QPushButton{background-color:rgb(27,137,202)}");
    ui->debug_pagebtn->setStyleSheet("QPushButton{background-color:rgb(27,137,202)}");
    ui->pwd_pagebtn->setStyleSheet("QPushButton{background-color:rgb(27,137,202)}");
    ui->startLocationCard_pagebtn->setStyleSheet("QPushButton{background-color:rgb(27,137,202)}");
    ui->tmcInfo_pagebtn->setStyleSheet("QPushButton{background-color:rgb(27,137,202)}");
    ui->crossInfo_pagebtn->setStyleSheet("QPushButton{background-color:rgb(27,137,202)}");
    ui->stationCall_pagebtn->setStyleSheet("QPushButton{background-color:rgb(27,137,202)}");
    ui->stationCargo_pagebtn->setStyleSheet("QPushButton{background-color:rgb(27,137,202)}");
    ui->readcard_pagebtn->setStyleSheet("QPushButton{background-color:rgb(27,137,202)}");
}

void MainPage::on_monitor_pagebtn_clicked()
{
    buttonColorChange();
    ui->monitor_pagebtn->setStyleSheet("QPushButton{background-color:rgb(0, 255, 0)}");
    ui->stackedWidget->setCurrentWidget(monitorPage);
}

void MainPage::on_statistics_pagebtn_clicked()
{
    buttonColorChange();
    ui->statistics_pagebtn->setStyleSheet("QPushButton{background-color:rgb(0, 255, 0)}");

    ui->stackedWidget->setCurrentWidget(statisticShowPage);

}

void MainPage::on_about_pagebtn_clicked()
{
    buttonColorChange();
    ui->about_pagebtn->setStyleSheet("QPushButton{background-color:rgb(0, 255, 0)}");
    ui->stackedWidget->setCurrentWidget(aboutPage);
}

void MainPage::closeEvent(QCloseEvent *event)
{
    int returnValue =WidgetDraw::ShowMessageBoxQuesion (QStringLiteral("确定关闭所有窗口并退出吗？"));
    if(returnValue == 1)
    {
        QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
        QSqlQuery query(db);
        for (int i=1; i<5; i++)
        {
            query.exec (QString("update AgvMessage set agvIpAddress=%1,agvCurrentCard=%2 where agvNum=%3").arg (0).arg (0).arg(i));
        }
        query.exec (QString("delete from TmcAgvNum"));//删除交管表中的数据
        query.exec(QString("delete from DoubleTmcAgvNum"));//删除双向交管卡的数据
        uint stramp=QDateTime::currentDateTime().toTime_t();
        uint temp=stramp-(5*24*60*60);//2天记录
        query.exec(QString("delete from startmp where stramp <= %1").arg(temp));
        query.exec(QString("delete from ReadCardTable where stramp <= %1").arg(temp));//删除七天以上的数据
        Singleton<ConnectionPool>::getInstance().releaseConnection(db);
        QCoreApplication::quit();
        event->accept ();
    }else{
        event->ignore ();
    }
}
//UDP数据发送信息显示窗口
void MainPage::on_debug_pagebtn_clicked()
{
    buttonColorChange();
    ui->debug_pagebtn->setStyleSheet("QPushButton{background-color:rgb(0, 255, 0)}");
    ui->stackedWidget->setCurrentWidget(debugWindow);
    connect(agvCommunication,&AgvCommunication::sigSendContextToShow,debugWindow,&DebugWindow::slotSendContext);
    connect(agvCommunication,&AgvCommunication::sigReceiveContextToShow,debugWindow,&DebugWindow::slotReceiveContext);
    connect(stationCommunication,&StationCommunication::sigSendContextToShow,debugWindow,&DebugWindow::slotSendContext);
    connect(stationCommunication,&StationCommunication::sigReceiveContextToShow,debugWindow,&DebugWindow::slotReceiveContext);

}
//密码修改窗口
void MainPage::on_pwd_pagebtn_clicked()
{
    buttonColorChange();
    ui->pwd_pagebtn->setStyleSheet("QPushButton{background-color:rgb(0, 255, 0)}");
    ui->stackedWidget->setCurrentWidget(modifyPwdWindow);

}
//起始点卡号窗口
void MainPage::on_startLocationCard_pagebtn_clicked()
{
    buttonColorChange();
    ui->startLocationCard_pagebtn->setStyleSheet("QPushButton{background-color:rgb(0, 255, 0)}");
    ui->stackedWidget->setCurrentWidget(startLocationWindow);

}

//交管信息修改窗口
void MainPage::on_tmcInfo_pagebtn_clicked()
{
    buttonColorChange();
    ui->tmcInfo_pagebtn->setStyleSheet("QPushButton{background-color:rgb(0, 255, 0)}");
    ui->stackedWidget->setCurrentWidget(tmcModifyWindow);
}
//路口信息窗口
void MainPage::on_crossInfo_pagebtn_clicked()
{
    buttonColorChange();
    ui->crossInfo_pagebtn->setStyleSheet("QPushButton{background-color:rgb(0, 255, 0)}");
    ui->stackedWidget->setCurrentWidget(crossInfoWindow);
}
//工位呼叫窗口
void MainPage::on_stationCall_pagebtn_clicked()
{
    buttonColorChange();
    ui->stationCall_pagebtn->setStyleSheet("QPushButton{background-color:rgb(0, 255, 0)}");
    ui->stackedWidget->setCurrentWidget(stationCallWindow);
}
//物料信息窗口
void MainPage::on_stationCargo_pagebtn_clicked()
{
    buttonColorChange();
    ui->stationCargo_pagebtn->setStyleSheet("QPushButton{background-color:rgb(0, 255, 0)}");
    ui->stackedWidget->setCurrentWidget(stationCargoInfoWindow);
}

//数据统计界面
void MainPage::on_datastatistics_pagebtn_clicked()
{
    buttonColorChange();
    ui->datastatistics_pagebtn->setStyleSheet("QPushButton{background-color:rgb(0, 255, 0)}");
    ui->stackedWidget->setCurrentWidget(dataStatisticsPage);
}

void MainPage::on_readcard_pagebtn_clicked()
{
    buttonColorChange();
    ui->readcard_pagebtn->setStyleSheet("QPushButton{background-color:rgb(0, 255, 0)}");
    ui->stackedWidget->setCurrentWidget(readCardWindow);
}
