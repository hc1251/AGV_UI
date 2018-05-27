#include "DebugWindow.h"
#include "ui_DebugWindow.h"

DebugWindow::DebugWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DebugWindow)
{
    ui->setupUi(this);
    this->setAttribute (Qt::WA_DeleteOnClose);
    ui->sendContext->setMaximumBlockCount (1000);
    ui->receiveContext->setMaximumBlockCount (1000);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);//去掉窗口中的？号按钮
    setWindowFlags (windowFlags ()|Qt::WindowMinimizeButtonHint);
    setWindowFlags (windowFlags ()|Qt::WindowMaximizeButtonHint);

}

DebugWindow::~DebugWindow()
{
   delete ui;
}
void DebugWindow::slotSendContext(QByteArray text)
{
    ui->sendContext->appendPlainText(QString(text.toHex()));
}

void DebugWindow::slotReceiveContext(QByteArray text)
{
    ui->receiveContext->appendPlainText(QString(text.toHex()));
}
void DebugWindow::on_receiveTxtClearBtn_clicked()
{
    ui->receiveContext->clear();
}
void DebugWindow::on_sendTxtClearBtn_clicked ()
{
    ui->sendContext->clear();
}
