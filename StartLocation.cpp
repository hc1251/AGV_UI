#include "StartLocation.h"
#include "ui_StartLocation.h"

StartLocation::StartLocation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartLocation)
{
    ui->setupUi(this);
}

StartLocation::~StartLocation()
{
    delete ui;
}
