#include "StatisticShowPage.h"
#include "ui_StatisticShowPage.h"
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QDateTime>

QT_CHARTS_USE_NAMESPACE

StatisticShowPage::StatisticShowPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticShowPage)
{
    ui->setupUi(this);

    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy-MM-dd");
    ui->groupBox->setTitle(current_date+QStringLiteral("  小车利用率"));
    QPieSeries *series = new QPieSeries();
    series->append("free:2h", 2);
    series->append("run:10h", 10);
    series->setLabelsVisible();

    QPieSlice *slice0 = series->slices().at(0);
    QPieSlice *slice1 = series->slices().at(1);
    slice0->setColor(QColor(255,0,0,255));
    slice1->setColor(QColor(0,255,0,255));

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(QStringLiteral("1号车"));
    chart->setFont(QFont("Times",18,QFont::Bold));
    chart->legend()->hide();
    ui->graphicsView->setChart(chart);

    QPieSeries *series2 = new QPieSeries();
    series2->append("free:3h",3);
    series2->append("run:8h", 8);
    series2->setLabelsVisible();
    QPieSlice *slice02 = series2->slices().at(0);
    QPieSlice *slice12 = series2->slices().at(1);
    slice02->setColor(QColor(255,0,0,255));
    slice12->setColor(QColor(0,255,0,255));
    QChart *chart2 = new QChart();
    chart2->addSeries(series2);
    chart2->setTitle(QStringLiteral("2号车"));
    chart2->setFont(QFont("Times",18,QFont::Bold));
    chart2->legend()->hide();
    ui->graphicsView_2->setChart(chart2);

    QPieSeries *series3 = new QPieSeries();
    series3->append("free:2.5h", 2.5);
    series3->append("run:9h", 9);
    series3->setLabelsVisible();
    QPieSlice *slice03 = series3->slices().at(0);
    QPieSlice *slice13 = series3->slices().at(1);
    slice03->setColor(QColor(255,0,0,255));
    slice13->setColor(QColor(0,255,0,255));
    QChart *chart3 = new QChart();
    chart3->addSeries(series3);
    chart3->setTitle(QStringLiteral("3号车"));
    chart3->setFont(QFont("Times",18,QFont::Bold));
    chart3->legend()->hide();
    ui->graphicsView_3->setChart(chart3);

    QPieSeries *series4 = new QPieSeries();
    series4->append("free:1h", 1);
    series4->append("run:4h", 4);
    series4->setLabelsVisible();
    QPieSlice *slice04 = series4->slices().at(0);
    QPieSlice *slice14 = series4->slices().at(1);
    slice04->setColor(QColor(255,0,0,255));
    slice14->setColor(QColor(0,255,0,255));
    QChart *chart4 = new QChart();
    chart4->addSeries(series4);
    chart4->setTitle(QStringLiteral("4号车"));
    chart4->setFont(QFont("Times",18,QFont::Bold));
    chart4->legend()->hide();
    ui->graphicsView_4->setChart(chart4);

    setupStyledDemo(ui->widget);
}


void StatisticShowPage::setupStyledDemo(QCustomPlot *customPlot)
{
    customPlot->legend->setVisible(true);
    customPlot->legend->setFont(QFont("Helvetica", 9));
    customPlot->legend->setRowSpacing(-3);
    QVector<QCPScatterStyle::ScatterShape> shapes;
    QVector<QString> names;
    names<<QStringLiteral("1号车")<<QStringLiteral("2号车")<<QStringLiteral("3号车")<<QStringLiteral("4号车");
    shapes << QCPScatterStyle::ssCircle;
    shapes << QCPScatterStyle::ssDisc;
    shapes << QCPScatterStyle::ssTriangle;
    shapes << QCPScatterStyle::ssStar;
    QVector<double> fullData1,fullData2,fullData3,fullData4;
    fullData1<<10<<11<<10<<6<<6<<12.5<<12;

    fullData2<<8.9<<8<<12<<8.6<<9<<16<<11;
    fullData3<<7.8<<9<<10.5<<9<<9.5<<14<<11.5;
    fullData4<<12<<5<<11.5<<6<<4<<8<<5;
    QPen pen;
    // add graphs with different scatter styles:

    for (int i=0; i<shapes.size(); i++)
    {customPlot->addGraph();

        pen.setColor(QColor(i*255/4, 255-i*255/4, 255));
        // generate data:
        QVector<double> x(7), y(7);
        for (int k=0; k<7; k++)
        {
            x[k] =k;
            if(i==0)
            {
                y[k] = fullData1[k];
            }
            else if(i==1)
            {
                y[k]=fullData2[k];
            }
            else if(i==2)
            {
                y[k]=fullData3[k];
            }
            else
            {
                y[k]=fullData4[k];
            }
        }
        customPlot->graph(i)->setData(x, y);
        customPlot->graph(i)->rescaleAxes(true);
        customPlot->graph(i)->setPen(pen);
        customPlot->graph(i)->setName(names[i]);
        customPlot->graph(i)->setLineStyle(QCPGraph::lsLine);
        // set scatter style:
        if (shapes.at(i) != QCPScatterStyle::ssCustom)
        {
            customPlot->graph(i)->setScatterStyle(QCPScatterStyle(shapes.at(i), 10));
        }
        else
        {
            QPainterPath customScatterPath;
            for (int i=0; i<3; ++i)
                customScatterPath.cubicTo(qCos(2*M_PI*i/3.0)*9, qSin(2*M_PI*i/3.0)*9, qCos(2*M_PI*(i+0.9)/3.0)*9, qSin(2*M_PI*(i+0.9)/3.0)*9, 0, 0);
            customPlot->graph()->setScatterStyle(QCPScatterStyle(customScatterPath, QPen(Qt::black, 0), QColor(40, 70, 255, 50), 10));
        }
        customPlot->graph(i)->setScatterStyle(QCPScatterStyle(shapes.at(i), 10));
    }
    // set blank axis lines:
    customPlot->rescaleAxes();
    customPlot->xAxis->setLabel(QStringLiteral("天数（天）"));
    customPlot->yAxis->setLabel(QStringLiteral("小车运行时间（h）"));
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(0, 6);
    customPlot->yAxis->setRange(0, 18);
    //  customPlot->xAxis->setTicks(false);
    // customPlot->yAxis->setTicks(false);
    // customPlot->xAxis->setTickLabels(false);
    //customPlot->yAxis->setTickLabels(false);
    // make top right axes clones of bottom left axes:
    //customPlot->axisRect()->setupFullAxesBox();
}


StatisticShowPage::~StatisticShowPage()
{
    delete ui;
}
