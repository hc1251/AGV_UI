#ifndef AGVSHOWINMAP_H
#define AGVSHOWINMAP_H

#include <QObject>
#include <QPoint>


class PosInfo{
public:
    QPoint *agvPos;
    int direction;
};

class AgvShowInMap : public QObject
{
    Q_OBJECT
public:
    explicit AgvShowInMap(QObject *parent = 0);
    /*
        输入：startCardNum， endCardNum， 移动距离
        返回：当前在坐标系中坐标，所在位置车的方向
    */
    // PosInfo *agvPosition(int start, int end, float distance);
    /*
        输入：startCardNum， endCardNum， 移动距离
        返回：当前在坐标系中坐标,所在位置车的方向
    */
    PosInfo *agvPosition(int start);
    //输入：工位的卡号   返回：工位卡号所对应的Id
    int agvStationId(int stationNum);
    //输入：工位卡号对应的Id 返回：工位的卡号
    int agvStationNum(int stationId);
public:
    PosInfo *posInfo;
private:
    //返回：0 水平， 1 竖直， 2 转弯
    int isInline(QPoint *pt1, QPoint *pt2) ;
    int lineNum(int agvTargetStationNum);
};

#endif // AGVSHOWINMAP_H
