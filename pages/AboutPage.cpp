#include "AboutPage.h"
#include "ui_AboutPage.h"

AboutPage::AboutPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutPage)
{
    ui->setupUi(this);
     //ui->context_label->adjustSize();

    ui->context_label->setGeometry(QRect(328, 240, 329, 27*4));  //四倍行距
    ui->context_label->setWordWrap(true);
    ui->context_label->setAlignment(Qt::AlignTop);
    ui->context_label->setText(QStringLiteral("\n\n\n公司网址：http:// www.hitrobot.com.cn;事业部网址：http:// www.myagv.cn/"));

    QImage image,result;
    image.load(":images/dxAgv.jpg");
//    result = image.scaled(800, 600).scaled(300, 300, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//放缩图片，以固定大小显示
    result = image.scaled(800, 600).scaled(220, 220, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//放缩图片，以固定大小显示
    ui->label_agvimg1->setPixmap(QPixmap::fromImage(result));//在Label控件上显示图片
    image.load(":images/sxAgv.jpg");
    result = image.scaled(800, 600).scaled(220, 220, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//放缩图片，以固定大小显示
    ui->label_agvimg2->setPixmap(QPixmap::fromImage(result));//在Label控件上显示图片
    image.load(":images/qxAgv.jpg");
    result = image.scaled(800, 600).scaled(220, 220, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//放缩图片，以固定大小显示
    ui->label_agvimg3->setPixmap(QPixmap::fromImage(result));//在Label控件上显示图片
    image.load(":images/jgAgv.jpg");
    result = image.scaled(800, 600).scaled(220, 220, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//放缩图片，以固定大小显示
    ui->label_agvimg4->setPixmap(QPixmap::fromImage(result));//在Label控件上显示图片
    image.load(":images/ydAgv.jpg");
    result = image.scaled(800, 600).scaled(220, 220, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//放缩图片，以固定大小显示
    ui->label_agvimg5->setPixmap(QPixmap::fromImage(result));//在Label控件上显示图片
    image.load(":images/gtAgv.jpg");
    result = image.scaled(800, 600).scaled(220, 220, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//放缩图片，以固定大小显示
    ui->label_agvimg6->setPixmap(QPixmap::fromImage(result));//在Label控件上显示图片


}

AboutPage::~AboutPage()
{
    delete ui;
}


