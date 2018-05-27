/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_mainwindow_H
#define UI_mainwindow_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/Qmainwindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mainwindow
{
public:
    QAction *actionDebugger;
    QAction *actionMaxinize;
    QAction *actionMininize;
    QAction *actionNormalize;
    QAction *actionFullScreen;
    QAction *actionAbout;
    QAction *actionTmcWindow;
    QAction *actionBarrier;
    QAction *actionAgvSchedule;
    QAction *actionBatteryRecord;
    QAction *actionTmc_FIFO;
    QAction *actionAgvTaskRecord;
    QAction *actionTmc_MaxNum;
    QAction *actionClose_Tmc;
    QAction *actionTimeout_Go;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout_4;
    QWidget *mapShowWidget;
    QVBoxLayout *verticalLayout_8;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QComboBox *agvControlComboBox;
    QGridLayout *gridLayout;
    QPushButton *unlockButton;
    QPushButton *stopButton;
    QPushButton *resetButton;
    QPushButton *startButton;
    QGridLayout *agvShowGridLayout;
    QMenuBar *menuBar;
    QMenu *menu_AGV;
    QMenu *menu;
    QMenu *menuOption;
    QMenu *menuHelp;
    QStatusBar *statusBar;

    void setupUi(Qmainwindow *mainwindow)
    {
        if (mainwindow->objectName().isEmpty())
            mainwindow->setObjectName(QStringLiteral("mainwindow"));
        mainwindow->resize(734, 565);
        mainwindow->setAcceptDrops(false);
        mainwindow->setStyleSheet(QStringLiteral(""));
        actionDebugger = new QAction(mainwindow);
        actionDebugger->setObjectName(QStringLiteral("actionDebugger"));
        actionMaxinize = new QAction(mainwindow);
        actionMaxinize->setObjectName(QStringLiteral("actionMaxinize"));
        actionMininize = new QAction(mainwindow);
        actionMininize->setObjectName(QStringLiteral("actionMininize"));
        actionNormalize = new QAction(mainwindow);
        actionNormalize->setObjectName(QStringLiteral("actionNormalize"));
        actionFullScreen = new QAction(mainwindow);
        actionFullScreen->setObjectName(QStringLiteral("actionFullScreen"));
        actionAbout = new QAction(mainwindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionTmcWindow = new QAction(mainwindow);
        actionTmcWindow->setObjectName(QStringLiteral("actionTmcWindow"));
        actionBarrier = new QAction(mainwindow);
        actionBarrier->setObjectName(QStringLiteral("actionBarrier"));
        actionBarrier->setCheckable(true);
        actionBarrier->setChecked(true);
        actionAgvSchedule = new QAction(mainwindow);
        actionAgvSchedule->setObjectName(QStringLiteral("actionAgvSchedule"));
        actionBatteryRecord = new QAction(mainwindow);
        actionBatteryRecord->setObjectName(QStringLiteral("actionBatteryRecord"));
        actionBatteryRecord->setCheckable(false);
        actionTmc_FIFO = new QAction(mainwindow);
        actionTmc_FIFO->setObjectName(QStringLiteral("actionTmc_FIFO"));
        actionTmc_FIFO->setCheckable(true);
        actionTmc_FIFO->setChecked(false);
        actionTmc_FIFO->setEnabled(true);
        actionAgvTaskRecord = new QAction(mainwindow);
        actionAgvTaskRecord->setObjectName(QStringLiteral("actionAgvTaskRecord"));
        actionTmc_MaxNum = new QAction(mainwindow);
        actionTmc_MaxNum->setObjectName(QStringLiteral("actionTmc_MaxNum"));
        actionTmc_MaxNum->setCheckable(true);
        actionTmc_MaxNum->setChecked(true);
        actionClose_Tmc = new QAction(mainwindow);
        actionClose_Tmc->setObjectName(QStringLiteral("actionClose_Tmc"));
        actionClose_Tmc->setCheckable(true);
        actionClose_Tmc->setChecked(false);
        actionTimeout_Go = new QAction(mainwindow);
        actionTimeout_Go->setObjectName(QStringLiteral("actionTimeout_Go"));
        actionTimeout_Go->setCheckable(true);
        actionTimeout_Go->setChecked(true);
        centralWidget = new QWidget(mainwindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_5 = new QVBoxLayout(centralWidget);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        mapShowWidget = new QWidget(centralWidget);
        mapShowWidget->setObjectName(QStringLiteral("mapShowWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(mapShowWidget->sizePolicy().hasHeightForWidth());
        mapShowWidget->setSizePolicy(sizePolicy);
        mapShowWidget->setMinimumSize(QSize(0, 0));
        mapShowWidget->setBaseSize(QSize(600, 400));
        mapShowWidget->setStyleSheet(QStringLiteral(""));
        verticalLayout_8 = new QVBoxLayout(mapShowWidget);
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));

        verticalLayout_4->addWidget(mapShowWidget);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setMaximumSize(QSize(16777215, 211));
        verticalLayout_3 = new QVBoxLayout(groupBox);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        groupBox_2 = new QGroupBox(groupBox);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy1);
        groupBox_2->setMinimumSize(QSize(170, 170));
        groupBox_2->setMaximumSize(QSize(170, 170));
        verticalLayout = new QVBoxLayout(groupBox_2);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy2);

        horizontalLayout_2->addWidget(label);

        agvControlComboBox = new QComboBox(groupBox_2);
        agvControlComboBox->setObjectName(QStringLiteral("agvControlComboBox"));

        horizontalLayout_2->addWidget(agvControlComboBox);


        verticalLayout->addLayout(horizontalLayout_2);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        unlockButton = new QPushButton(groupBox_2);
        unlockButton->setObjectName(QStringLiteral("unlockButton"));

        gridLayout->addWidget(unlockButton, 1, 0, 1, 1);

        stopButton = new QPushButton(groupBox_2);
        stopButton->setObjectName(QStringLiteral("stopButton"));

        gridLayout->addWidget(stopButton, 0, 1, 1, 1);

        resetButton = new QPushButton(groupBox_2);
        resetButton->setObjectName(QStringLiteral("resetButton"));

        gridLayout->addWidget(resetButton, 1, 1, 1, 1);

        startButton = new QPushButton(groupBox_2);
        startButton->setObjectName(QStringLiteral("startButton"));

        gridLayout->addWidget(startButton, 0, 0, 1, 1);


        verticalLayout->addLayout(gridLayout);


        horizontalLayout->addWidget(groupBox_2);

        agvShowGridLayout = new QGridLayout();
        agvShowGridLayout->setSpacing(6);
        agvShowGridLayout->setObjectName(QStringLiteral("agvShowGridLayout"));
        agvShowGridLayout->setSizeConstraint(QLayout::SetNoConstraint);
        agvShowGridLayout->setHorizontalSpacing(2);
        agvShowGridLayout->setVerticalSpacing(10);
        agvShowGridLayout->setContentsMargins(2, -1, -1, -1);

        horizontalLayout->addLayout(agvShowGridLayout);

        verticalLayout_3->addLayout(horizontalLayout);


        verticalLayout_4->addWidget(groupBox);


        verticalLayout_5->addLayout(verticalLayout_4);

        mainwindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(mainwindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 734, 26));
        menu_AGV = new QMenu(menuBar);
        menu_AGV->setObjectName(QStringLiteral("menu_AGV"));
        menu = new QMenu(menu_AGV);
        menu->setObjectName(QStringLiteral("menu"));
        menuOption = new QMenu(menuBar);
        menuOption->setObjectName(QStringLiteral("menuOption"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        mainwindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(mainwindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        mainwindow->setStatusBar(statusBar);

        menuBar->addAction(menu_AGV->menuAction());
        menuBar->addAction(menuOption->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menu_AGV->addAction(actionBarrier);
        menu_AGV->addAction(menu->menuAction());
        menu_AGV->addAction(actionBatteryRecord);
        menu_AGV->addAction(actionAgvSchedule);
        menu_AGV->addAction(actionAgvTaskRecord);
        menu->addAction(actionTmc_MaxNum);
        menu->addAction(actionTmc_FIFO);
        menu->addAction(actionTimeout_Go);
        menu->addAction(actionClose_Tmc);
        menuOption->addAction(actionMaxinize);
        menuOption->addAction(actionMininize);
        menuOption->addAction(actionNormalize);
        menuOption->addAction(actionFullScreen);
        menuOption->addAction(actionDebugger);
        menuOption->addAction(actionTmcWindow);
        menuHelp->addAction(actionAbout);

        retranslateUi(mainwindow);
        QObject::connect(actionMininize, SIGNAL(triggered()), mainwindow, SLOT(showMinimized()));
        QObject::connect(actionNormalize, SIGNAL(triggered()), mainwindow, SLOT(showNormal()));
        QObject::connect(actionMaxinize, SIGNAL(triggered()), mainwindow, SLOT(showMaximized()));
        QObject::connect(actionFullScreen, SIGNAL(triggered()), mainwindow, SLOT(showFullScreen()));

        QMetaObject::connectSlotsByName(mainwindow);
    } // setupUi

    void retranslateUi(Qmainwindow *mainwindow)
    {
        mainwindow->setWindowTitle(QApplication::translate("mainwindow", "AGV\347\256\241\347\220\206\347\263\273\347\273\237-\344\270\212\346\265\267\344\274\237\344\270\226\351\200\232\346\261\275\350\275\246\347\224\265\345\255\220\346\234\211\351\231\220\345\205\254\345\217\270", 0));
        actionDebugger->setText(QApplication::translate("mainwindow", "\347\250\213\345\272\217\346\224\266\345\217\221\347\252\227\345\217\243", 0));
        actionMaxinize->setText(QApplication::translate("mainwindow", "\346\234\200\345\244\247\345\214\226", 0));
        actionMininize->setText(QApplication::translate("mainwindow", "\346\234\200\345\260\217\345\214\226", 0));
        actionNormalize->setText(QApplication::translate("mainwindow", "\346\231\256\351\200\232\346\230\276\347\244\272", 0));
        actionFullScreen->setText(QApplication::translate("mainwindow", "\345\205\250\345\261\217\346\230\276\347\244\272", 0));
        actionAbout->setText(QApplication::translate("mainwindow", "\345\205\263\344\272\216\350\275\257\344\273\266", 0));
        actionTmcWindow->setText(QApplication::translate("mainwindow", "\347\256\241\345\210\266\351\223\276\350\241\250\347\252\227\345\217\243", 0));
        actionBarrier->setText(QApplication::translate("mainwindow", "\351\232\234\347\242\215\347\224\237\346\225\210\346\243\200\346\265\213", 0));
        actionAgvSchedule->setText(QApplication::translate("mainwindow", "AGV\350\260\203\345\272\246", 0));
        actionBatteryRecord->setText(QApplication::translate("mainwindow", "\347\224\265\346\261\240\346\233\264\346\215\242\350\256\260\345\275\225", 0));
        actionTmc_FIFO->setText(QApplication::translate("mainwindow", "\345\205\210\350\277\233\345\205\210\345\207\272\346\250\241\345\274\217", 0));
        actionAgvTaskRecord->setText(QApplication::translate("mainwindow", "AGV\344\273\273\345\212\241\350\256\260\345\275\225", 0));
        actionTmc_MaxNum->setText(QApplication::translate("mainwindow", "\345\244\247\345\217\267\345\205\210\350\241\214\346\250\241\345\274\217", 0));
        actionClose_Tmc->setText(QApplication::translate("mainwindow", "\345\205\263\351\227\255\344\270\273\346\216\247\344\272\244\347\256\241", 0));
        actionTimeout_Go->setText(QApplication::translate("mainwindow", "\350\266\205\346\227\266\350\207\252\345\212\250\346\224\276\350\241\214", 0));
        groupBox->setTitle(QApplication::translate("mainwindow", "\345\212\237\350\203\275\346\250\241\345\235\227", 0));
        groupBox_2->setTitle(QApplication::translate("mainwindow", "AGV\346\216\247\345\210\266", 0));
        label->setText(QApplication::translate("mainwindow", "A G V\n"
"\347\274\226 \345\217\267", 0));
        unlockButton->setText(QApplication::translate("mainwindow", "\350\247\243\351\224\201", 0));
        stopButton->setText(QApplication::translate("mainwindow", "\345\201\234\346\255\242", 0));
        resetButton->setText(QApplication::translate("mainwindow", "\345\244\215\344\275\215", 0));
        startButton->setText(QApplication::translate("mainwindow", "\345\220\257\345\212\250", 0));
        menu_AGV->setTitle(QApplication::translate("mainwindow", "AGV\347\256\241\347\220\206", 0));
        menu->setTitle(QApplication::translate("mainwindow", "\350\267\257\345\217\243\344\272\244\351\200\232\347\256\241\345\210\266", 0));
        menuOption->setTitle(QApplication::translate("mainwindow", "\347\252\227\345\217\243\351\200\211\351\241\271", 0));
        menuHelp->setTitle(QApplication::translate("mainwindow", "\345\270\256\345\212\251", 0));
    } // retranslateUi

};

namespace Ui {
    class mainwindow: public ui_mainwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_mainwindow_H
