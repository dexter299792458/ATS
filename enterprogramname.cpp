#include "enterprogramname.h"
#include "ui_enterprogramname.h"

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
