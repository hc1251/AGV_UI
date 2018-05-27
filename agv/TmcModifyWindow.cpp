#include "TmcModifyWindow.h"
#include "ui_TmcModifyWindow.h"
#include "database/ConnectionPool.h"
#include <css/WidgetDraw.h>
#include <QSqlDatabase>
#include <QSqlQuery>

TmcModifyWindow::TmcModifyWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TmcModifyWindow)
{
    ui->setupUi(this);

    this->setWindowTitle(QStringLiteral("交管路口表"));
    // ui->board_lineEdit->setFocusPolicy (Qt::NoFocus);

    ui->tmcInfoTableWidget->setColumnCount(3);
    ui->tmcInfoTableWidget->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("交管路口")<<QStringLiteral("加锁卡")<<QStringLiteral("解锁卡"));
    ui->tmcInfoTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->tmcInfoTableWidget->verticalHeader()->setFixedWidth (300);
    ui->tmcInfoTableWidget->verticalHeader()->setDefaultAlignment (Qt::AlignCenter);
    ui->tmcInfoTableWidget->setFocusPolicy(Qt::NoFocus);//去掉虚框
    ui->tmcInfoTableWidget->verticalHeader ()->hide ();

    int crossNum,lockCard,unlockCard;
    int rowNum=0;
    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlQuery query(db);
    query.exec("select * from TmcTable");
    while (query.next ())
    {
        crossNum=query.value ("crossid").toInt();
        lockCard=query.value ("lockCard").toInt ();
        unlockCard=query.value ("unlockCard").toInt();
        ui->tmcInfoTableWidget->setRowCount(rowNum+1);
        ui->tmcInfoTableWidget->setItem (rowNum,0,new QTableWidgetItem(QString::number (crossNum)));
        ui->tmcInfoTableWidget->item(rowNum,0)->setTextAlignment(Qt::AlignCenter);
        ui->tmcInfoTableWidget->setItem (rowNum,1,new QTableWidgetItem(QString::number (lockCard)));
        ui->tmcInfoTableWidget->item(rowNum,1)->setTextAlignment(Qt::AlignCenter);
        ui->tmcInfoTableWidget->setItem (rowNum,2,new QTableWidgetItem(QString::number(unlockCard)));
        ui->tmcInfoTableWidget->item(rowNum,2)->setTextAlignment(Qt::AlignCenter);
        rowNum++;
    }
    Singleton<ConnectionPool>::getInstance().releaseConnection(db);
    //双击表格
 //   connect (ui->tmcInfoTableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(slotTableWidgetDoubleClicked(int,int)));
//单击表格
    for(int i=0;i<3;i++)
    {
        //ui->tmcInfoTableWidget->setColumnWidth(i, ui->tmcInfoTableWidget->width()*2.8);//设置tablewidget的列宽
          ui->tmcInfoTableWidget->setColumnWidth(i, ui->tmcInfoTableWidget->width()*2);//设置tablewidget的列宽
    }
    connect(ui->tmcInfoTableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(slotTableWidgetClicked(int,int)));
}

TmcModifyWindow::~TmcModifyWindow()
{
    delete ui;
}
void TmcModifyWindow::slotTableWidgetClicked(int row,int column)
{
    rowNumber=row;
    QString crossNum,lockCard,unlockCard;
    crossNum=ui->tmcInfoTableWidget->item(row,0)->text();
    lockCard=ui->tmcInfoTableWidget->item(row,1)->text();
    unlockCard=ui->tmcInfoTableWidget->item(row,2)->text();
    ui->cross_lineEdit->setText(crossNum);
    ui->lock_lineEdit->setText(lockCard);
    ui->unlock_lineEdit->setText(unlockCard);

}

