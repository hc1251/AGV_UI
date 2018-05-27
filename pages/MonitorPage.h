#ifndef MONITORPAGE_H
#define MONITORPAGE_H

#include <QWidget>
#include <QButtonGroup>
#include <QLabel>
#include <QTimer>
#include "agv/AgvSymbol.h"

class MainPage;
class MapShow;
class AgvCommunication;
class StationInformation;
class AgvShowInMap;
class AgvSymbol;
class  StationCommunication;
class StationBtnWindow;
namespace Ui {
class MonitorPage;
}

class MonitorPage : public QWidget
{
    Q_OBJECT

public:
    explicit MonitorPage(QWidget *parent = 0);
    ~MonitorPage();
public:
    MapShow *mapShow;
    AgvCommunication *agvCommunication;
    StationInformation *stationInformation;
    AgvShowInMap *agvShowInMap;
    AgvSymbol *agvSymbol;
    StationCommunication *stationCommunication;//工位通信
    StationBtnWindow *stationBtnWindow;

    QButtonGroup *agvBtnGroup;
    void resizeEvent(QResizeEvent*);

    int agvClickId;


    int callRowNum;
    QList<int>stationReleaseList;
    QTimer *stationTimer;
private:
    Ui::MonitorPage *ui;

private:
    void initUIWindow();
    void initStationShowItem();
    quint16 agvInitPosition;

public slots:
    void slotAgvReceiveMessage(quint8 num);//socket发送信号后的接收槽函数
    void slotStationReceiveInfo(quint8 num);
    void slotStationReceiveNew(StationInformation *staNew);
    void checkonoffTimer();//实时检查小车是否掉线

    void stationReleaseTimer();//工位方向实时监测在初始位置上的小车
    void slotAgvBtnClicked(int);//点击AGV的按钮

signals:
    void sigDoubleControlAgv(const quint8& agvNum,HITAGV::ActionSymbol action1,HITAGV::ActionSymbol action2);//发送双向启动信号
    void sigScheduleAgv(const quint8 &agvId,const QByteArray &station);//调度AGV至某一工位
    void sigControlAgv(const quint8& agvNum,HITAGV::ActionSymbol action);//发送AGV启动、停止、解锁、复位信号
    void sigUpdateMapShowAgvLocation(const quint8& agvNum,const quint16& xAxis,const quint16& yAxis,const quint16& cardDirection,const quint16& state,const quint16& agvErrorInfo);//根据AGV所在的卡号，实时更新AGV的位置
    void sigTmcTableReceive(int agvnum,int cardnum,int directionnum);//检查是否处在交管表中的发送信号

    //void sigDoubleTmcTableReceive(int agvnum,int cardnum,int directionnum);//检查是否处在双向交管表中的发送信号

    void buttonClicked(int);


private slots:
    void on_stationReleaseBtn_clicked();
    void on_pushButton_clicked();
    void on_stationStartBtn_clicked();
    void on_startButton_clicked();
    void on_startButton2_clicked();
    void on_stopBtn_clicked();
    void on_resetBtn_clicked();
    void on_unlockedBtn_clicked();
    void on_selectStationBtn_clicked();
     void receiveSelectStationInfo(QString data);//接收传过来的信息


};

#endif // MONITORPAGE_H
