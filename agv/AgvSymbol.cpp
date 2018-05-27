#include "AgvSymbol.h"
volatile bool AgvSymbol::haveInitBatteryWindow = false;
volatile bool AgvSymbol::haveInitDepartureWindow = false;

AgvSymbol::AgvSymbol()
{

}
AgvSymbol::~AgvSymbol()
{

}
QString AgvSymbol::getAgvStatus(const quint8 &value){
    QString state("");
    switch (value) {
    case 1:
        state=QStringLiteral("运行中");
        break;
    case 2:
        state=QStringLiteral("停止中");
        break;
    case 3:
        state=QStringLiteral("锁定中");
        break;
    default:
        break;
    }
    return state;
}


QString AgvSymbol::errorInfo(const quint8 &value)
{
    QString error("");
    switch (value) {
    case 0:
        error = QStringLiteral("无错误");
        break;
    case 1:
        error = QStringLiteral("无路径信号信息");
        break;
    case 2:
        error = QStringLiteral("电源电量过低");
        break;
    case 3:
        error = QStringLiteral("机械防撞生效");
        break;
    case 4:
        error = QStringLiteral("驱动器报警");
        break;
    case 5:
        error = QStringLiteral("路径检测报警");
        break;
    case 6:
        error = QStringLiteral("路径设置出错");
        break;
    case 7:
        error = QStringLiteral("急停按下");
        break;
    case 8:
        error = QStringLiteral("驱动升降感应异常");
        break;
    case 9:
        error = QStringLiteral("牵引升降异常");
        break;
    case 10:
        error = QStringLiteral("驱动升起状态，未能启动错误");
        break;
    default:
        break;
    }
    return error;
}

