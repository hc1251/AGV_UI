#ifndef AGVSYMBOL_H
#define AGVSYMBOL_H

#include <QMetaType>
#include <QString>
#include <QDebug>
#include "util/Singleton.h"
#include "database/ConnectionPool.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMap>

//命令协议控制标识
namespace HITAGV {
enum ActionSymbol{
    //主控到小车
    Central_Start = 0x01,  //启动
    Central_Stop = 0x02,    //停止
    Central_Reset = 0x03,   //复位
    Central_Station=0x04,//调度AGV
    Central_MakeCharge=0x05,//充电命令
    Central_Lock=0x06,//锁定AGV
    Central_Unlock=0x07,//解锁
    Central_Front=0x01,//启动方向：前向
    Central_Back=0x02,//启动方向：后向

};
}
//Q_DECLARE_METATYPE(OKAGV::ActionSymbol)
class AgvSymbol
{
    SINGLETON(AgvSymbol)
    public:
    static volatile bool haveInitBatteryWindow;
    static volatile bool haveInitDepartureWindow;
    static QString errorInfo(const quint8& value);
    static QString getAgvStatus(const quint8& value);
};
#endif // AGVSYMBOL_H
