#include "FrmMessagebox.h"
#include "ui_FrmMessagebox.h"
#include "IconDraw.h"
#include "WidgetDraw.h"

frmMessageBox::frmMessageBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmMessageBox)
{
    ui->setupUi(this);

    this->mousePressed = false;
    //设置窗体标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint);
    //设置窗体关闭时自动释放内存
    this->setAttribute(Qt::WA_DeleteOnClose);
    //设置图形字体
    IconDraw::Instance()->SetIcon(ui->lab_Ico, QChar(0xf015), 12);
    IconDraw::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    //关联关闭按钮
    connect(ui->btnMenu_Close, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
    //窗体居中显示
  //  myHelper::FormInCenter(this);

}

frmMessageBox::~frmMessageBox()
{
    delete ui;
}

void frmMessageBox::SetMessage(const QString &msg, int type)
{
    if (type == 0) {
        ui->labIcoMain->setStyleSheet("border-image: url(:/image/info.png);");
        ui->btnCancel->setVisible(false);
        ui->lab_Title->setText(QStringLiteral("提示"));
    } else if (type == 1) {
        ui->labIcoMain->setStyleSheet("border-image: url(:/image/question.png);");
        ui->lab_Title->setText(QStringLiteral("询问"));
    } else if (type == 2) {
        ui->labIcoMain->setStyleSheet("border-image: url(:/image/error.png);");
        ui->btnCancel->setVisible(false);
        ui->lab_Title->setText(QStringLiteral("错误"));
    }
    else if(type==3)
    {
        ui->labIcoMain->setStyleSheet("border-image: url(:/image/warning.png);");
        ui->btnCancel->setVisible(false);
        ui->lab_Title->setText(QStringLiteral("警告aaa"));
    }
    else if(type==4)
    {
        ui->labIcoMain->setStyleSheet("border-image: url(:/image/hit.png);");
        ui->btnCancel->setVisible(false);
        ui->btnOk->setVisible (false);
        ui->lab_Title->setText(QStringLiteral("关于"));
    }
    ui->labInfo->setText(msg);
}

void frmMessageBox::on_btnOk_clicked()
{
    done(1);
    this->close();
}

void frmMessageBox::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton)) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void frmMessageBox::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void frmMessageBox::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void frmMessageBox::on_btnCancel_clicked()
{
    //return;
}
