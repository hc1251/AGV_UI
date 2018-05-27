#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QMainWindow>
#include "MonitorPage.h"
#include "StatisticsPage.h"
#include "AboutPage.h"
#include "MonitorPage.h"
#include "StatisticsPage.h"
#include "DataStatisticsPage.h"
#include "StatisticShowPage.h"


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
class DoubleTmcModifyWindow;
class DoubleInfoWindow;
class StartLocationWindow;
class StationBtnWindow;



namespace Ui {
class MainPage;
}

class MainPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainPage(QWidget *parent = 0);
    ~MainPage();

    MonitorPage *monitorPage;
  //  StatisticsPage *statisticsPage;
    StatisticShowPage *statisticShowPage;
    AboutPage *aboutPage;
    DataStatisticsPage *dataStatisticsPage;

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
    StartLocationWindow *startLocationWindow;

private:

    //多线程
    static QMutex mutex;
    QThread threadAgv;
    QThread threadStation;
    QSqlDatabase db;
    //状态栏
    QTimer *timer;
    QLabel *time_statusLabel;
    QLabel *info_statusLabel;

    QList<int>stationReleaseList;


private:
    void initStatusBar();
    void initMainWindow();
    void closeEvent(QCloseEvent *event);
    void buttonColorChange();





private slots:
    void on_monitor_pagebtn_clicked();
    void on_statistics_pagebtn_clicked();
    void on_about_pagebtn_clicked();
     void statusbarTimer();//状态栏显示时间函数

     void on_debug_pagebtn_clicked();

     void on_pwd_pagebtn_clicked();

     void on_startLocationCard_pagebtn_clicked();

     void on_tmcInfo_pagebtn_clicked();

     void on_crossInfo_pagebtn_clicked();

     void on_stationCall_pagebtn_clicked();

     void on_stationCargo_pagebtn_clicked();



     void on_datastatistics_pagebtn_clicked();

     void on_readcard_pagebtn_clicked();

private:
    Ui::MainPage *ui;
};

#endif // MAINPAGE_H
