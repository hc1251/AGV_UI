#include "StartLocationWindow.h"
#include "ui_StartLocationWindow.h"


#include "database/ConnectionPool.h"
#include "css/WidgetDraw.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
StartLocationWindow::StartLocationWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartLocationWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);//去掉窗口中的？号按钮
    setWindowFlags (windowFlags ()|Qt::WindowMinimizeButtonHint);
    setWindowFlags (windowFlags ()|Qt::WindowMaximizeButtonHint);
    this->setWindowTitle(QStringLiteral("起始点信息表"));
      ui->agvNumEdit->setEnabled(false);
      ui->startCardEdit->setEnabled(false);

      ui->tableWidget->setColumnCount (2);
       ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("AGV编号")<<QStringLiteral("起始点卡号"));
       ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
       ui->tableWidget->verticalHeader()->setFixedWidth (200);
       ui->tableWidget->verticalHeader()->setDefaultAlignment (Qt::AlignCenter);
       ui->tableWidget->setFocusPolicy(Qt::NoFocus);//去掉虚框
       ui->tableWidget->verticalHeader ()->hide ();

       int agvNum,startCardNum;
       int rowNum=0;
       QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
          QSqlQuery query(db);
           query.exec("select * from StartLocationCard");
           while (query.next ())
           {
               agvNum=query.value ("agvNum").toInt();
               startCardNum=query.value ("startCardNum").toInt ();
               ui->tableWidget->setRowCount(rowNum+1);
               ui->tableWidget->setItem (rowNum,0,new QTableWidgetItem(QString::number (agvNum)));
               ui->tableWidget->item(rowNum,0)->setTextAlignment(Qt::AlignCenter);
               ui->tableWidget->setItem (rowNum,1,new QTableWidgetItem(QString::number (startCardNum)));
               ui->tableWidget->item(rowNum,1)->setTextAlignment(Qt::AlignCenter);
               rowNum++;
           }
           for(int i=0;i<ui-> tableWidget->columnCount();i++)
           {
            //   ui->tableWidget->setColumnWidth(i, ui->tableWidget->width()*3.2);//设置stationInfoTableWidget的列宽、
               ui->tableWidget->setColumnWidth(i, ui->tableWidget->width()*2.3);//设置stationInfoTableWidget的列宽

           }

           Singleton<ConnectionPool>::getInstance().releaseConnection(db);
           connect (ui->tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(slotTableWidgetClicked(int,int)));
}

StartLocationWindow::~StartLocationWindow()
{
    delete ui;
}
void StartLocationWindow::slotTableWidgetClicked(int row, int columm)
{
    rowNumber=row;

    QString agvNum,startCardNum;
    agvNum=ui->tableWidget->item (row,0)->text ();
    startCardNum=ui->tableWidget->item (row,1)->text ();
    ui->agvNumEdit->setText(agvNum);
    ui->startCardEdit->setText(startCardNum);
    ui->startCardEdit->setEnabled(true);

}

void StartLocationWindow::on_modifyBtn_clicked()
{
    QString agvNum,startCardNum;
    agvNum=ui->agvNumEdit->text();
    startCardNum=ui->startCardEdit->text();
    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
        QSqlQuery query(db);

        bool succ=query.exec(QString("select * from mapPoint where cardPoint=%1").arg (startCardNum));
        if(!(succ&&query.first()))
        {
            WidgetDraw::ShowMessageBoxError (QStringLiteral("请选择界面中存在的卡号作为AGV小车的起始卡号"));
             Singleton<ConnectionPool>::getInstance().releaseConnection(db);
            return;
        }

        bool succ1=query.exec (QString("update StartLocationCard set startCardNum='%1' where agvNum='%2'")
                              .arg (startCardNum).arg(agvNum));
        if(succ1)
        {
            WidgetDraw::ShowMessageBoxInfo (QStringLiteral("修改成功"));
            ui->tableWidget->setItem(rowNumber,1,new QTableWidgetItem(startCardNum));
            ui->tableWidget->item(rowNumber,1)->setTextAlignment(Qt::AlignCenter);
        }
        else
        {
            WidgetDraw::ShowMessageBoxError (QStringLiteral("修改失败"));
        }
        Singleton<ConnectionPool>::getInstance().releaseConnection(db);

}

void StartLocationWindow::on_cancelBtn_clicked()
{
    this->close();
}
