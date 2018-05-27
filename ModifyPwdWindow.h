#ifndef MODIFYPWDWINDOW_H
#define MODIFYPWDWINDOW_H

#include <QWidget>
#include "database/ConnectionPool.h"
#include <QSqlDatabase>

namespace Ui {
class ModifyPwdWindow;
}

class ModifyPwdWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ModifyPwdWindow(QWidget *parent = 0);
    ~ModifyPwdWindow();

private:
    Ui::ModifyPwdWindow *ui;
    int verificationCode;
    void setVerificationCode();
private slots:
    void on_sureModifyBtn_clicked();
    void on_cancelModifyBtn_clicked();
};

#endif // MODIFYPWDWINDOW_H
