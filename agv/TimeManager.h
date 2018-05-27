#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H



#include<QTimer>
#include<QObject>
class TimeManager:public QObject
{
    Q_OBJECT
public:
    explicit  TimeManager(QObject *parent=0);
    void agvOnLine();
    void agvArrivalStation(){
        agvArrivalStationTimer->start(1000);
    }
    void agvLeaveStation(){
        agvArrivalStationTimer->stop();
    }
private slots:
    void timeout();
private:
    QTimer *agvArrivalStationTimer;
    QTimer *runTimer;
    quint64 totalTime;
    quint64 runTime;
    quint64 stopTime;
};

#endif // TIMEMANAGER_H
