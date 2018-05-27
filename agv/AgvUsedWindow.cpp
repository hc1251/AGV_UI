#include "AgvUsedWindow.h"
#include "ui_AgvUsedWindow.h"

AgvUsedWindow::AgvUsedWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AgvUsedWindow)
{
    ui->setupUi(this);
}

AgvUsedWindow::~AgvUsedWindow()
{
    delete ui;
}
