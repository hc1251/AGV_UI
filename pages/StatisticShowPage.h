#ifndef STATISTICSHOWPAGE_H
#define STATISTICSHOWPAGE_H

#include <QWidget>
#include "qcustomplot.h"

namespace Ui {
class StatisticShowPage;
}

class StatisticShowPage : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticShowPage(QWidget *parent = 0);
    ~StatisticShowPage();

private:
    Ui::StatisticShowPage *ui;
       void setupStyledDemo(QCustomPlot *customPlot);
};

#endif // STATISTICSHOWPAGE_H
