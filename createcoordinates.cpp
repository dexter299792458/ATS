#include "createcoordinates.h"
#include "ui_createcoordinates.h"

CreateCoordinates::CreateCoordinates(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateCoordinates)
{
    ui->setupUi(this);
}

CreateCoordinates::~CreateCoordinates()
{
    delete ui;
}

void CreateCoordinates::on_buttonBox_accepted()
{

}

void CreateCoordinates::ReturnCoordinatesToUI()
{

}
