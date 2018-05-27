#include "DataStatisticsPage.h"
#include "ui_DataStatisticsPage.h"
#include <QDebug>
#include "database/ConnectionPool.h"
#include "css/WidgetDraw.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>

DataStatisticsPage::DataStatisticsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataStatisticsPage)
{
    ui->setupUi(this);
    ui->radioButton_all->setChecked(true);
    btnGroup=new QButtonGroup(this);
    btnGroup->addButton(ui->radioButton_all);
    btnGroup->addButton(ui->radioButton_day);
    btnGroup->addButton(ui->radioButton_week);
    btnGroup->addButton(ui->radioButton_month);
    connect(ui->radioButton_all,SIGNAL(clicked()),this,SLOT(onRadioClick()));
    connect(ui->radioButton_day,SIGNAL(clicked()),this,SLOT(onRadioClick()));
    connect(ui->radioButton_month,SIGNAL(clicked()),this,SLOT(onRadioClick()));
    connect(ui->radioButton_week,SIGNAL(clicked()),this,SLOT(onRadioClick()));
    initUI();
}
void DataStatisticsPage::initUI()
{
    ui->tableWidget->setColumnCount (4);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("日期")<<QStringLiteral("运行时间(h)")<<QStringLiteral("空闲时间(h)")<<QStringLiteral("总时间(h)"));
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->tableWidget->verticalHeader()->setDefaultAlignment (Qt::AlignCenter);
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);//去掉虚框
    ui->tableWidget->verticalHeader ()->hide ();
    for(int i=0;i<ui->tableWidget->columnCount();i++)
    {
      //  ui->tableWidget->setColumnWidth(i,ui->tableWidget->width()*1.1);//设置stationInfoTableWidget的列宽
         ui->tableWidget->setColumnWidth(i,ui->tableWidget->width()*0.825);//设置stationInfoTableWidget的列宽
    }
    ui->tableWidget_2->setColumnCount (4);
    ui->tableWidget_2->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("日期")<<QStringLiteral("运行时间(h)")<<QStringLiteral("空闲时间(h)")<<QStringLiteral("总时间(h)"));
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->tableWidget_2->verticalHeader()->setDefaultAlignment (Qt::AlignCenter);
    ui->tableWidget_2->setFocusPolicy(Qt::NoFocus);//去掉虚框
    ui->tableWidget_2->verticalHeader ()->hide ();
    for(int i=0;i<ui->tableWidget_2->columnCount();i++)
    {
        ui->tableWidget_2->setColumnWidth(i,ui->tableWidget_2->width()*0.825);//设置stationInfoTableWidget的列宽
    }
    ui->tableWidget_3->setColumnCount (4);
    ui->tableWidget_3->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("日期")<<QStringLiteral("运行时间(h)")<<QStringLiteral("空闲时间(h)")<<QStringLiteral("总时间(h)"));
    ui->tableWidget_3->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->tableWidget_3->verticalHeader()->setDefaultAlignment (Qt::AlignCenter);
    ui->tableWidget_3->setFocusPolicy(Qt::NoFocus);//去掉虚框
    ui->tableWidget_3->verticalHeader ()->hide ();
    for(int i=0;i<ui->tableWidget_3->columnCount();i++)
    {
        ui->tableWidget_3->setColumnWidth(i,ui->tableWidget_3->width()*0.825);//设置stationInfoTableWidget的列宽
    }

    ui->tableWidget_4->setColumnCount (4);
    ui->tableWidget_4->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("日期")<<QStringLiteral("运行时间(h)")<<QStringLiteral("空闲时间(h)")<<QStringLiteral("总时间(h)"));
    ui->tableWidget_4->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->tableWidget_4->verticalHeader()->setDefaultAlignment (Qt::AlignCenter);
    ui->tableWidget_4->setFocusPolicy(Qt::NoFocus);//去掉虚框
    ui->tableWidget_4->verticalHeader ()->hide ();
    for(int i=0;i<ui->tableWidget_4->columnCount();i++)
    {
        ui->tableWidget_4->setColumnWidth(i,ui->tableWidget_4->width()*0.825);//设置stationInfoTableWidget的列宽
    }
    allShow();
}

