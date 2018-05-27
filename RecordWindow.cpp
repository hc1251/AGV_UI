#include "RecordWindow.h"
#include "ui_RecordWindow.h"

RecordWindow::RecordWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecordWindow)
{
    ui->setupUi(this);
    this->setAttribute (Qt::WA_DeleteOnClose);
    this->showNormal ();
}

RecordWindow::~RecordWindow()
{
    if(AgvSymbol::haveInitBatteryWindow && whichWindow == BatteryWindow)
        AgvSymbol::haveInitBatteryWindow = false;
    if(AgvSymbol::haveInitDepartureWindow && whichWindow == DepartureWindow)
        AgvSymbol::haveInitDepartureWindow = false;
    emit sigCloseRecordWindow();
    delete ui;
}

void RecordWindow::initBatteryWindowShow()
{
    this->setWindowTitle(QStringLiteral("电池更换记录"));
    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlTableModel *model = new QSqlTableModel(this,db);
    model->setTable("batteryTable");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    model->setHeaderData(0, Qt::Horizontal, QStringLiteral("小车编号"));
    model->setHeaderData(1, Qt::Horizontal, QStringLiteral("更换电池时间"));
    model->removeColumn(2);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setModel(model);
    ui->tableView->setColumnWidth(0,ui->tableView->width()/3);
    ui->tableView->setColumnWidth(1,ui->tableView->width()*2/3);
    Singleton<ConnectionPool>::getInstance().releaseConnection(db);
    batteryRecordSummary();
}

void RecordWindow::initDepartureAgvWindowShow()
{
    this->setWindowTitle( QStringLiteral("小车发车记录"));
    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlTableModel *model = new QSqlTableModel(this,db);
    model->setTable("AgvSchedule");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    model->setHeaderData(0, Qt::Horizontal,  QStringLiteral("小车编号"));
    model->setHeaderData(1, Qt::Horizontal,  QStringLiteral("运行路线"));
    model->setHeaderData(2, Qt::Horizontal,  QStringLiteral("发车时间"));
    model->removeColumn(3);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setModel(model);
    ui->tableView->setColumnWidth(0,ui->tableView->width()/3);
    ui->tableView->setColumnWidth(1,ui->tableView->width()/3);
    ui->tableView->setColumnWidth(2,ui->tableView->width()/3);
    Singleton<ConnectionPool>::getInstance().releaseConnection(db);
    departureFreqRecordSummary();
}

void RecordWindow::batteryRecordSummary()
{
    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlQuery query(db);
    uint stramp = QDateTime::currentDateTime().toTime_t();
    uint temp = stramp - (7 * 24 * 60 * 60 );
    query.exec(QString("select agvNum as xm,count(agvNum) as cs from batteryTable where stramp >= %1 group by agvNum order by agvNum").arg(temp));
    while(query.next()){
        ui->plainTextEdit->appendPlainText( QStringLiteral("%1号车最近一周更换电池 %2 次").arg(query.value(0).toInt()).arg(query.value(1).toInt()));
    }
    Singleton<ConnectionPool>::getInstance().releaseConnection(db);
}

void RecordWindow::departureFreqRecordSummary()
{
    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlQuery query(db);
    uint stramp = QDateTime::currentDateTime().toTime_t();
    uint temp = stramp - (7 * 24 * 60 * 60 );
    query.exec(QString("select agvNum as xm,count(agvNum) as cs from AgvSchedule where stramp >= %1 group by agvNum order by agvNum").arg(temp));
    while(query.next()){
        ui->plainTextEdit->appendPlainText( QStringLiteral("%1号车最近一周发车 %2 次").arg(query.value(0).toInt()).arg(query.value(1).toInt()));
    }
    Singleton<ConnectionPool>::getInstance().releaseConnection(db);
}

void RecordWindow::resizeEvent(QResizeEvent *)
{
    if(AgvSymbol::haveInitBatteryWindow && whichWindow == BatteryWindow){
        ui->tableView->setColumnWidth(0,ui->tableView->width()/3);
        ui->tableView->setColumnWidth(1,ui->tableView->width()*2/3);
    }
    if(AgvSymbol::haveInitDepartureWindow && whichWindow == DepartureWindow){
        ui->tableView->setColumnWidth(0,ui->tableView->width()/3);
        ui->tableView->setColumnWidth(1,ui->tableView->width()/3);
        ui->tableView->setColumnWidth(2,ui->tableView->width()/3);
    }
}
