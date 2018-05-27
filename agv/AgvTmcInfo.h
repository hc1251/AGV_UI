#ifndef AGVTMCINFO_H
#define AGVTMCINFO_H
#include <QObject>
#include "AgvSymbol.h"
#include <QMutex>
#include "util/singleton.h"

class AgvSymbol;

class AgvTmcInfo : public QObject
{
    Q_OBJECT
    //SINGLETON(AgvTmcInfo)
public:
    explicit AgvTmcInfo(QObject *parent = 0);
    static AgvTmcInfo *getInstance();
    int crossNum;

signals:
    void sigControlAgv(const quint8& agvNum,HITAGV::ActionSymbol action);//发送AGV启动、停止、解锁、复位信号

public slots:
    //处理交管信息 agvnum：AGV的编号，cardnum:AGV所在的卡号，directionnum:AGV行使的方向, isFiFo是否是fifo
    void slotTmcTable (int agvnum,int cardnum,int directionnum);

private:
    static QMutex mutex;
    static AgvTmcInfo* instance;
};

#endif // AGVTMCINFO_H
