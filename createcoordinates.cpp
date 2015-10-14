#include "createcoordinates.h"
#include "ui_createcoordinates.h"

CreateCoordinates::CreateCoordinates(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateCoordinates)
{
    ui->setupUi(this);
    ui->Loading->setVisible(false);
}

CreateCoordinates::~CreateCoordinates()
{
    delete ui;
}

void CreateCoordinates::on_buttonBox_accepted()
{
    ui->Loading->setVisible(true);
    coordinate.append("defp\x20" + ui->PosName->text() + "\x00D");
    coordinate.append("teach\x20" + ui->PosName->text() + "\x00D" );
    coordinate.append(ui->X->text() + "\x00D");
    coordinate.append(ui->Y->text() + "\x00D");
    coordinate.append(ui->Z->text() + "\x00D");
    coordinate.append(ui->P->text() + "\x00D");
    coordinate.append(ui->R->text() + "\x00D");
    CreateCoordinates::SendCoordinatesToController(coordinate);
}

void CreateCoordinates::SendCoordinatesToController(QStringList sendCoordinate)
{
    singleton_SerialPortManager = SerialPortManager::GetInstance();
    singleton_SerialPortManager->WriteMultipleACLCommands(sendCoordinate, WRITE_TO_CONSOLE);
    coordinate.clear();
}
