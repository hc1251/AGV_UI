#include "StationBtnWindow.h"
#include "ui_StationBtnWindow.h"
#include <QDebug>

StationBtnWindow::StationBtnWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StationBtnWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);//去掉窗口中的？号按钮
    setWindowFlags (windowFlags ()|Qt::WindowMinimizeButtonHint);
    setWindowFlags (windowFlags ()|Qt::WindowMaximizeButtonHint);
    this->setWindowTitle(QStringLiteral("请选择启动工位"));
    stationBtnGroup=new QButtonGroup(this);
    stationBtnGroup->addButton(ui->pushButton_3,0);
    stationBtnGroup->addButton(ui->pushButton_4,1);
    stationBtnGroup->addButton(ui->pushButton_5,2);
    stationBtnGroup->addButton(ui->pushButton_6,3);
    stationBtnGroup->addButton(ui->pushButton_7,4);
    stationBtnGroup->addButton(ui->pushButton_8,5);
    stationBtnGroup->addButton(ui->pushButton_9,6);
    stationBtnGroup->addButton(ui->pushButton_10,7);
    stationBtnGroup->addButton(ui->pushButton_11,8);
    stationBtnGroup->addButton(ui->pushButton_12,9);
   // ui->radioButton->setChecked(true);
    connect(stationBtnGroup,SIGNAL(buttonClicked(int)),SLOT(on_pushButton_clicked(int)));
}

StationBtnWindow::~StationBtnWindow()
{
    delete ui;
}

void StationBtnWindow::on_pushButton_clicked(int buttonID)
{
QString stationName="";
    switch(buttonID)
    {
    case 0:
        stationName=ui->pushButton_3->text();
        break;
    case 1:
        stationName=ui->pushButton_4->text();
        break;
    case 2:
        stationName=ui->pushButton_5->text();
        break;
    case 3:
        stationName=ui->pushButton_6->text();
        break;
    case 4:
        stationName=ui->pushButton_7->text();
        break;
    case 5:
        stationName=ui->pushButton_8->text();
        break;
    case 6:
        stationName=ui->pushButton_9->text();
        break;
    case 7:
        stationName=ui->pushButton_10->text();
        break;
    case 8:
        stationName=ui->pushButton_11->text();
        break;
    case 9:
        stationName=ui->pushButton_12->text();
        break;
    default:
        stationName=QStringLiteral("选择工位111");
        break;
    }
    emit sendSelectStationInfo(stationName);
    this->close();

}

void StationBtnWindow::on_pushButton_2_clicked()
{
    this->close();
}
