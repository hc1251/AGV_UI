#include "pointinput.h"
#include "ui_pointinput.h"

PointInput::PointInput(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PointInput)
{
    ui->setupUi(this);
}

PointInput::~PointInput()
{
    delete ui;
}

void PointInput::on_insertButton_clicked()
{
    if(ui->pointEdit->text().isEmpty())
    {
        QMessageBox::information(this,QStringLiteral("提示"),QStringLiteral("卡号不能为空"));
        return;
    }
    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlQuery query(db);
    bool succ=query.exec("select* from mapPoint where cardPoint = " + ui->pointEdit->text());
    if(succ&&query.first())
    {
        QMessageBox::information(this,QStringLiteral("提示"),QStringLiteral("已存在卡号"));
        Singleton<ConnectionPool>::getInstance().releaseConnection(db);
        return;
    }
    else
    {
//        QString insertStr = QString("insert into MapPoint(cardPoint,x,y) values(%1,%2,%3)")
//                .arg(ui->pointEdit->text().toInt()).arg(ui->xEdit->text().toUInt()).arg(ui->yEdit->text().toUInt());
        query.prepare("insert into mapPoint(cardPoint,x,y) values(:cardPoint,:x,:y)");
        query.bindValue(":cardPoint",ui->pointEdit->text().toInt());
        query.bindValue(":x",ui->xEdit->text().toInt());
        query.bindValue(":y",ui->yEdit->text().toInt());
        query.exec();
    }
    this->x = ui->xEdit->text().toUInt();
    this->y = ui->yEdit->text().toUInt();
    this->card = ui->pointEdit->text();
    emit addPoint();
    this->hide();
    Singleton<ConnectionPool>::getInstance().releaseConnection(db);
}

void PointInput::on_deleteButton_clicked()
{
    if(ui->pointEdit->text().isEmpty())
    {
        QMessageBox::information(this,QStringLiteral("提示"),QStringLiteral("卡号不能为空"));
        return;
    }
    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlQuery query(db);
    query.exec(QString("delete from mapPoint where cardPoint = %1").arg(ui->pointEdit->text().toInt()));
    emit deletePoint(ui->pointEdit->text());
    this->hide();
    Singleton<ConnectionPool>::getInstance().releaseConnection(db);
}

void PointInput::setPoint(QPoint tempPoint)
{
    nowPoint = tempPoint;
    QString xStr,yStr;
    xStr.setNum(nowPoint.x());
    yStr.setNum(nowPoint.y());
    ui->xEdit->setText(xStr);
    ui->yEdit->setText(yStr);
}
