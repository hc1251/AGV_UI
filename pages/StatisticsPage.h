#ifndef STATISTICSPAGE_H
#define STATISTICSPAGE_H

#include <QWidget>
#include "qcustomplot.h"

namespace Ui {
class StatisticsPage;
}

class StatisticsPage : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticsPage(QWidget *parent = 0);
    ~StatisticsPage();

private:
    Ui::StatisticsPage *ui;
     void setupBarChartDemo(QCustomPlot *customPlot);
         void setupStyledDemo(QCustomPlot *customPlot);
};

#endif // STATISTICSPAGE_H
