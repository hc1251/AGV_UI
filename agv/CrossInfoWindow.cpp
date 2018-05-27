#include "CrossInfoWindow.h"
#include "ui_CrossInfoWindow.h"
#include "database/ConnectionPool.h"
#include <css/WidgetDraw.h>
#include <QSqlDatabase>
#include <QSqlQuery>


CrossInfoWindow::CrossInfoWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CrossInfoWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(QStringLiteral("路口信息表"));
    // ui->board_lineEdit->setFocusPolicy (Qt::NoFocus);

    ui->crossInfoTableWidget->setColumnCount(3);
    ui->crossInfoTableWidget->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("路口")<<QStringLiteral("Agv编号")<<QStringLiteral("时间"));
    ui->crossInfoTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->crossInfoTableWidget->verticalHeader()->setFixedWidth (100);
    ui->crossInfoTableWidget->verticalHeader()->setDefaultAlignment (Qt::AlignCenter);
    ui->crossInfoTableWidget->setFocusPolicy(Qt::NoFocus);//去掉虚框
    ui->crossInfoTableWidget->verticalHeader ()->hide ();

    int crossNum,agvNum;
    QString agvTime;
    int rowNum=0;
    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlQuery query(db);
    query.exec("select * from TmcAgvNum");
    while (query.next ())
    {
        crossNum=query.value ("crossid").toInt();
        agvNum=query.value ("agvNum").toInt ();
        agvTime=query.value ("agvTime").toString();
        ui->crossInfoTableWidget->setRowCount(rowNum+1);
        ui->crossInfoTableWidget->setItem (rowNum,0,new QTableWidgetItem(QString::number (crossNum)));
        ui->crossInfoTableWidget->item(rowNum,0)->setTextAlignment(Qt::AlignCenter);
        ui->crossInfoTableWidget->setItem (rowNum,1,new QTableWidgetItem(QString::number (agvNum)));
        ui->crossInfoTableWidget->item(rowNum,1)->setTextAlignment(Qt::AlignCenter);
        ui->crossInfoTableWidget->setItem (rowNum,2,new QTableWidgetItem(agvTime));
        ui->crossInfoTableWidget->item(rowNum,2)->setTextAlignment(Qt::AlignCenter);
        rowNum++;
    }
    for(int i=0;i<ui->crossInfoTableWidget->columnCount();i++)
    {
     //   ui->crossInfoTableWidget->setColumnWidth(i,ui->crossInfoTableWidget->width()*2);//设置stationInfoTableWidget的列宽
         ui->crossInfoTableWidget->setColumnWidth(i,ui->crossInfoTableWidget->width()*1.5);//设置stationInfoTableWidget的列宽
    }

    Singleton<ConnectionPool>::getInstance().releaseConnection(db);
}

CrossInfoWindow::~CrossInfoWindow()
{
    delete ui;
}

//清空按钮
void CrossInfoWindow::on_clearBtn_clicked()
{
 for(int i=0;i<ui->crossInfoTableWidget->rowCount();i++)
  {
    ui->crossInfoTableWidget->removeRow(i);
 }


 QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
 QSqlQuery query(db);
 query.exec (QString("delete from TmcAgvNum"));//删除交管表中的数据
 Singleton<ConnectionPool>::getInstance().releaseConnection(db);
}
//退出按钮
void CrossInfoWindow::on_quitBtn_clicked()
{
    this->close ();
}

void CrossInfoWindow::on_crossInfoTableWidget_activated(const QModelIndex &index)
{

}