DataStatisticsPage::~DataStatisticsPage()
{
    delete ui;
}
void DataStatisticsPage::onRadioClick()
{
    switch (btnGroup->checkedId()) {
    case 0:
        allShow();
        break;
    case 1:
        dayShow();
        break;
    case 2:
        weekShow();
        break;
    case 3:
        monthShow();
        break;
    default:
        break;
    }
}
void DataStatisticsPage::allShow()
{
    QString runtime,freetime,alltime;
    QString time;
    int rowNum=0;
    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlQuery query(db);
    query.exec(QString("select * from agvRunTime where agvNum='%1'").arg(1));
    while (query.next ())
    {
        runtime=query.value ("runtime").toString();
        freetime=query.value ("freetime").toString();
        alltime=query.value("alltime").toString();
        time=query.value ("time").toString ();
        ui->tableWidget->setRowCount(rowNum+1);
       ui->tableWidget->setItem (rowNum,0,new QTableWidgetItem(time));
        ui->tableWidget->item(rowNum,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem (rowNum,1,new QTableWidgetItem(runtime));
       ui->tableWidget->item(rowNum,1)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem (rowNum,2,new QTableWidgetItem(freetime));
        ui->tableWidget->item(rowNum,2)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem (rowNum,3,new QTableWidgetItem(alltime));
        ui->tableWidget->item(rowNum,3)->setTextAlignment(Qt::AlignCenter);
        rowNum++;
    }

    int rowNum2=0;
    query.exec(QString("select * from agvRunTime where agvNum='%1'").arg(2));
    while (query.next ())
    {
        runtime=query.value ("runtime").toString();
        freetime=query.value ("freetime").toString();
        alltime=query.value("alltime").toString();
        time=query.value ("time").toString ();
        ui->tableWidget_2->setRowCount(rowNum2+1);
       ui->tableWidget_2->setItem (rowNum2,0,new QTableWidgetItem(time));
        ui->tableWidget_2->item(rowNum2,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_2->setItem (rowNum2,1,new QTableWidgetItem(runtime));
       ui->tableWidget_2->item(rowNum2,1)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_2->setItem (rowNum2,2,new QTableWidgetItem(freetime));
        ui->tableWidget_2->item(rowNum2,2)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_2->setItem (rowNum2,3,new QTableWidgetItem(alltime));
        ui->tableWidget_2->item(rowNum2,3)->setTextAlignment(Qt::AlignCenter);
        rowNum2++;
    }
    int rowNum3=0;
    query.exec(QString("select * from agvRunTime where agvNum='%1'").arg(3));
    while (query.next ())
    {
        runtime=query.value ("runtime").toString();
        freetime=query.value ("freetime").toString();
        alltime=query.value("alltime").toString();
        time=query.value ("time").toString ();
        ui->tableWidget_3->setRowCount(rowNum3+1);
       ui->tableWidget_3->setItem (rowNum3,0,new QTableWidgetItem(time));
        ui->tableWidget_3->item(rowNum3,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_3->setItem (rowNum3,1,new QTableWidgetItem(runtime));
       ui->tableWidget_3->item(rowNum3,1)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_3->setItem (rowNum3,2,new QTableWidgetItem(freetime));
        ui->tableWidget_3->item(rowNum3,2)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_3->setItem (rowNum3,3,new QTableWidgetItem(alltime));
        ui->tableWidget_3->item(rowNum3,3)->setTextAlignment(Qt::AlignCenter);
        rowNum3++;
    }
    int rowNum4=0;
    query.exec(QString("select * from agvRunTime where agvNum='%1'").arg(4));
    while (query.next ())
    {
        runtime=query.value ("runtime").toString();
        freetime=query.value ("freetime").toString();
        alltime=query.value("alltime").toString();
        time=query.value ("time").toString ();
        ui->tableWidget_4->setRowCount(rowNum4+1);
       ui->tableWidget_4->setItem (rowNum4,0,new QTableWidgetItem(time));
        ui->tableWidget_4->item(rowNum4,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_4->setItem (rowNum4,1,new QTableWidgetItem(runtime));
       ui->tableWidget_4->item(rowNum4,1)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_4->setItem (rowNum4,2,new QTableWidgetItem(freetime));
        ui->tableWidget_4->item(rowNum4,2)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_4->setItem (rowNum4,3,new QTableWidgetItem(alltime));
        ui->tableWidget_4->item(rowNum4,3)->setTextAlignment(Qt::AlignCenter);
        rowNum4++;
    }
    Singleton<ConnectionPool>::getInstance().releaseConnection(db);

}
void DataStatisticsPage::dayShow()
{

}
void DataStatisticsPage::weekShow()
{

}
void DataStatisticsPage::monthShow()
{

}
