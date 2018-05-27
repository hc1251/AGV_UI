#include "StatisticsPage.h"
#include "ui_statisticspage.h"

StatisticsPage::StatisticsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticsPage)
{
    ui->setupUi(this);
      setupBarChartDemo(ui->widget);
       setupStyledDemo(ui->widget_2);
}

StatisticsPage::~StatisticsPage()
{
    delete ui;
}


void StatisticsPage::setupBarChartDemo(QCustomPlot *customPlot)
{
//  demoName = "Bar Chart Demo";
  // set dark background gradient:
  QLinearGradient gradient(0, 0, 0, 400);
  gradient.setColorAt(0, QColor(90, 90, 90));
  gradient.setColorAt(0.38, QColor(105, 105, 105));
  gradient.setColorAt(1, QColor(70, 70, 70));
  customPlot->setBackground(QBrush(gradient));

  // create empty bar chart objects:
// QCPBars *regen = new QCPBars(customPlot->xAxis, customPlot->yAxis);
  QCPBars *nuclear = new QCPBars(customPlot->xAxis, customPlot->yAxis);
  QCPBars *fossil = new QCPBars(customPlot->xAxis, customPlot->yAxis);
 // regen->setAntialiased(false); // gives more crisp, pixel aligned bar borders
  nuclear->setWidth(0.4);
  fossil->setWidth(0.4);
 nuclear->setAntialiased(false);
  fossil->setAntialiased(false);
  //regen->setStackingGap(1);
  nuclear->setStackingGap(1);
  fossil->setStackingGap(1);
  // set names and colors:
  fossil->setName( QStringLiteral("运行时间"));
  fossil->setPen(QPen(QColor(0, 255, 0).lighter(170)));
  fossil->setBrush(QColor(0, 255, 0));
 nuclear->setName(QStringLiteral("空闲时间"));
 nuclear->setPen(QPen(QColor(255, 0, 0).lighter(150)));
 nuclear->setBrush(QColor(255, 0, 0));
  // stack bars on top of each other:
  nuclear->moveAbove(fossil);
//regen->moveAbove(nuclear);




  /*QCPBars *regen = new QCPBars(customPlot->xAxis, customPlot->yAxis);
  QCPBars *nuclear = new QCPBars(customPlot->xAxis, customPlot->yAxis);
  QCPBars *fossil = new QCPBars(customPlot->xAxis, customPlot->yAxis);
  regen->setAntialiased(false); // gives more crisp, pixel aligned bar borders
  nuclear->setAntialiased(false);
  fossil->setAntialiased(false);
  regen->setStackingGap(1);
  nuclear->setStackingGap(1);
  fossil->setStackingGap(1);
  // set names and colors:
  fossil->setName("Fossil fuels");
  fossil->setPen(QPen(QColor(111, 9, 176).lighter(170)));
  fossil->setBrush(QColor(111, 9, 176));
  nuclear->setName("Nuclear");
  nuclear->setPen(QPen(QColor(250, 170, 20).lighter(150)));
  nuclear->setBrush(QColor(250, 170, 20));
  regen->setName("Regenerative");
  regen->setPen(QPen(QColor(0, 168, 140).lighter(130)));
  regen->setBrush(QColor(0, 168, 140));
  // stack bars on top of each other:
  nuclear->moveAbove(fossil);
  regen->moveAbove(nuclear);*/



  // prepare x axis with country labels:
  QVector<double> ticks;
  QVector<QString> labels;
  ticks << 1 << 2 << 3 << 4 ;
  labels <<QStringLiteral("1号车") << QStringLiteral("2号车") << QStringLiteral("3号车") << QStringLiteral("4号车") ;
  QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
  textTicker->addTicks(ticks, labels);
  customPlot->xAxis->setTicker(textTicker);
  customPlot->xAxis->setTickLabelRotation(60);
  customPlot->xAxis->setSubTicks(false);
  customPlot->xAxis->setTickLength(0, 4);
  customPlot->xAxis->setRange(0, 5);
  customPlot->xAxis->setBasePen(QPen(Qt::white));
  customPlot->xAxis->setTickPen(QPen(Qt::white));
  customPlot->xAxis->grid()->setVisible(true);
  customPlot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
  customPlot->xAxis->setTickLabelColor(Qt::white);
  customPlot->xAxis->setLabelColor(Qt::white);

  // prepare y axis:
  customPlot->yAxis->setRange(0, 15);
  customPlot->yAxis->setPadding(5); // a bit more space to the left border
  customPlot->yAxis->setLabel( QStringLiteral("运行时间（h）"));
  customPlot->yAxis->setBasePen(QPen(Qt::white));
  customPlot->yAxis->setTickPen(QPen(Qt::white));
  customPlot->yAxis->setSubTickPen(QPen(Qt::white));
  customPlot->yAxis->grid()->setSubGridVisible(true);
  customPlot->yAxis->setTickLabelColor(Qt::white);
  customPlot->yAxis->setLabelColor(Qt::white);
 customPlot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
  customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

  // Add data:
  QVector<double> fossilData, nuclearData;
  fossilData  << 10<< 8 << 9 << 4 ;
  nuclearData << 2 << 3 << 2.5 << 1 ;

  //fossilData  << 0.86*10.5 << 0.83*5.5 << 0.84*5.5 << 0.52*5.8 ;
//  nuclearData << 0.08*10.5 << 0.12*5.5 << 0.12*5.5 << 0.40*5.8 ;
 // regenData   << 0.06*10.5 << 0.05*5.5 << 0.04*5.5 << 0.06*5.8 << 0.02*5.2 << 0.07*4.2 << 0.25*11.2;
  fossil->setData(ticks, fossilData);
  nuclear->setData(ticks, nuclearData);
 // regen->setData(ticks, regenData);

  // setup legend:
  customPlot->legend->setVisible(true);
  customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
  customPlot->legend->setBrush(QColor(255, 255, 255, 20));
  customPlot->legend->setBorderPen(Qt::NoPen);
  QFont legendFont = font();
  legendFont.setPointSize(10);
  customPlot->legend->setFont(legendFont);
  customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}


void StatisticsPage::setupStyledDemo(QCustomPlot *customPlot)
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
