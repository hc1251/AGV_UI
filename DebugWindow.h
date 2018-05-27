#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include <QDialog>

namespace Ui {
class DebugWindow;
}

class DebugWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DebugWindow(QWidget *parent = 0);
    ~DebugWindow();

private:
    Ui::DebugWindow *ui;
public slots:
    void slotSendContext(QByteArray text);
    void slotReceiveContext(QByteArray text);
private slots:
    void on_sendTxtClearBtn_clicked();
    void on_receiveTxtClearBtn_clicked();

};

#endif // DEBUGWINDOW_H
