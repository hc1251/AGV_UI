#include "ReadCardWindow.h"
#include "ui_ReadCardWindow.h"
#include "database/ConnectionPool.h"
#include "css/WidgetDraw.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSplitter>
ReadCardWindow::ReadCardWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReadCardWindow)
{

    ui->setupUi(this);

    //QSplitter *splitterCard = new QSplitter(Qt::Horizontal,0);

    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);//去掉窗口中的？号按钮
    setWindowFlags (windowFlags ()|Qt::WindowMinimizeButtonHint);
    setWindowFlags (windowFlags ()|Qt::WindowMaximizeButtonHint);
    this->setWindowTitle(QStringLiteral("地标卡窗口"));
    ui->cardTableWidget1->setColumnCount (2);
    ui->cardTableWidget1->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("时间")<<QStringLiteral("卡号"));
    ui->cardTableWidget1->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->cardTableWidget1->verticalHeader()->setFixedWidth (100);
    ui->cardTableWidget1->verticalHeader()->setDefaultAlignment (Qt::AlignCenter);
    ui->cardTableWidget1->setFocusPolicy(Qt::NoFocus);//去掉虚框
    ui->cardTableWidget1->verticalHeader ()->hide ();
    ui->cardTableWidget1->setColumnWidth(0,ui->cardTableWidget1->width()*1.4);
        ui->cardTableWidget1->setColumnWidth(1,ui->cardTableWidget1->width()*1.4);

    ui->cardTableWidget2->setColumnCount (2);
    ui->cardTableWidget2->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("时间")<<QStringLiteral("卡号"));
    ui->cardTableWidget2->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->cardTableWidget2->verticalHeader()->setFixedWidth (100);
    ui->cardTableWidget2->verticalHeader()->setDefaultAlignment (Qt::AlignCenter);
    ui->cardTableWidget2->setFocusPolicy(Qt::NoFocus);//去掉虚框
    ui->cardTableWidget2->verticalHeader ()->hide ();
   //ui->cardTableWidget2->setColumnWidth(0,ui->cardTableWidget2->width()*1.85);
    ui->cardTableWidget2->setColumnWidth(0,ui->cardTableWidget2->width()*1.4);
    ui->cardTableWidget2->setColumnWidth(1,ui->cardTableWidget2->width()*1.4);

    ui->cardTableWidget3->setColumnCount (2);
    ui->cardTableWidget3->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("时间")<<QStringLiteral("卡号"));
    ui->cardTableWidget3->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->cardTableWidget3->verticalHeader()->setFixedWidth (100);
    ui->cardTableWidget3->verticalHeader()->setDefaultAlignment (Qt::AlignCenter);
    ui->cardTableWidget3->setFocusPolicy(Qt::NoFocus);//去掉虚框
    ui->cardTableWidget3->verticalHeader ()->hide ();
    ui->cardTableWidget3->setColumnWidth(0,ui->cardTableWidget3->width()*1.4);
    ui->cardTableWidget3->setColumnWidth(1,ui->cardTableWidget3->width()*1.4);

    ui->cardTableWidget4->setColumnCount (2);
    ui->cardTableWidget4->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("时间")<<QStringLiteral("卡号"));
    ui->cardTableWidget4->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->cardTableWidget4->verticalHeader()->setFixedWidth (100);
    ui->cardTableWidget4->verticalHeader()->setDefaultAlignment (Qt::AlignCenter);
    ui->cardTableWidget4->setFocusPolicy(Qt::NoFocus);//去掉虚框
    ui->cardTableWidget4->verticalHeader ()->hide ();
    ui->cardTableWidget4->setColumnWidth(0,ui->cardTableWidget4->width()*1.4);
    ui->cardTableWidget4->setColumnWidth(1,ui->cardTableWidget4->width()*1.4);


    int cardNum1,cardNum2,cardNum3,cardNum4;
    int rowNum1=0,rowNum2=0,rowNum3=0,rowNum4=0;
    QString time1,time2,time3,time4;
    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlQuery query(db);

    bool succ1=query.exec(QString("select cardNum,time from ReadCardTable where agvNum=%1 order by stramp desc").arg(1));
    while (succ1&&query.next()) {
        time1=query.value("time").toString();
        cardNum1=query.value("cardNum").toInt();

        ui->cardTableWidget1->setRowCount(rowNum1+1);
        ui->cardTableWidget1->setItem (rowNum1,0,new QTableWidgetItem(time1));
        ui->cardTableWidget1->item(rowNum1,0)->setTextAlignment(Qt::AlignCenter);
        ui->cardTableWidget1->setItem (rowNum1,1,new QTableWidgetItem(QString::number (cardNum1)));
        ui->cardTableWidget1->item(rowNum1,1)->setTextAlignment(Qt::AlignCenter);
        rowNum1++;
    }

    bool succ2=query.exec(QString("select cardNum,time from ReadCardTable where agvNum=%1 order by stramp desc").arg(2));
    //query.last();
    while (succ2&&query.next()) {
        time2=query.value("time").toString();
        cardNum2=query.value("cardNum").toInt();
        ui->cardTableWidget2->setRowCount(rowNum2+1);
        ui->cardTableWidget2->setItem (rowNum2,0,new QTableWidgetItem(time2));
        ui->cardTableWidget2->item(rowNum2,0)->setTextAlignment(Qt::AlignCenter);
        ui->cardTableWidget2->setItem (rowNum2,1,new QTableWidgetItem(QString::number (cardNum2)));
        ui->cardTableWidget2->item(rowNum2,1)->setTextAlignment(Qt::AlignCenter);
        rowNum2++;
    }

    bool succ3=query.exec(QString("select cardNum,time from ReadCardTable where agvNum=%1 order by stramp desc").arg(3));
    //query.last();
    while (succ3&&query.next()) {
        time3=query.value("time").toString();
        cardNum3=query.value("cardNum").toInt();
        ui->cardTableWidget3->setRowCount(rowNum3+1);
        ui->cardTableWidget3->setItem (rowNum3,0,new QTableWidgetItem(time3));
        ui->cardTableWidget3->item(rowNum3,0)->setTextAlignment(Qt::AlignCenter);
        ui->cardTableWidget3->setItem (rowNum3,1,new QTableWidgetItem(QString::number (cardNum3)));
        ui->cardTableWidget3->item(rowNum3,1)->setTextAlignment(Qt::AlignCenter);
        rowNum3++;
    }

    bool succ4=query.exec(QString("select cardNum,time from ReadCardTable where agvNum=%1 order by stramp desc").arg(4));
    //query.last();
    while (succ4&&query.next()) {
        time4=query.value("time").toString();
        cardNum4=query.value("cardNum").toInt();
        ui->cardTableWidget4->setRowCount(rowNum4+1);
        ui->cardTableWidget4->setItem (rowNum4,0,new QTableWidgetItem(time4));
        ui->cardTableWidget4->item(rowNum4,0)->setTextAlignment(Qt::AlignCenter);
        ui->cardTableWidget4->setItem (rowNum4,1,new QTableWidgetItem(QString::number (cardNum4)));
        ui->cardTableWidget4->item(rowNum4,1)->setTextAlignment(Qt::AlignCenter);
        rowNum4++;
    }

   // Singleton<ConnectionPool>::getInstance().releaseConnection(db);

    //QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();





    ui->tableWidget_mapPoint->setColumnCount (3);
    ui->tableWidget_mapPoint->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("地表卡号")<<QStringLiteral("X地标位置")<<QStringLiteral("地标位置"));
    ui->tableWidget_mapPoint->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    //ui->tableWidget_mapPoint->verticalHeader()->setFixedWidth (100);
    ui->tableWidget_mapPoint->verticalHeader()->setDefaultAlignment (Qt::AlignCenter);
    ui->tableWidget_mapPoint->setFocusPolicy(Qt::NoFocus);//去掉虚框
    ui->tableWidget_mapPoint->verticalHeader ()->hide ();
    //for(int i=0;i<ui->tableWidget_mapPoint->columnCount();i++)
    //{

    //    ui->tableWidget_mapPoint->setColumnWidth(i,ui->tableWidget_mapPoint->width()*2.5);//设置stationInfoTableWidget的列宽
    //}
    ui->tableWidget_mapPoint->setColumnWidth(0,ui->tableWidget_mapPoint->width()*1.4);
    ui->tableWidget_mapPoint->setColumnWidth(1,ui->tableWidget_mapPoint->width()*1.4);
    ui->tableWidget_mapPoint->setColumnWidth(2,ui->tableWidget_mapPoint->width()*1.4);

    bool succ5=query.exec(QString("select * from mapPoint"));
    rowNum4 = 0;
    while (succ5&&query.next()) {
        int num =query.value("cardPoint").toInt();
        int x =query.value("X").toInt();
        int y = query.value("Y").toInt();
        ui->tableWidget_mapPoint->setRowCount(rowNum4+1);
        ui->tableWidget_mapPoint->setItem (rowNum4,0,new QTableWidgetItem(QString::number(num)));
        ui->tableWidget_mapPoint->item(rowNum4,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_mapPoint->setItem (rowNum4,1,new QTableWidgetItem(QString::number (x)));
        ui->tableWidget_mapPoint->item(rowNum4,1)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_mapPoint->setItem (rowNum4,2,new QTableWidgetItem(QString::number (y)));
        ui->tableWidget_mapPoint->item(rowNum4,1)->setTextAlignment(Qt::AlignCenter);
        rowNum4++;
    }

/* */


    Singleton<ConnectionPool>::getInstance().releaseConnection(db);




}

ReadCardWindow::~ReadCardWindow()
{
    delete ui;
}
