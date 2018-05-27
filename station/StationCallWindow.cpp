#include "StationCallWindow.h"
#include "ui_StationCallWindow.h"
#include "database/ConnectionPool.h"
#include "css/WidgetDraw.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>

StationCallWindow::StationCallWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StationCallWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);//去掉窗口中的？号按钮
    setWindowFlags (windowFlags ()|Qt::WindowMinimizeButtonHint);
    setWindowFlags (windowFlags ()|Qt::WindowMaximizeButtonHint);
    this->setWindowTitle(QStringLiteral("呼叫信息窗口"));
    ui->startmpWidget->setColumnCount (3);
    ui->startmpWidget->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("工位号")<<QStringLiteral("呼叫盒")<<QStringLiteral("时间"));
    ui->startmpWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    //ui->startmpWidget->verticalHeader()->setFixedWidth (100);
    ui->startmpWidget->verticalHeader()->setDefaultAlignment (Qt::AlignCenter);
    ui->startmpWidget->setFocusPolicy(Qt::NoFocus);//去掉虚框
    ui->startmpWidget->verticalHeader ()->hide ();
    for(int i=0;i<ui->startmpWidget->columnCount();i++)
    {
     //   ui->startmpWidget->setColumnWidth(i,ui->startmpWidget->width()*4);//设置stationInfoTableWidget的列宽
        ui->startmpWidget->setColumnWidth(i,ui->startmpWidget->width()*2.5);//设置stationInfoTableWidget的列宽
    }


    int stationNum,callBoxNum;
    QString time;
    int rowNum=0;
    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlQuery query(db);
    query.exec("select * from StationCallTable order by stramp desc");
    while (query.next ())
    //query.last();
    //while (query.previous())
    {
        stationNum=query.value ("station").toInt();
        callBoxNum=query.value ("callBox").toInt ();
        time=query.value ("time").toString ();
        ui->startmpWidget->setRowCount(rowNum+1);
        ui->startmpWidget->setItem (rowNum,0,new QTableWidgetItem(QString::number (stationNum)));
        ui->startmpWidget->item(rowNum,0)->setTextAlignment(Qt::AlignCenter);
        ui->startmpWidget->setItem (rowNum,1,new QTableWidgetItem(QString::number (callBoxNum)));
        ui->startmpWidget->item(rowNum,1)->setTextAlignment(Qt::AlignCenter);
        ui->startmpWidget->setItem (rowNum,2,new QTableWidgetItem(time));
        ui->startmpWidget->item(rowNum,2)->setTextAlignment(Qt::AlignCenter);
        rowNum++;
    }
    Singleton<ConnectionPool>::getInstance().releaseConnection(db);
}

StationCallWindow::~StationCallWindow()
{
    delete ui;
}