//删除按钮
void TmcModifyWindow::on_deteleBtn_clicked()
{
    QString crossNum,lockCard,unlockCard;
    crossNum=ui->cross_lineEdit->text();
    lockCard=ui->lock_lineEdit->text();
    unlockCard=ui->unlock_lineEdit->text();

    if((crossNum=="")||(lockCard=="")||(unlockCard==""))
    {
        WidgetDraw::ShowMessageBoxError(QStringLiteral("编辑框中不允许为空"));
        return;
    }
    if(lockCard==unlockCard)
    {
        WidgetDraw::ShowMessageBoxError(QStringLiteral("加锁卡和解锁卡不允许相同"));
        return;
    }
    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlQuery query(db);

       bool succ= query.exec(QString("delete from TmcTable where crossid=%1 and lockCard=%2 and unlockCard=%3").arg(crossNum.toInt()).arg(lockCard.toInt()).arg(unlockCard.toInt()));

       // bool succ= query.exec(QString("delete from TmcTable where crossid=%1 and lockCard=%2").arg(crossNum.toInt()).arg(lockCard.toInt()));

        if(succ)
        {
            WidgetDraw::ShowMessageBoxInfo(QStringLiteral("删除成功"));
            ui->tmcInfoTableWidget->removeRow(rowNumber);
        }
        else{
            WidgetDraw::ShowMessageBoxError(QStringLiteral("删除失败"));
        }

    Singleton<ConnectionPool>::getInstance().releaseConnection(db);
}
//插入按钮
void TmcModifyWindow::on_insertBtn_clicked()
{
    QString crossNum,lockCard,unlockCard;
    crossNum=ui->cross_lineEdit->text();
    lockCard=ui->lock_lineEdit->text();
    unlockCard=ui->unlock_lineEdit->text();

    if((crossNum=="")||(lockCard=="")||(unlockCard==""))
    {
        WidgetDraw::ShowMessageBoxError(QStringLiteral("编辑框中不允许为空"));
        return;
    }
    if(lockCard==unlockCard)
    {
        WidgetDraw::ShowMessageBoxError(QStringLiteral("加锁卡和解锁卡不允许相同"));
        return;
    }
    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlQuery query(db);
    qDebug()<<crossNum.toInt()<<lockCard.toInt()<<unlockCard.toInt();
    bool succ1=query.exec(QString("select * from TmcTable where crossid='%1' and lockCard='%2' and unlockCard='%3'").arg(crossNum.toInt()).arg(lockCard.toInt()).arg(unlockCard.toInt()));
    if(succ1)
    {qDebug()<<"chenggong";}
    else{
        qDebug()<<"shibai";
    }

    if(succ1&&query.first())
    {
        QMessageBox::information(this,QStringLiteral("提示"),QStringLiteral("该信息已存在"));
        Singleton<ConnectionPool>::getInstance().releaseConnection(db);
        return;
    }
    else
    {



      query.prepare("insert into TmcTable(crossid,lockCard,unlockCard) values(:crossid,:lockCard,:unlockCard)");
      query.bindValue(0,ui->cross_lineEdit->text().toInt());//crossNum.toInt());
      query.bindValue(1,ui->lock_lineEdit->text().toInt());//lockCard.toInt());
      query.bindValue(2,ui->unlock_lineEdit->text().toInt());//unlockCard.toInt());

      bool succ = query.exec();
      if(succ)
      {

          int rowNum=0;
          int crossNum,lockCard,unlockCard;
          query.exec("select * from TmcTable");
          while (query.next ())
          {
              crossNum=query.value ("crossid").toInt();
              lockCard=query.value ("lockCard").toInt ();
              unlockCard=query.value ("unlockCard").toInt();
              ui->tmcInfoTableWidget->setRowCount(rowNum+1);
              ui->tmcInfoTableWidget->setItem (rowNum,0,new QTableWidgetItem(QString::number (crossNum)));
              ui->tmcInfoTableWidget->item(rowNum,0)->setTextAlignment(Qt::AlignCenter);
              ui->tmcInfoTableWidget->setItem (rowNum,1,new QTableWidgetItem(QString::number (lockCard)));
              ui->tmcInfoTableWidget->item(rowNum,1)->setTextAlignment(Qt::AlignCenter);
              ui->tmcInfoTableWidget->setItem (rowNum,2,new QTableWidgetItem(QString::number(unlockCard)));
              ui->tmcInfoTableWidget->item(rowNum,2)->setTextAlignment(Qt::AlignCenter);
              rowNum++;
          }

       /*
        //int len=ui->tmcInfoTableWidget->rowCount();


        ui->tmcInfoTableWidget->insertRow(len+1);
        ui->tmcInfoTableWidget->setItem(len+1,0,new QTableWidgetItem(crossNum));
        ui->tmcInfoTableWidget->setItem (len+1,1,new QTableWidgetItem(lockCard));
        ui->tmcInfoTableWidget->setItem (len+1,2,new QTableWidgetItem(unlockCard));
        ui->tmcInfoTableWidget->show();
        */
        WidgetDraw::ShowMessageBoxInfo (QStringLiteral("插入成功"));
      }
      else
      {
          qDebug()<<query.lastError().text();
        WidgetDraw::ShowMessageBoxError (QStringLiteral("插入失败"));
      }
    }
    Singleton<ConnectionPool>::getInstance().releaseConnection(db);
}
//退出按钮
void TmcModifyWindow::on_quitBtn_clicked()
{
    this->close ();
}
