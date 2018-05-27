#include "TimeManager.h"

TimeManager::TimeManager(QObject *parent):QObject(parent)
{
    agvArrivalStationTimer=new QTimer(this);
    connect(agvArrivalStationTimer,&QTimer::timeout,this,&TimeManager::timeout);
    totalTime = 0;
}

void TimeManager::timeout()
{
    totalTime++;
}
