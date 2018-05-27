#include "ModifyPwdWindow.h"
#include "ui_ModifyPwdWindow.h"
#include <QTime>
#include "css/WidgetDraw.h"

ModifyPwdWindow::ModifyPwdWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModifyPwdWindow)
{
    ui->setupUi(this);
    ui->oldPwd_lineEdit->setEchoMode (QLineEdit::Password);
    ui->oldPwd_lineEdit->setPlaceholderText (QStringLiteral("请输入旧密码"));
    ui->newPwd_lineEdit->setEchoMode (QLineEdit::Password);
    ui->newPwd_lineEdit->setPlaceholderText (QStringLiteral("请输入新密码"));
    ui->newPwd2_lineEdit->setEchoMode (QLineEdit::Password);
    ui->newPwd2_lineEdit->setPlaceholderText (QStringLiteral("请再次输入新密码"));
    ui->verificationCode_lineEdit->setPlaceholderText (QStringLiteral("请输入验证码"));
    //设置验证码
    QTime timer;
    timer=QTime::currentTime ();
    qsrand(timer.msec ()+timer.second ()*10000);
    this->setVerificationCode();
}

ModifyPwdWindow::~ModifyPwdWindow()
{
    delete ui;
}
void ModifyPwdWindow::setVerificationCode (){
    verificationCode=rand()%10000;
    while (verificationCode<1000) {
        verificationCode=rand()%10000;
    }
    ui->verificationCode_label->setText (QString::number (verificationCode));
}
void ModifyPwdWindow::on_sureModifyBtn_clicked ()
{
    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlQuery query(db);
    QString user,pwd;
    query.exec ("select user,password from userinfo");
    while (query.next ()) {
        user=query.value ("user").toString ();
        pwd= query.value("password").toString ();//获取密码
    }
    if(pwd!=ui->oldPwd_lineEdit->text ())
    {
      WidgetDraw::ShowMessageBoxWarning (QStringLiteral("旧密码输入有误,请重新设置"));
        return;
    }
    if(ui->newPwd_lineEdit->text ()!=ui->newPwd2_lineEdit->text ())
    {
        WidgetDraw::ShowMessageBoxWarning (QStringLiteral("两次密码输入不一致,请重新设置"));
        return;
    }
    if(ui->oldPwd_lineEdit->text ()==ui->newPwd_lineEdit->text ())
    {
        WidgetDraw::ShowMessageBoxWarning (QStringLiteral("新旧密码一致,请重新设置"));
        return;
    }
    if(ui->verificationCode_label->text ()!=ui->verificationCode_lineEdit->text ())
    {
        WidgetDraw::ShowMessageBoxWarning (QStringLiteral("验证码有误,请重新设置"));
        return;
    }
    if(pwd==ui->oldPwd_lineEdit->text ()&&ui->newPwd_lineEdit->text ()==ui->newPwd2_lineEdit->text ()&&ui->verificationCode_label->text ()==ui->verificationCode_lineEdit->text ())
    {
        qDebug()<<pwd<<ui->newPwd_lineEdit->text ()<<user;
        QString newPwd=ui->newPwd_lineEdit->text ();
        bool succ=query.exec (QString("update userinfo set password='%1' where user='%2'").arg (newPwd).arg (user));
        if(succ)
        {
            WidgetDraw::ShowMessageBoxInfo (QStringLiteral("密码修改成功"));
        }
        else
            WidgetDraw::ShowMessageBoxWarning (QStringLiteral("密码修改失败"));
    }
    Singleton<ConnectionPool>::getInstance().releaseConnection(db);

}
void ModifyPwdWindow::on_cancelModifyBtn_clicked ()
{
    this->close ();
}
