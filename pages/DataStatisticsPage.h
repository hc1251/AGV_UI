#ifndef DATASTATISTICSPAGE_H
#define DATASTATISTICSPAGE_H

#include <QWidget>
#include <QButtonGroup>

namespace Ui {
class DataStatisticsPage;
}

class DataStatisticsPage : public QWidget
{
    Q_OBJECT

public:
    explicit DataStatisticsPage(QWidget *parent = 0);
    ~DataStatisticsPage();

private:
    Ui::DataStatisticsPage *ui;
    QButtonGroup *btnGroup;
    void  allShow();
    void dayShow();
    void weekShow();
    void monthShow();
    void initUI();
public slots:
    void onRadioClick();

};

#endif // DATASTATISTICSPAGE_H
