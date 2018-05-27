#include "LoginDialog.h"
#include "ui_LoginDialog.h"
#include "css/WidgetDraw.h"


LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    QImage image,result;
    image.load(":images/AgvImg.png");
    result = image.scaled(800, 600).scaled(70, 70, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//放缩图片，以固定大小显示
    ui->agvimg_label->setPixmap(QPixmap::fromImage(result));//在Label控件上显示图片
    image.load(":images/agvImg1.png");
    result = image.scaled(800, 600).scaled(230, 150, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//放缩图片，以固定大小显示
    ui->agvimg_label_2->setPixmap(QPixmap::fromImage(result));//在Label控件上显示图片
    setWindowTitle (QStringLiteral("登陆窗口"));
    ui->LoginBtn->setDefault (true);
    //打开数据库
    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlQuery query(db);
    //query.exec ("select userid,password,checkbox from userinfo");
    query.exec ("select * from userinfo");
    while (query.next ()) {
        username= query.value("userid").toString ();
        psw= query.value("password").toString ();//获取卡片的坐标
        checkvvalue=query.value ("checkbox").toBool ();
    }
    if(checkvvalue)
    {
        ui->pwdcheckBox->setChecked (true);
        ui->user_lineEdit->setText (username);
        ui->password_lineEdit->setText (psw);
    }
    Singleton<ConnectionPool>::getInstance().releaseConnection(db);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);//去掉窗口中的？号按钮
}
LoginDialog::~LoginDialog()
{
    delete ui;
}
void LoginDialog::on_pwdcheckBox_clicked (){
    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().getConnection();
    QSqlQuery query(db);
    bool check;
    if((ui->pwdcheckBox->isChecked ()&&(ui->user_lineEdit->text ()==username)&&(ui->password_lineEdit->text ()==psw)))
    {
        check=true;
    }
    else{
        check=false;
    }
    query.exec (QString("update userinfo set checkbox = '%1'").arg (check));
    Singleton<ConnectionPool>::getInstance().releaseConnection(db);
}
void LoginDialog::on_LoginBtn_clicked ()
{
    if((ui->user_lineEdit->text ()==username)&&(ui->password_lineEdit->text ()==psw))
    {

        QDialog::accept ();
    }
    else{
        WidgetDraw::ShowMessageBoxWarning (QStringLiteral("请输入正确的用户名和密码!"));
    }
}

void LoginDialog::on_QuitBtn_clicked (){
    QDialog::reject ();
}
