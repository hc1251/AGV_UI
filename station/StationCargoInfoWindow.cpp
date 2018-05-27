#include "StationCargoInfoWindow.h"
#include "ui_StationCargoInfoWindow.h"
#include "database/ConnectionPool.h"
#include "css/WidgetDraw.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>

/**工位物料窗口**/
StationCargoInfoWindow::StationCargoInfoWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StationCargoInfoWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);//去掉窗口中的？号按钮
    setWindowFlags (windowFlags ()|Qt::WindowMinimizeButtonHint);
    setWindowFlags (windowFlags ()|Qt::WindowMaximizeButtonHint);
    this->setWindowTitle(QStringLiteral("修改窗口"));
    // ui->board_lineEdit->setFocusPolicy (Qt::NoFocus);
    ui->station_lineEdit->setEnabled (false);//不可编辑
    ui->callBox_lineEdit->setEnabled (false);

    ui->stationCargoTableWidget->setColumnCount (4);
    ui->stationCargoTableWidget->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("工位号")<<QStringLiteral("呼叫盒")<<QStringLiteral("机床号")<<QStringLiteral("物料名称"));
    ui->stationCargoTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->stationCargoTableWidget->verticalHeader()->setFixedWidth (100);
    ui->stationCargoTableWidget->verticalHeader()->setDefaultAlignment (Qt::AlignCenter);
    ui->stationCargoTableWidget->setFocusPolicy(Qt::NoFocus);//去掉虚框
    ui->stationCargoTableWidget->verticalHeader ()->hide ();

    int stationNum,callBoxNum;
    QString boardName,cargoName;
    int rowNum=0;
    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlQuery query(db);
    query.exec("select * from CallBoxInfo");
    while (query.next ())
    {
        stationNum=query.value ("stationNum").toInt();
        callBoxNum=query.value ("callBoxNum").toInt ();
        boardName=query.value ("boardName").toString ();
        cargoName=query.value ("cargoName").toString ();
        ui->stationCargoTableWidget->setRowCount(rowNum+1);
        ui->stationCargoTableWidget->setItem (rowNum,0,new QTableWidgetItem(QString::number (stationNum)));
        ui->stationCargoTableWidget->item(rowNum,0)->setTextAlignment(Qt::AlignCenter);
        ui->stationCargoTableWidget->setItem (rowNum,1,new QTableWidgetItem(QString::number (callBoxNum)));
        ui->stationCargoTableWidget->item(rowNum,1)->setTextAlignment(Qt::AlignCenter);
        ui->stationCargoTableWidget->setItem (rowNum,2,new QTableWidgetItem(boardName));
        ui->stationCargoTableWidget->item(rowNum,2)->setTextAlignment(Qt::AlignCenter);
        ui->stationCargoTableWidget->setItem (rowNum,3,new QTableWidgetItem(cargoName));
        ui->stationCargoTableWidget->item(rowNum,3)->setTextAlignment(Qt::AlignCenter);
        rowNum++;
    }

    for(int i=0;i<ui->stationCargoTableWidget->columnCount();i++)
    {
 //      ui->stationCargoTableWidget->setColumnWidth(i,ui->stationCargoTableWidget->width()*2.3 );//设置stationInfoTableWidget的列宽
        ui->stationCargoTableWidget->setColumnWidth(i,ui->stationCargoTableWidget->width()*1.6);//设置stationInfoTableWidget的列宽
    }
    Singleton<ConnectionPool>::getInstance().releaseConnection(db);
    connect (ui->stationCargoTableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(slotTableWidgetClicked(int,int)));
}

StationCargoInfoWindow::~StationCargoInfoWindow()
{
    delete ui;
}
void StationCargoInfoWindow::slotTableWidgetClicked(int row, int columm)
{
    rowNumber=row;
    QString stationName,callBoxName;
    QString boardName,cargoName;
    stationName=ui->stationCargoTableWidget->item (row,0)->text ();
    callBoxName=ui->stationCargoTableWidget->item (row,1)->text ();
    boardName=ui->stationCargoTableWidget->item (row,2)->text ();
    cargoName=ui->stationCargoTableWidget->item (row,3)->text ();
    ui->station_lineEdit->setText (stationName);
    ui->callBox_lineEdit->setText (callBoxName);
    ui->board_lineEdit->setText (boardName);
    ui->cargo_lineEdit->setText (cargoName);
}

void StationCargoInfoWindow::on_modifyBtn_clicked()
{
    QString stationName,callBoxName;
    QString boardName,cargoName;
    stationName=ui->station_lineEdit->text ();
    callBoxName=ui->callBox_lineEdit->text ();
    boardName=ui->board_lineEdit->text ();
    cargoName=ui->cargo_lineEdit->text ();
    qDebug()<<stationName<<callBoxName<<boardName<<cargoName;
    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlQuery query(db);
    bool succ=query.exec (QString("update CallBoxInfo set boardName='%1',cargoName='%2' where stationNum='%3' and callBoxNum='%4'")
                          .arg (boardName).arg (cargoName).arg (stationName.toInt ()).arg (callBoxName.toInt ()));
    if(succ)
    {
        WidgetDraw::ShowMessageBoxInfo( QStringLiteral("修改成功"));
        ui->stationCargoTableWidget->setItem (rowNumber,2,new QTableWidgetItem(boardName));
        ui->stationCargoTableWidget->item(rowNumber,2)->setTextAlignment(Qt::AlignCenter);
        ui->stationCargoTableWidget->setItem (rowNumber,3,new QTableWidgetItem(cargoName));
        ui->stationCargoTableWidget->item(rowNumber,3)->setTextAlignment(Qt::AlignCenter);
    }
    else
    {
        WidgetDraw::ShowMessageBoxError (QStringLiteral("修改失败"));
    }
    Singleton<ConnectionPool>::getInstance().releaseConnection(db);
}

void StationCargoInfoWindow::on_cancelBtn_clicked()
{
    this->close ();
   /* //insert数据
    * QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlQuery query(db);
    int num=1;
    for(int i=1;i<56;i++)
    {
        for(int j=1;j<7;j++)
        {    query.prepare("insert into CallBoxInfo(stationNum,callBoxNum,boardName,cargoName) values(:stationNum,:callBoxNum,:boardName,:cargoName)");
            query.bindValue (":stationNum",i);
            query.bindValue(":callBoxNum",j);
            query.bindValue(":boardName","机床"+QString::number (num));
            query.bindValue(":cargoName","物料"+QString::number (num));
            query.exec();
            num++;

        }
    }
       Singleton<ConnectionPool>::getInstance().releaseConnection(db);
       */



}
