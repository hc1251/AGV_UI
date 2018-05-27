#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QUdpSocket>
#include <QMap>
#include <QSqlDatabase>
#include <QLabel>
#include <QFont>
#include <QScrollBar>
#include <QUdpSocket>
#include <QHostInfo>
#include <QMessageBox>
#include <QScrollBar>
#include <QDateTime>
#include <QNetworkInterface>
#include <QProcess>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlDriver>
#include <QSqlError>
#include <QHeaderView>
#include <QTime>
#include <QThread>
#include <QMutex>
#include <QSignalMapper>
#include <QStatusBar>
#include <QDesktopServices>
#include <QEvent>
#include <QActionGroup>
#include <QSystemTrayIcon>
#include <QComboBox>
#include <QToolButton>
#include <QButtonGroup>
#include <QMediaPlayer>
#include <QHBoxLayout>
#include "agv/AgvSymbol.h"
#include <QList>
#include "station/StationInformation.h"
#include "qcustomplot.h"

class DebugWindow;
class AgvCommunication;
class RecordWindow;
class TimeManager;
class ConnectionPool;
class ConfigUtil;
class MapShow;
class AgvShowInMap;
class AgvSymbol;
class AgvTmcInfo;
class AgvDoubleTmcInfo;
class ModifyPwdWindow;
class StationShowItem;
class StationCommunication;
class StationCargoInfoWindow;
class StationCallWindow;
class TmcModifyWindow;
class CrossInfoWindow;
class ReadCardWindow;
class StartLocationWindow;
class StationBtnWindow;
class HttpCommunication;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public:
    Ui::MainWindow *ui;
    MapShow *mapShow; //地图显示窗口
    RecordWindow *batteryRecordWindow;//
    RecordWindow *departureRecordWindow;
    DebugWindow *debugWindow;
    AgvCommunication *agvCommunication;  //小车通讯
    AgvShowInMap *agvShowInMap;
    AgvSymbol *agvSymbol;
    AgvTmcInfo *agvTmcInfo;//交管

    ModifyPwdWindow *modifyPwdWindow;
    StationShowItem *stationShowItem;
    StationCommunication *stationCommunication;//工位通信
    StationCargoInfoWindow *stationCargoInfoWindow;
    StationCallWindow *stationCallWindow;
    TmcModifyWindow *tmcModifyWindow;
    CrossInfoWindow *crossInfoWindow;
    ReadCardWindow *readCardWindow;

    StartLocationWindow *startLocationWindow;//起始点卡号对话框

    StationBtnWindow *stationBtnWindow;//选择工位对话框

    HttpCommunication *httpCommunication;//http连接

    int agvClickId;
private:
    quint16 agvInitPosition;
    //多线程
    static QMutex mutex;
    QThread threadAgv;
    QThread threadStation;
    QThread threadHttp;
    QSqlDatabase db;
    //状态栏
    QTimer *timer;
    QLabel *time_statusLabel;
    QLabel *info_statusLabel;
    QMediaPlayer *myPlayer;//播放音乐
    QHBoxLayout *layout;
    QTimer *stationTimer;
    QList<int>stationReleaseList;
    int callRowNum;

    QButtonGroup *agvBtnGroup;
    //QUdpSocket *udpSocket;

protected:
    void closeEvent(QCloseEvent *);
private://初始化函数
    void initMainWindow();//初始化主界面
    void initStationShowItem();//初始化工位显示界面
    void initStatusBar();//初始化状态栏
    void resizeEvent(QResizeEvent*);
private slots:
    void on_startButton_clicked();
    void on_stopBtn_clicked();
    void on_resetBtn_clicked();
    void on_unlockedBtn_clicked();
    void on_stationReleaseBtn_clicked();
    void on_stationStartBtn_clicked();
    void on_modifyPwdAction_triggered();
   // void on_aboutAction_triggered();
   // void on_urlAction_triggered();
    void on_debugAction_triggered();
    //void on_maxSizeAction_triggered();
    void on_miniSizeAction_triggered();
   // void on_agvTaskRecordAction_triggered();
   // void on_crossTmcFIFOAction_triggered();
   // void on_crossTmcMaxNumAction_triggered();
   // void on_crossTmcCloseAction_triggered();
   // void on_batteryRecordAction_triggered();
   // void on_barrierAction_triggered();

    void on_stationCargoAction_triggered();

    void on_tmcInfoAction_triggered();

    void on_crossInfoAction_triggered();

    void on_pushButton_clicked();

    void on_actionCall_triggered();

    void on_readCardAction_triggered();


    void on_aboutAction_triggered();

    void on_urlAction_triggered();

    void on_startLocationAction_triggered();

    void on_startButton2_clicked();

    void on_selectStationBtn_clicked();

    void receiveSelectStationInfo(QString data);//接收传过来的信息
    void slotAgvBtnClicked(int);//点击AGV的按钮
     void setupWidgetBarChart(QCustomPlot *customPlot);
    //
public slots:
    void slotAgvReceiveMessage(quint8 num);//socket发送信号后的接收槽函数
    void slotStationReceiveInfo(quint8 num);
    void slotStationReceiveNew(StationInformation *staNew);
    void checkonoffTimer();//实时检查小车是否掉线

    void statusbarTimer();//状态栏显示时间函数
    void stationReleaseTimer();//工位方向实时监测在初始位置上的小车
signals:
    void sigDoubleControlAgv(const quint8& agvNum,HITAGV::ActionSymbol action1,HITAGV::ActionSymbol action2);//发送双向启动信号
    void sigScheduleAgv(const quint8 &agvId,const QByteArray &station);//调度AGV至某一工位
    void sigControlAgv(const quint8& agvNum,HITAGV::ActionSymbol action);//发送AGV启动、停止、解锁、复位信号
    void sigUpdateMapShowAgvLocation(const quint8& agvNum,const quint16& xAxis,const quint16& yAxis,const quint16& cardDirection,const quint16& state,const quint16& agvErrorInfo);//根据AGV所在的卡号，实时更新AGV的位置
    void sigTmcTableReceive(int agvnum,int cardnum,int directionnum);//检查是否处在交管表中的发送信号

    void sigDoubleTmcTableReceive(int agvnum,int cardnum,int directionnum);//检查是否处在双向交管表中的发送信号

    void buttonClicked(int);

};
#endif // MAINWINDOW_H
