#include "MainWindow.h"
#include <QApplication>
#include "LoginDialog.h"
#include "agv/AgvSymbol.h"
#include <QThread>
#include <QStyleFactory>
#include <QDesktopWidget>
#include "css/WidgetDraw.h"

#include <QSqlDriver>
#include <QSqlDatabase>
#include <QDebug>

#include "pages/MainPage.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WidgetDraw::SetUTF8Code ();
    WidgetDraw::SetStyle ("blue");
    WidgetDraw::SetChinese ();
    QApplication::setStyle(QStyleFactory::create("Plastique"));

    qDebug()<<"Available drivers:";
    QStringList drivers=QSqlDatabase::drivers();
    foreach (QString driver, drivers)
    {
        qDebug()<<driver;
    }


    qRegisterMetaType<HITAGV::ActionSymbol>("HITAGV::ActionSymbol");
    qRegisterMetaType<QHostAddress>("QHostAddress");
    LoginDialog dlg;
    if(dlg.exec ()==QDialog::Accepted){

    //    MainWindow w;
    //    w.show();
    //    w.showMaximized();//窗口最大化
        MainPage w;
        w.show();
        w.showMaximized();//窗口最大化
        return a.exec();
    }
    else
    {
        return 0;
    }


}
