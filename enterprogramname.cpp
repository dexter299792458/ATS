#include "enterprogramname.h"
#include "ui_enterprogramname.h"
#include <QDebug>

EnterProgramName::EnterProgramName(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EnterProgramName)
{
    ui->setupUi(this);
    ui->lineEdit->setFocus();
}

EnterProgramName::~EnterProgramName()
{
    delete ui;
}

void EnterProgramName::on_buttonBox_accepted()
{

}

QString EnterProgramName::ReturnEnteredProgramName()
{
    return ui->lineEdit->text();
}
