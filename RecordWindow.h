#ifndef RECORDWINDOW_H
#define RECORDWINDOW_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QStandardItemModel>
#include <database/ConnectionPool.h>
#include "agv/AgvSymbol.h"
#include <QDateTime>

namespace Ui {
class RecordWindow;
}
enum WhichWindow{
    BatteryWindow,
    DepartureWindow,
};

class RecordWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RecordWindow(QWidget *parent = 0);
    ~RecordWindow();
    void initBatteryWindowShow();
    void initDepartureAgvWindowShow();
    WhichWindow whichWindow;
signals:
    void sigCloseRecordWindow();

private:
    void batteryRecordSummary();
    void departureFreqRecordSummary();
    void resizeEvent(QResizeEvent*);
    Ui::RecordWindow *ui;
};

#endif // RECORDWINDOW_H
