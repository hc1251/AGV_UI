#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "database/ConnectionPool.h"
#include <QSqlDatabase>
namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

private:
    Ui::LoginDialog *ui;
private slots:
    void on_LoginBtn_clicked();
    void on_QuitBtn_clicked();
    void on_pwdcheckBox_clicked();

private :
    QString username,psw;
    bool checkvvalue;

};

#endif // LOGINDIALOG_H
